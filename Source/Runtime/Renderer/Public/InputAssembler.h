#pragma once

#include "RSIDataTypes.h"

class InputAssembler final
{
public:

	InputAssembler() = default;
	~InputAssembler();

	FORCEINLINE void SetVertexBuffer(VertexBuffer* InBuffer, UINT InStride);
	FORCEINLINE void SetIndexBuffer(IndexBuffer* InBuffer, UINT InStride);

private:

	FORCEINLINE void ClearBuffers();

private:

	VertexBuffer* mVertexBuffer;
	UINT mVertexStride;
	IndexBuffer* mIndexBuffer;
	UINT mIndexStride;

public:

	FORCEINLINE VertexBuffer* GetVertexBuffer() const { return mVertexBuffer; }
	FORCEINLINE UINT GetVertexStride() const { return mVertexStride; }
	FORCEINLINE IndexBuffer* GetIndexBuffer() const { return mIndexBuffer; }
	FORCEINLINE UINT GetIndexStride() const { return mIndexStride; }

};
