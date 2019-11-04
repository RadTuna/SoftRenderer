
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
	VertexShader::VertexOutput OutPrimitiveData[PRIMITIVE_COUNT];

	UINT NumIndices = mInputAssembler->GetIndexBuffer()->DataSize / mInputAssembler->GetIndexStride();
	for (UINT IndexOffset = 0; IndexOffset < NumIndices; IndexOffset += PRIMITIVE_COUNT)
	{
		// 버텍스 쉐이더를 호출해 InputAssembler의 값을 연산.
		// 이후, 연산이 끝난 값을 PRIMITIVE단위로 반환.
		mVertexShader->ProcessVertexShader(
			mInputAssembler->GetVertexBuffer(),
			mInputAssembler->GetIndexBuffer(),
			IndexOffset,
			OutPrimitiveData);

		mRasterizer->SetFragmentShaderFunction(mFragmentShader->ProcessFragmentShader);

		mRasterizer->Rasterize(OutPrimitiveData);
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