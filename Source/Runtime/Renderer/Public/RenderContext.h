#pragma once

#include <memory>
#include "InputAssembler.h"
#include "VertexShader.h"
#include "Rasterizer.h"
#include "FragmentShader.h"
#include "RSIDataTypes.h"
#include "RenderingSoftwareInterface.h"
#include "..\Public\ScreenPoint.h"

class RenderContext final
{
public:

	RenderContext() = default;
	~RenderContext() = default;

	bool Initialize(const std::shared_ptr<RenderingSoftwareInterface>& InRSI, const ScreenPoint& InScreenSize);
	FORCEINLINE void DrawCall();
	FORCEINLINE void RSSetRasterizeState(bool UseOutline, bool UseRasterization, CullingMode CullMode);
	FORCEINLINE void IASetVertexBuffer(VertexBuffer* Buffer, UINT Stride);
	FORCEINLINE void IASetIndexBuffer(IndexBuffer* Buffer, UINT Stride);
	FORCEINLINE void VSSetMatrixBuffer(void* Buffer);

private:

private:

	std::unique_ptr<InputAssembler> mInputAssembler;
	std::unique_ptr<VertexShader> mVertexShader;
	std::unique_ptr<Rasterizer> mRasterizer;
	std::shared_ptr<FragmentShader> mFragmentShader;

	ScreenPoint mScreenSize;
	std::shared_ptr<RenderingSoftwareInterface> mRSI;

};

FORCEINLINE void RenderContext::DrawCall()
{
	VertexShader::VertexOutput OutPrimitiveData[PRIMITIVE_COUNT];

	mRasterizer->SetFragmentShader(mFragmentShader);

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

		mRasterizer->Rasterize(OutPrimitiveData);
	}

}

FORCEINLINE void RenderContext::RSSetRasterizeState(bool UseOutline, bool UseRasterization, CullingMode CullMode)
{
	mRasterizer->SetRasterizerState(mScreenSize, UseOutline, UseRasterization, CullMode);
}

FORCEINLINE void RenderContext::IASetVertexBuffer(VertexBuffer* Buffer, UINT Stride)
{
	mInputAssembler->SetVertexBuffer(Buffer, Stride);
}

FORCEINLINE void RenderContext::IASetIndexBuffer(IndexBuffer* Buffer, UINT Stride)
{
	mInputAssembler->SetIndexBuffer(Buffer, Stride);
}

FORCEINLINE void RenderContext::VSSetMatrixBuffer(void* Buffer)
{
	if (Buffer == nullptr)
	{
		return;
	}

	VertexShader::MatrixBuffer* pMatrixBuffer = reinterpret_cast<VertexShader::MatrixBuffer*>(Buffer);

	mVertexShader->VertexShaderMatrix.WorldMatrix = pMatrixBuffer->WorldMatrix;
	mVertexShader->VertexShaderMatrix.ViewMatrix = pMatrixBuffer->ViewMatrix;
	mVertexShader->VertexShaderMatrix.ProjectionMatrix = pMatrixBuffer->ProjectionMatrix;
}

