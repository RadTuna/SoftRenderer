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

private:
};

