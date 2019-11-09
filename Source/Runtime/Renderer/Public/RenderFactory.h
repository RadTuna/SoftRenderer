#pragma once

#include "RSIDataTypes.h"

class RenderFactory final
{
public:

	RenderFactory() = default;
	~RenderFactory() = default;

	bool CreateVertexBuffer(UINT DataSize, void* InData, VertexBuffer** OutBuffer);
	bool CreateIndexBuffer(UINT DataSize, void* InData, IndexBuffer** OutBuffer);

private:
};
