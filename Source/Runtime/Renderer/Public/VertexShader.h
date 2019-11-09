#pragma once

#include "Vector4.h"
#include "RSIDataTypes.h"


class VertexShader final
{
public:

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

	Output.Position = InputData.Position;

	return Output;
}

