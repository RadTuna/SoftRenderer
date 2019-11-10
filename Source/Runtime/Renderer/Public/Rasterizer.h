#pragma once

#include "RSIDataTypes.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include "RenderingSoftwareInterface.h"
#include "ScreenPoint.h"
#include "LinearColor.h"

class Rasterizer final
{
public:

	Rasterizer(RenderingSoftwareInterface* InRSI) : RSI(InRSI), FragmentShaderFunc(nullptr) {}
	~Rasterizer() = default;

	FORCEINLINE void Rasterize(VertexShader::VertexOutput* PrimitiveData);

	FORCEINLINE void RasterizeBottomFlatPrimitive(const VertexShader::VertexOutput* InPrimitiveData);
	FORCEINLINE void RasterizeBottomFlatPrimitive(const Vector4& Point1, const Vector4& Point2, const Vector4& Point3);
	FORCEINLINE void RasterizeTopFlatPrimitive(const VertexShader::VertexOutput* InPrimitiveData);
	FORCEINLINE void RasterizeTopFlatPrimitive(const Vector4& Point1, const Vector4& Point2, const Vector4& Point3);

	FORCEINLINE void WriteFlatLine(const Vector4& StartPoint, const Vector4& EndPoint); // WIP
	FORCEINLINE void WriteGeometryOutline(const Vector4& StartPoint, const Vector4& EndPoint);

	FORCEINLINE void GetBaycentricCoodinate(); // WIP
	FORCEINLINE void GetInterpolratedPrimitive(); // WIP
	FORCEINLINE void SortVertexByY(VertexShader::VertexOutput* PrimitiveData);
	FORCEINLINE bool IsFrontNormal(VertexShader::VertexOutput* PrimitiveData);
	
	FORCEINLINE void SetFragmentShaderFunction(void(FragmentShader::*InFragmentShaderFunc)(FragmentShader::FragmentInput*, UINT));
	FORCEINLINE void SetRasterizerState(const ScreenPoint& InScreenSize, bool InUseGeometryOutline, bool InUseRasterization, CullingMode InCullMode);

private:

	// FragmentShader ProcessFunction;
	void(FragmentShader::*FragmentShaderFunc)(FragmentShader::FragmentInput*, UINT);

	RenderingSoftwareInterface* RSI;
	ScreenPoint ScreenSize;
	bool UseGeometryOutline = false;
	bool UseRasterization = true;
	CullingMode CullMode;

};

void Rasterizer::Rasterize(VertexShader::VertexOutput* PrimitiveData)
{
	if (PrimitiveData == nullptr)
	{
		return;
	}

	if (PrimitiveData[PRIMITIVE_INDEX_ONE].Position.Z > 1.0f)
	{
		return;
	}

	for (int i = 0; i < PRIMITIVE_COUNT; ++i)
	{
		PrimitiveData[i].Position.X *= ScreenSize.Y;
		PrimitiveData[i].Position.Y *= ScreenSize.X;
	}

	switch (CullMode)
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

	SortVertexByY(PrimitiveData);

	// Rasterization 기능이 활성화 되어 있으면 실행.
	if (UseRasterization == true)
	{
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
			NewPosition.X = PrimitiveData[PRIMITIVE_INDEX_TWO].Position.X;
			NewPosition.Y = (PrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y - PrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y) / (PrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y - PrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y)
				* (PrimitiveData[PRIMITIVE_INDEX_THREE].Position.X - PrimitiveData[PRIMITIVE_INDEX_ONE].Position.X) + PrimitiveData[PRIMITIVE_INDEX_ONE].Position.X;
			RasterizeTopFlatPrimitive(PrimitiveData[PRIMITIVE_INDEX_TWO].Position, NewPosition, PrimitiveData[PRIMITIVE_INDEX_THREE].Position);
			RasterizeBottomFlatPrimitive(PrimitiveData[PRIMITIVE_INDEX_ONE].Position, PrimitiveData[PRIMITIVE_INDEX_TWO].Position, NewPosition);
		}
	}

	// GeometryOutline 기능이 활성화 되어 있으면 실행.
	if (UseGeometryOutline == true)
	{
		WriteGeometryOutline(PrimitiveData[PRIMITIVE_INDEX_ONE].Position, PrimitiveData[PRIMITIVE_INDEX_TWO].Position);
		WriteGeometryOutline(PrimitiveData[PRIMITIVE_INDEX_TWO].Position, PrimitiveData[PRIMITIVE_INDEX_THREE].Position);
		WriteGeometryOutline(PrimitiveData[PRIMITIVE_INDEX_THREE].Position, PrimitiveData[PRIMITIVE_INDEX_ONE].Position);
	}
}

void Rasterizer::RasterizeBottomFlatPrimitive(const VertexShader::VertexOutput* InPrimitiveData)
{
	float SlopeL = (InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.X - InPrimitiveData[PRIMITIVE_INDEX_TWO].Position.X) /
		(InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y - InPrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y);
	float SlopeR = (InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.X - InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.X) /
		(InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y - InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y);

	Vector4 StartPoint;
	Vector4 EndPoint;
	StartPoint = EndPoint = InPrimitiveData[PRIMITIVE_INDEX_ONE].Position;

	for (int PosY = InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y; PosY >= InPrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y; --PosY)
	{
		WriteFlatLine(StartPoint, EndPoint);

		StartPoint.X += SlopeL;
		EndPoint.X += SlopeR;
	}
}

