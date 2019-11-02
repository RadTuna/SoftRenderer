#pragma once

#include "Vector4.h"
#include "RSIDataTypes.h"


class VertexShader final
{
public:

	struct VertexInput
	{
		Vector4 Position;
	};

	struct FragmentInput
	{
		Vector4 Position;
	};

public:

	VertexShader() = default;
	~VertexShader() = default;

	FORCEINLINE void ProcessVertexShader(VertexBuffer* InVertexBuffer, IndexBuffer* InIndexBuffer, UINT IndexOffset, void* OutputData);

private:

	FORCEINLINE FragmentInput VertexMain(VertexInput InputData);

private:
};

