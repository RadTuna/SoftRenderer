
#include "Precompiled.h"
#include "..\Public\RenderContext.h"

bool RenderContext::Initialize()
{
	mInputAssembler = std::make_unique<InputAssembler>();
	if (mInputAssembler == nullptr)
	{
		return false;
	}

	mVertexShader = std::make_unique<VertexShader>();
	if (mVertexShader == nullptr)
	{
		return false;
	}

	mRasterizer = std::make_unique<Rasterizer>();
	if (mRasterizer == nullptr)
	{
		return false;
	}

	mFragmentShader = std::make_unique<FragmentShader>();
	if (mFragmentShader == nullptr)
	{
		return false;
	}

	mOutputMerger = std::make_unique<OutputMerger>();
	if (mOutputMerger == nullptr)
	{
		return false;
	}

	return true;
}

void RenderContext::DrawCall()
{
	VertexShader::FragmentInput OutPrimitiveData[PRIMITIVE_COUNT];

	UINT NumIndices = mInputAssembler->GetIndexBuffer()->DataSize / mInputAssembler->GetIndexStride();
	for (UINT IndexOffset = 0; IndexOffset < NumIndices; IndexOffset += PRIMITIVE_COUNT)
	{
		mVertexShader->ProcessVertexShader(
			mInputAssembler->GetVertexBuffer(),
			mInputAssembler->GetIndexBuffer(),
			IndexOffset,
			OutPrimitiveData);


	}

}

void RenderContext::IASetVertexBuffer(VertexBuffer* Buffer, UINT Stride)
{
	mInputAssembler->SetVertexBuffer(Buffer, Stride);
}

void RenderContext::IASetIndexBuffer(IndexBuffer* Buffer, UINT Stride)
{
	mInputAssembler->SetIndexBuffer(Buffer, Stride);
}