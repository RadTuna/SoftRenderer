
#include "Precompiled.h"
#include "..\Public\VertexShader.h"

void VertexShader::ProcessVertexShader(VertexBuffer* InVertexBuffer, IndexBuffer* InIndexBuffer, UINT IndexOffset, void* OutputData)
{
	VertexInput* VertexInputData = reinterpret_cast<VertexInput*>(InVertexBuffer->Data);
	UINT* IndexInputData = InIndexBuffer->Data;
	if (VertexInputData == nullptr || IndexInputData == nullptr)
	{
		return;
	}

	FragmentInput* Output = reinterpret_cast<FragmentInput*>(OutputData);

	for (UINT i = 0; i < PRIMITIVE_COUNT; ++i)
	{
		Output[i] = VertexMain(VertexInputData[IndexInputData[i + IndexOffset]]);
	}
}

VertexShader::FragmentInput VertexShader::VertexMain(VertexInput InputData)
{
	FragmentInput Output;

	Output.Position = InputData.Position;

	return Output;
}

