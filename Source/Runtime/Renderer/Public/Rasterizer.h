#pragma once

#include <cassert>

#include "RSIDataTypes.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include "RenderingSoftwareInterface.h"
#include "ScreenPoint.h"
#include "LinearColor.h"

class Rasterizer final
{
public:

	Rasterizer(const std::shared_ptr<RenderingSoftwareInterface>& InRSI) 
		: mRSI(InRSI)
		, mCurrentPrimitiveData(nullptr)
		, mCullMode(CullingMode::CULL_BACK)
	{}
	~Rasterizer() = default;

	FORCEINLINE void Rasterize(VertexShader::VertexOutput* PrimitiveData);

	FORCEINLINE void RasterizeBottomFlatPrimitive(const VertexShader::VertexOutput* InPrimitiveData);
	FORCEINLINE void RasterizeBottomFlatPrimitive(const Vector4& Point1, const Vector4& Point2, const Vector4& Point3);
	FORCEINLINE void RasterizeTopFlatPrimitive(const VertexShader::VertexOutput* InPrimitiveData);
	FORCEINLINE void RasterizeTopFlatPrimitive(const Vector4& Point1, const Vector4& Point2, const Vector4& Point3);

	FORCEINLINE void WriteFlatLine(const Vector4& StartPoint, const Vector4& EndPoint);
	FORCEINLINE void WriteGeometryOutline(const Vector4& StartPoint, const Vector4& EndPoint);

	FORCEINLINE void GetBaycentricCoodinate(const Vector2& InPosition, Vector3& OutCoord);
	FORCEINLINE void GetInterpolratedFragment(const Vector2& InPosition, FragmentShader::FragmentInput* OutPrimitive);
	FORCEINLINE void SortVertexByY(VertexShader::VertexOutput* PrimitiveData);
	FORCEINLINE bool IsFrontNormal(VertexShader::VertexOutput* PrimitiveData);
	FORCEINLINE void CalculrateCachedDotProduct();
	
	FORCEINLINE void SetRasterizerState(const ScreenPoint& InScreenSize, bool InUseGeometryOutline, bool InUseRasterization, CullingMode InCullMode);
	FORCEINLINE void SetFragmentShader(const std::shared_ptr<FragmentShader>& InFragShader);

private:

	std::shared_ptr<FragmentShader> mFragmentShader;

	std::shared_ptr<RenderingSoftwareInterface> mRSI;
	VertexShader::VertexOutput* mCurrentPrimitiveData;

	ScreenPoint mScreenSize;
	Vector2 ScreenLimit;
	bool bUseGeometryOutline = false;
	bool bUseRasterization = true;
	CullingMode mCullMode;

	Vector4 mPrimitiveVectorU;
	Vector4 mPrimitiveVectorV;
	Vector4 mPrimitiveVectorW;
	float mDotUU = 0.0f;
	float mDotUV = 0.0f;
	float mDotVV = 0.0f;
	float mDenominator = 0.0f;

};