void Rasterizer::RasterizeBottomFlatPrimitive(const Vector4& Point1, const Vector4& Point2, const Vector4& Point3)
{
	float SlopeL = (Point1.X - Point2.X) / (Point1.Y - Point2.Y);
	float SlopeR = (Point1.X - Point3.X) / (Point1.Y - Point3.Y);

	Vector4 StartPoint;
	Vector4 EndPoint;
	StartPoint = EndPoint = Point1;

	for (int PosY = Point1.Y; PosY >= Point2.Y; --PosY)
	{
		WriteFlatLine(StartPoint, EndPoint);

		StartPoint.X += SlopeL;
		EndPoint.X += SlopeR;
	}
}

void Rasterizer::RasterizeTopFlatPrimitive(const VertexShader::VertexOutput* InPrimitiveData)
{
	float SlopeL = (InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.X - InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.X) /
		(InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y - InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y);
	float SlopeR = (InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.X - InPrimitiveData[PRIMITIVE_INDEX_TWO].Position.X) /
		(InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y - InPrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y);

	Vector4 StartPoint;
	Vector4 EndPoint;
	StartPoint = EndPoint = InPrimitiveData[PRIMITIVE_INDEX_THREE].Position;

	for (int PosY = InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y; PosY <= InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y; ++PosY)
	{
		WriteFlatLine(StartPoint, EndPoint);

		StartPoint.X += SlopeL;
		EndPoint.X += SlopeR;
	}
}

void Rasterizer::RasterizeTopFlatPrimitive(const Vector4& Point1, const Vector4& Point2, const Vector4& Point3)
{
	float SlopeL = (Point3.X - Point1.X) / (Point3.Y - Point1.Y);
	float SlopeR = (Point3.X - Point2.X) / (Point3.Y - Point2.Y);

	Vector4 StartPoint;
	Vector4 EndPoint;
	StartPoint = EndPoint = Point3;

	for (int PosY = Point3.Y; PosY >= Point1.Y; --PosY)
	{
		WriteFlatLine(StartPoint, EndPoint);

		StartPoint.X += SlopeL;
		EndPoint.X += SlopeR;
	}
}

void Rasterizer::WriteFlatLine(const Vector4& StartPoint, const Vector4& EndPoint)
{
	// Draw FlatLine;
	// Not Implemented;
}

void Rasterizer::WriteGeometryOutline(const Vector4& StartPoint, const Vector4& EndPoint)
{
	ScreenPoint CurrentPoint = ScreenPoint::ToScreenCoordinate(ScreenSize, StartPoint.ToVector2());
	int XMin = 0;
	int XMax = ScreenSize.X;
	int YMin = 0;
	int YMax = ScreenSize.Y;

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
		if (CurrentPoint.X > XMin&& CurrentPoint.X < XMax && CurrentPoint.Y > YMin&& CurrentPoint.Y < YMax)
		{
			RSI->DrawPoint(CurrentPoint, OutlineColor);
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


void Rasterizer::GetBaycentricCoodinate()
{
	// Calculrate BaycentiricCoodinate;
	// Not Implemented;
}

void Rasterizer::GetInterpolratedPrimitive()
{
	// return InterpolratedPrimitive;
	// Not Implemented;
}

void Rasterizer::SortVertexByY(VertexShader::VertexOutput* PrimitiveData)
{
	if ((PrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y >= PrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y) &&
		(PrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y >= PrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y))
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

inline bool Rasterizer::IsFrontNormal(VertexShader::VertexOutput* PrimitiveData)
{
	// 2D Vector에 대해 외적을 하여 Z값을 얻음
	Vector2 PrimitiveVector1 = PrimitiveData[PRIMITIVE_INDEX_THREE].Position.ToVector2() - PrimitiveData[PRIMITIVE_INDEX_TWO].Position.ToVector2();
	Vector2 PrimitiveVector2 = PrimitiveData[PRIMITIVE_INDEX_THREE].Position.ToVector2() - PrimitiveData[PRIMITIVE_INDEX_ONE].Position.ToVector2();
	float PrimitiveZ = (PrimitiveVector1.X * PrimitiveVector2.Y) - (PrimitiveVector1.Y * PrimitiveVector2.X);

	return PrimitiveZ > 0;
}

void Rasterizer::SetFragmentShaderFunction(void(FragmentShader::* InFragmentShaderFunc)(FragmentShader::FragmentInput*, UINT))
{
	FragmentShaderFunc = InFragmentShaderFunc;
}

void Rasterizer::SetRasterizerState(const ScreenPoint& InScreenSize, bool InUseGeometryOutline, bool InUseRasterization, CullingMode InCullMode)
{
	ScreenSize = InScreenSize;
	UseGeometryOutline = InUseGeometryOutline;
	UseRasterization = InUseRasterization;
	CullMode = InCullMode;
}

