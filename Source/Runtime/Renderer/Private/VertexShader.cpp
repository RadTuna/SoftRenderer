
#include "Precompiled.h"
#include "..\Public\VertexShader.h"

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