FORCEINLINE void Rasterizer::Rasterize(VertexShader::VertexOutput* PrimitiveData)
{
	if (PrimitiveData == nullptr)
	{
		return;
	}

	// 3개 버텍스를 기준으로 Z Test를 실시.
	bool ZTestOne = PrimitiveData[PRIMITIVE_INDEX_ONE].Position.Z > 1.0f || PrimitiveData[PRIMITIVE_INDEX_ONE].Position.Z < 0.0f;
	bool ZTestTwo = PrimitiveData[PRIMITIVE_INDEX_TWO].Position.Z > 1.0f || PrimitiveData[PRIMITIVE_INDEX_TWO].Position.Z < 0.0f;
	bool ZTestThree = PrimitiveData[PRIMITIVE_INDEX_THREE].Position.Z > 1.0f || PrimitiveData[PRIMITIVE_INDEX_THREE].Position.Z < 0.0f;
	if (ZTestOne || ZTestTwo || ZTestThree)
	{
		return;
	}

	// NDC Space에서 ScreenSpace로 변환.
	for (int i = 0; i < PRIMITIVE_COUNT; ++i)
	{
		PrimitiveData[i].Position.X *= mScreenSize.Y;
		PrimitiveData[i].Position.Y *= mScreenSize.X;
	}

	switch (mCullMode)
	{
	case CullingMode::CULL_FRONT:
		if (IsFrontNormal(PrimitiveData) == true)
		{
			return;
		}
		break;
	case CullingMode::CULL_BACK:
		if (IsFrontNormal(PrimitiveData) == false)
		{
			return;
		}
		break;
	}

	// Vertex Sort
	SortVertexByY(PrimitiveData);

	// Allocate Current Primitive
	mCurrentPrimitiveData = PrimitiveData;

	// Rasterization 기능이 활성화 되어 있으면 실행.
	if (bUseRasterization == true)
	{
		// Calculrate Dotproduct Cache
		CalculrateCachedDotProduct();

		if (Math::IsNearlyFloat(PrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y, PrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y) == true)
		{
			RasterizeTopFlatPrimitive(PrimitiveData);
		}
		else if (Math::IsNearlyFloat(PrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y, PrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y) == true)
		{
			RasterizeBottomFlatPrimitive(PrimitiveData);
		}
		else
		{
			Vector4 NewPosition;
			NewPosition.Y = PrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y;
			NewPosition.X = (PrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y - PrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y) / (PrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y - PrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y)
				* (PrimitiveData[PRIMITIVE_INDEX_THREE].Position.X - PrimitiveData[PRIMITIVE_INDEX_ONE].Position.X) + PrimitiveData[PRIMITIVE_INDEX_ONE].Position.X;
			RasterizeTopFlatPrimitive(PrimitiveData[PRIMITIVE_INDEX_TWO].Position, NewPosition, PrimitiveData[PRIMITIVE_INDEX_THREE].Position);
			RasterizeBottomFlatPrimitive(PrimitiveData[PRIMITIVE_INDEX_ONE].Position, PrimitiveData[PRIMITIVE_INDEX_TWO].Position, NewPosition);
		}
	}

	// GeometryOutline 기능이 활성화 되어 있으면 실행.
	if (bUseGeometryOutline == true)
	{
		WriteGeometryOutline(PrimitiveData[PRIMITIVE_INDEX_ONE].Position, PrimitiveData[PRIMITIVE_INDEX_TWO].Position);
		WriteGeometryOutline(PrimitiveData[PRIMITIVE_INDEX_TWO].Position, PrimitiveData[PRIMITIVE_INDEX_THREE].Position);
		WriteGeometryOutline(PrimitiveData[PRIMITIVE_INDEX_THREE].Position, PrimitiveData[PRIMITIVE_INDEX_ONE].Position);
	}
}

FORCEINLINE void Rasterizer::RasterizeBottomFlatPrimitive(const VertexShader::VertexOutput* InPrimitiveData)
{
	float SlopeL = (InPrimitiveData[PRIMITIVE_INDEX_TWO].Position.X - InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.X)
		/ (InPrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y - InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y);
	float SlopeR = (InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.X - InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.X)
		/ (InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y - InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y);

	Vector4 StartPoint;
	Vector4 EndPoint;
	StartPoint = EndPoint = InPrimitiveData[PRIMITIVE_INDEX_ONE].Position;

	for (float PosY = InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y; PosY >= InPrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y; --PosY)
	{
		WriteFlatLine(StartPoint, EndPoint);

		StartPoint.X -= SlopeL;
		EndPoint.X -= SlopeR;
		--StartPoint.Y;
		--EndPoint.Y;
	}
}

FORCEINLINE void Rasterizer::RasterizeBottomFlatPrimitive(const Vector4& Point1, const Vector4& Point2, const Vector4& Point3)
{
	float SlopeL = (Point2.X - Point1.X) / (Point2.Y - Point1.Y);
	float SlopeR = (Point3.X - Point1.X) / (Point3.Y - Point1.Y);

	Vector4 StartPoint;
	Vector4 EndPoint;
	StartPoint = EndPoint = Point1;

	for (float PosY = Point1.Y; PosY >= Point2.Y; --PosY)
	{
		WriteFlatLine(StartPoint, EndPoint);

		StartPoint.X -= SlopeL;
		EndPoint.X -= SlopeR;
		--StartPoint.Y;
		--EndPoint.Y;
	}
}

FORCEINLINE void Rasterizer::RasterizeTopFlatPrimitive(const VertexShader::VertexOutput* InPrimitiveData)
{
	float SlopeL = (InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.X - InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.X)
		/ (InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y - InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y);
	float SlopeR = (InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.X - InPrimitiveData[PRIMITIVE_INDEX_TWO].Position.X)
		/ (InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y - InPrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y);

	Vector4 StartPoint;
	Vector4 EndPoint;
	StartPoint = EndPoint = InPrimitiveData[PRIMITIVE_INDEX_THREE].Position;

	for (float PosY = InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y; PosY <= InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y; ++PosY)
	{
		WriteFlatLine(StartPoint, EndPoint);

		StartPoint.X += SlopeL;
		EndPoint.X += SlopeR;
		++StartPoint.Y;
		++EndPoint.Y;
	}
}

