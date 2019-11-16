#pragma once

#include <cassert>

#include "Vector4.h"
#include "RSIDataTypes.h"


class VertexShader final
{
public:

	struct MatrixBuffer
	{
		Matrix4x4 WorldMatrix;
		Matrix4x4 ViewMatrix;
		Matrix4x4 ProjectionMatrix;
	};

	// Structure의 이름은 고정.
	struct VertexInput
	{
		Vector4 Position;
		Vector3 Normal;
	};

	// Structure의 이름은 고정.
	struct VertexOutput
	{
		Vector4 Position;
		Vector3 WorldPosition;
		Vector3 WorldNormal;
	};

	MatrixBuffer VertexShaderMatrix;

public:

	VertexShader() = default;
	~VertexShader() = default;

	FORCEINLINE void ProcessVertexShader(VertexBuffer* InVertexBuffer, IndexBuffer* InIndexBuffer, UINT IndexOffset, VertexOutput* OutputData);

private:

	FORCEINLINE VertexOutput VertexMain(VertexInput InputData);

};


FORCEINLINE void VertexShader::ProcessVertexShader(VertexBuffer* InVertexBuffer, IndexBuffer* InIndexBuffer, UINT IndexOffset, VertexOutput* OutputData)
{
	VertexInput* VertexInputData = reinterpret_cast<VertexInput*>(InVertexBuffer->Data);
	UINT* IndexInputData = InIndexBuffer->Data;

	assert(VertexInputData);
	assert(IndexInputData);

	VertexOutput* Output = reinterpret_cast<VertexOutput*>(OutputData);

	assert(Output);

	for (UINT i = 0; i < PRIMITIVE_COUNT; ++i)
	{
		Output[i] = VertexMain(VertexInputData[IndexInputData[i + IndexOffset]]);
	}
}

FORCEINLINE VertexShader::VertexOutput VertexShader::VertexMain(VertexInput InputData)
{
	VertexOutput Output;

	InputData.Position.W = 1.0f;

	Output.Position = VertexShaderMatrix.WorldMatrix * InputData.Position;

	Output.WorldPosition = Output.Position.ToVector3();
	Output.WorldNormal = (VertexShaderMatrix.WorldMatrix * InputData.Normal).Normalize();

	Output.Position = VertexShaderMatrix.ViewMatrix * Output.Position;
	Output.Position = VertexShaderMatrix.ProjectionMatrix * Output.Position;

	Output.Position.X /= Output.Position.W;
	Output.Position.Y /= Output.Position.W;
	Output.Position.Z /= Output.Position.W;

	return Output;
}

