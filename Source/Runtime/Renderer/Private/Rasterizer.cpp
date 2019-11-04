
#include "Precompiled.h"
#include "..\Public\Rasterizer.h"

void Rasterizer::Rasterize(const VertexShader::VertexOutput* PrimitiveData)
{
	if (InPrimitiveData == nullptr)
	{
		return;
	}

	SortVertexByY(PrimitiveData);
	
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

void Rasterizer::RasterizeBottomFlatPrimitive(const VertexShader::VertexOutput* InPrimitiveData)
{
	float SlopeL = (InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.X - InPrimitiveData[PRIMITIVE_INDEX_TWO].Position.X) / 
		(InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y - InPrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y);
	float SlopeR = (InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.X - InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.X) / 
		(InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y - InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y);

	Vector4 StartPoint;
	Vector4 EndPoint;
	StartPoint = EndPoint = InPrimitiveData[PRIMITIVE_INDEX_ONE].Position;

	for (UINT PosY = InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y; PosY >= InPrimitiveData[PRIMITIVE_INDEX_TWO].Position.Y; --PosY)
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

	for (UINT PosY = Point1.Y; PosY >= Point2.Y; --PosY)
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

	for (UINT PosY = InPrimitiveData[PRIMITIVE_INDEX_THREE].Position.Y; PosY <= InPrimitiveData[PRIMITIVE_INDEX_ONE].Position.Y; ++PosY)
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

	for (UINT PosY = Point3.Y; PosY >= Point1.Y; --PosY)
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

void Rasterizer::SetFragmentShaderFunction(void(*InFragmentShaderFunc)(FragmentShader::FragmentInput*, UINT))
{
	FragmentShaderFunc = InFragmentShaderFunc;
}