FORCEINLINE void Rasterizer::RasterizeTopFlatPrimitive(const Vector4& Point1, const Vector4& Point2, const Vector4& Point3)
{
	float SlopeL = (Point3.X - Point1.X) / (Point3.Y - Point1.Y);
	float SlopeR = (Point3.X - Point2.X) / (Point3.Y - Point2.Y);

	Vector4 StartPoint;
	Vector4 EndPoint;
	StartPoint = EndPoint = Point3;

	for (float PosY = Point3.Y; PosY <= Point1.Y; ++PosY)
	{
		WriteFlatLine(StartPoint, EndPoint);

		StartPoint.X += SlopeL;
		EndPoint.X += SlopeR;
		++StartPoint.Y;
		++EndPoint.Y;
	}
}

FORCEINLINE void Rasterizer::WriteFlatLine(const Vector4& StartPoint, const Vector4& EndPoint)
{
	// Draw FlatLine;
	float EndX = Math::Max(StartPoint.X, EndPoint.X);
	for (float PosX = Math::Min(StartPoint.X, EndPoint.X); PosX <= EndX; ++PosX)
	{
		if (PosX < -ScreenLimit.X || PosX > ScreenLimit.X || StartPoint.Y < -ScreenLimit.Y || StartPoint.Y > ScreenLimit.Y)
		{
			continue;
		}

		Vector2 CurrentPosition(PosX, StartPoint.Y);

		FragmentShader::FragmentInput FragmentData;
		GetInterpolratedFragment(CurrentPosition, &FragmentData);
		mFragmentShader->ProcessFragmentShader(FragmentData);
	}
}

FORCEINLINE void Rasterizer::WriteGeometryOutline(const Vector4& StartPoint, const Vector4& EndPoint)
{
	ScreenPoint CurrentPoint = ScreenPoint::ToScreenCoordinate(mScreenSize, StartPoint.ToVector2());

	// Tempolary Code
	LinearColor OutlineColor = LinearColor::Red;

	int DeltaX = Math::FloorToInt(Math::Abs(EndPoint.X - StartPoint.X));
	int DeltaY = Math::FloorToInt(Math::Abs(EndPoint.Y - StartPoint.Y));
	int DirX = Math::Sign(EndPoint.X - StartPoint.X);
	int DirY = Math::Sign(EndPoint.Y - StartPoint.Y);

	bool IsSwapped = false;
	if (DeltaY > DeltaX) // 1 ~ 의 기울기
	{
		Math::Swap(DeltaX, DeltaY);
		IsSwapped = true;
	}

	int Judge = 2 * DeltaX - DeltaY;

	for (int CurX = 0; CurX < DeltaX; ++CurX)
	{
		// Out Boarder Exception
		if (CurrentPoint.X >= 0 && CurrentPoint.X <= mScreenSize.X && CurrentPoint.Y >= 0 && CurrentPoint.Y <= mScreenSize.Y)
		{
			mRSI->DrawPoint(CurrentPoint, OutlineColor);
		}

		if (Judge >= 0)
		{
			if (IsSwapped == true)
			{
				CurrentPoint.X += DirX;
			}
			else
			{
				CurrentPoint.Y -= DirY; // 스크린 스페이스의 Y좌표가 반전되어 있음.
			}

			Judge -= 2 * DeltaX;
		}

		if (IsSwapped == true)
		{
			CurrentPoint.Y -= DirY; // 스크린 스페이스의 Y좌표가 반전되어 있음.
		}
		else
		{
			CurrentPoint.X += DirX;
		}

		Judge += 2 * DeltaY;
	}
}


FORCEINLINE void Rasterizer::GetBaycentricCoodinate(const Vector2& InPosition, Vector3& OutCoord)
{
	// Calculrate BaycentiricCoodinate;
	mPrimitiveVectorW = InPosition - mCurrentPrimitiveData[PRIMITIVE_INDEX_ONE].Position.ToVector2();
	mPrimitiveVectorW.Z = mPrimitiveVectorW.W = 0.0f;

	OutCoord.Y = (mPrimitiveVectorW.Dot(mPrimitiveVectorU) * mDotVV - mPrimitiveVectorW.Dot(mPrimitiveVectorV) * mDotUV) / mDenominator;
	OutCoord.Z = (mPrimitiveVectorW.Dot(mPrimitiveVectorV) * mDotUU - mPrimitiveVectorW.Dot(mPrimitiveVectorU) * mDotUV) / mDenominator;
	OutCoord.X = 1.0f - OutCoord.Y - OutCoord.Z;
}

