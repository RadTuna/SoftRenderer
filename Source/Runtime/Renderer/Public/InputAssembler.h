#pragma once

#include "RSIDataTypes.h"

class InputAssembler final
{
public:

	InputAssembler() = default;
	~InputAssembler() { ClearBuffers(); }

	FORCEINLINE void SetVertexBuffer(VertexBuffer* InBuffer, UINT InStride);
	FORCEINLINE void SetIndexBuffer(IndexBuffer* InBuffer, UINT InStride);

private:

	FORCEINLINE void ClearVertexBuffer();
	FORCEINLINE void ClearIndexBuffer();
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

void InputAssembler::SetVertexBuffer(VertexBuffer* InBuffer, UINT InStride)
{
	ClearVertexBuffer();

	mVertexBuffer = InBuffer;
	mVertexStride = InStride;
}

void InputAssembler::SetIndexBuffer(IndexBuffer* InBuffer, UINT InStride)
{
	ClearIndexBuffer();

	mIndexBuffer = InBuffer;
	mIndexStride = InStride;
}

FORCEINLINE void InputAssembler::ClearVertexBuffer()
{
	if (mVertexBuffer != nullptr)
	{
		if (mVertexBuffer->Data != nullptr)
		{
			delete[] mVertexBuffer->Data;
			mVertexBuffer->Data = nullptr;
		}

		delete mVertexBuffer;
		mVertexBuffer = nullptr;
	}
	mVertexStride = 0;
}

FORCEINLINE void InputAssembler::ClearIndexBuffer()
{
	if (mIndexBuffer != nullptr)
	{
		if (mIndexBuffer->Data != nullptr)
		{
			delete[] mIndexBuffer->Data;
			mIndexBuffer->Data = nullptr;
		}

		delete mIndexBuffer;
		mIndexBuffer = nullptr;
	}
	mIndexStride = 0;
}

FORCEINLINE void InputAssembler::ClearBuffers()
{
	ClearVertexBuffer();
	ClearIndexBuffer();
}
