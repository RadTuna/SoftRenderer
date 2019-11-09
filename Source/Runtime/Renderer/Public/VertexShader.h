#pragma once

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
	};

	// Structure의 이름은 고정.
	struct VertexOutput
	{
		Vector4 Position;
	};

	MatrixBuffer VertexShaderMatrix;

public:

	VertexShader() = default;
	~VertexShader() = default;

	FORCEINLINE void ProcessVertexShader(VertexBuffer* InVertexBuffer, IndexBuffer* InIndexBuffer, UINT IndexOffset, VertexOutput* OutputData);

private:

	FORCEINLINE VertexOutput VertexMain(VertexInput InputData);

};


void VertexShader::ProcessVertexShader(VertexBuffer* InVertexBuffer, IndexBuffer* InIndexBuffer, UINT IndexOffset, VertexOutput* OutputData)
{
	VertexInput* VertexInputData = reinterpret_cast<VertexInput*>(InVertexBuffer->Data);
	UINT* IndexInputData = InIndexBuffer->Data;
	if (VertexInputData == nullptr || IndexInputData == nullptr)
	{
		return;
	}

	VertexOutput* Output = reinterpret_cast<VertexOutput*>(OutputData);

	for (UINT i = 0; i < PRIMITIVE_COUNT; ++i)
	{
		Output[i] = VertexMain(VertexInputData[IndexInputData[i + IndexOffset]]);
	}
}

VertexShader::VertexOutput VertexShader::VertexMain(VertexInput InputData)
{
	VertexOutput Output;

	InputData.Position.W = 1.0f;

	Output.Position = VertexShaderMatrix.WorldMatrix * InputData.Position;
	Output.Position = VertexShaderMatrix.ViewMatrix * Output.Position;
	Output.Position = VertexShaderMatrix.ProjectionMatrix * Output.Position;

	Output.Position.X /= Output.Position.W;
	Output.Position.Y /= Output.Position.W;
	Output.Position.Z /= Output.Position.W;

	return Output;
}

