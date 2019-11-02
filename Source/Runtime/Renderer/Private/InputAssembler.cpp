
#include "Precompiled.h"
#include "..\Public\InputAssembler.h"

InputAssembler::~InputAssembler()
{
	ClearBuffers();
}

void InputAssembler::SetVertexBuffer(VertexBuffer* InBuffer, UINT InStride)
{
	ClearBuffers();

	mVertexBuffer = InBuffer;
	mVertexStride = InStride;
}

void InputAssembler::SetIndexBuffer(IndexBuffer* InBuffer, UINT InStride)
{
	ClearBuffers();

	mIndexBuffer = InBuffer;
	mIndexStride = InStride;
}

void InputAssembler::ClearBuffers()
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

	mVertexStride = 0;
	mIndexStride = 0;
}
