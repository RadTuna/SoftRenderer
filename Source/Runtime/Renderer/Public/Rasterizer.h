#pragma once

#include "RSIDataTypes.h"
#include "..\Public\VertexShader.h"
#include "..\Public\FragmentShader.h"

class Rasterizer final
{
public:

	Rasterizer() = default;
	~Rasterizer() = default;

	FORCEINLINE void Rasterize(const VertexShader::VertexOutput* PrimitiveData);

	FORCEINLINE void RasterizeBottomFlatPrimitive(const VertexShader::VertexOutput* InPrimitiveData);
	FORCEINLINE void RasterizeBottomFlatPrimitive(const Vector4& Point1, const Vector4& Point2, const Vector4& Point3);
	FORCEINLINE void RasterizeTopFlatPrimitive(const VertexShader::VertexOutput* InPrimitiveData);
	FORCEINLINE void RasterizeTopFlatPrimitive(const Vector4& Point1, const Vector4& Point2, const Vector4& Point3);

	FORCEINLINE void WriteFlatLine(const Vector4& StartPoint, const Vector4& EndPoint); // WIP

	FORCEINLINE void GetBaycentricCoodinate(); // WIP
	FORCEINLINE void GetInterpolratedPrimitive(); // WIP
	FORCEINLINE void SortVertexByY(VertexShader::VertexOutput* PrimitiveData);

	FORCEINLINE void SetFragmentShaderFunction(void(*InFragmentShaderFunc)(FragmentShader::FragmentInput*, UINT));

private:

	// FragmentShader ProcessFunction;
	void(*FragmentShaderFunc)(FragmentShader::FragmentInput*, UINT);

};