FORCEINLINE void Rasterizer::GetInterpolratedFragment(const Vector2& InPosition, FragmentShader::FragmentInput* OutPrimitive)
{
	// return InterpolratedPrimitive;
	Vector3 BaycentricWeight;
	GetBaycentricCoodinate(InPosition, BaycentricWeight);

	// Do not Interpolrate
	OutPrimitive->Position = InPosition;

	// Interpolrate
	OutPrimitive->WorldPosition = mCurrentPrimitiveData[PRIMITIVE_INDEX_ONE].WorldPosition * BaycentricWeight.X
		+ mCurrentPrimitiveData[PRIMITIVE_INDEX_TWO].WorldPosition * BaycentricWeight.Y
		+ mCurrentPrimitiveData[PRIMITIVE_INDEX_THREE].WorldPosition * BaycentricWeight.Z;

	OutPrimitive->WorldNormal = mCurrentPrimitiveData[PRIMITIVE_INDEX_ONE].WorldNormal * BaycentricWeight.X
		+ mCurrentPrimitiveData[PRIMITIVE_INDEX_TWO].WorldNormal * BaycentricWeight.Y
		+ mCurrentPrimitiveData[PRIMITIVE_INDEX_THREE].WorldNormal * BaycentricWeight.Z;
}

FORCEINLINE void Rasterizer::SortVertexByY(VertexShader::VertexOutput* PrimitiveData)
{
	if ((PrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y <= PrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y) ||
		(PrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y <= PrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y))
	{
		if (PrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y > PrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y)
		{
			Math::Swap(PrimitiveData[PRIMITIVE_INDEX_ONE], PrimitiveData[PRIMITIVE_INDEX_TWO]);
		}
		else
		{
			Math::Swap(PrimitiveData[PRIMITIVE_INDEX_ONE], PrimitiveData[PRIMITIVE_INDEX_THREE]);
		}
	}

	if (PrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y < PrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y)
	{
		Math::Swap(PrimitiveData[PRIMITIVE_INDEX_TWO], PrimitiveData[PRIMITIVE_INDEX_THREE]);
	}
}

FORCEINLINE bool Rasterizer::IsFrontNormal(VertexShader::VertexOutput* PrimitiveData)
{
	// 2D Vector에 대해 외적을 하여 Z값을 얻음
	Vector2 PrimitiveVector1 = PrimitiveData[PRIMITIVE_INDEX_THREE].Position.ToVector2() - PrimitiveData[PRIMITIVE_INDEX_TWO].Position.ToVector2();
	Vector2 PrimitiveVector2 = PrimitiveData[PRIMITIVE_INDEX_THREE].Position.ToVector2() - PrimitiveData[PRIMITIVE_INDEX_ONE].Position.ToVector2();
	float PrimitiveZ = (PrimitiveVector1.X * PrimitiveVector2.Y) - (PrimitiveVector1.Y * PrimitiveVector2.X);

	return PrimitiveZ > 0;
}

FORCEINLINE void Rasterizer::CalculrateCachedDotProduct()
{
	mPrimitiveVectorU = (mCurrentPrimitiveData[PRIMITIVE_INDEX_TWO].Position - mCurrentPrimitiveData[PRIMITIVE_INDEX_ONE].Position);
	mPrimitiveVectorV = (mCurrentPrimitiveData[PRIMITIVE_INDEX_THREE].Position - mCurrentPrimitiveData[PRIMITIVE_INDEX_ONE].Position);
	mPrimitiveVectorU.Z = mPrimitiveVectorU.W = 0.0f;
	mPrimitiveVectorV.Z = mPrimitiveVectorV.W = 0.0f;

	mDotUU = mPrimitiveVectorU.Dot(mPrimitiveVectorU);
	mDotUV = mPrimitiveVectorU.Dot(mPrimitiveVectorV);
	mDotVV = mPrimitiveVectorV.Dot(mPrimitiveVectorV);

	mDenominator = (mDotUU * mDotVV) - (mDotUV * mDotUV);
}


FORCEINLINE void Rasterizer::SetRasterizerState(const ScreenPoint& InScreenSize, bool InUseGeometryOutline, bool InUseRasterization, CullingMode InCullMode)
{
	mScreenSize = InScreenSize;
	bUseGeometryOutline = InUseGeometryOutline;
	bUseRasterization = InUseRasterization;
	mCullMode = InCullMode;

	ScreenLimit.X = mScreenSize.X * 0.5f;
	ScreenLimit.Y = mScreenSize.Y * 0.5f;
}

FORCEINLINE void Rasterizer::SetFragmentShader(const std::shared_ptr<FragmentShader>& InFragShader)
{
	mFragmentShader = InFragShader;
}

