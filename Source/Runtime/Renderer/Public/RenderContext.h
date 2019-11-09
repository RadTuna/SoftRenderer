#pragma once

#include <memory>
#include "InputAssembler.h"
#include "VertexShader.h"
#include "Rasterizer.h"
#include "FragmentShader.h"
#include "OutputMerger.h"
#include "RSIDataTypes.h"
#include "RenderingSoftwareInterface.h"
#include "..\Public\ScreenPoint.h"

class RenderContext final
{
public:

	RenderContext() = default;
	~RenderContext() = default;

	bool Initialize(RenderingSoftwareInterface* InRSI, const ScreenPoint& InScreenSize);
	FORCEINLINE void DrawCall();
	FORCEINLINE void RSSetRasterizeState(bool UseOutline, bool UseRasterization);
	FORCEINLINE void IASetVertexBuffer(VertexBuffer* Buffer, UINT Stride);
	FORCEINLINE void IASetIndexBuffer(IndexBuffer* Buffer, UINT Stride);

private:

private:

	std::unique_ptr<InputAssembler> mInputAssembler;
	std::unique_ptr<VertexShader> mVertexShader;
	std::unique_ptr<Rasterizer> mRasterizer;
	std::unique_ptr<FragmentShader> mFragmentShader;
	std::unique_ptr<OutputMerger> mOutputMerger;

	ScreenPoint ScreenSize;
	RenderingSoftwareInterface* RSI;

};

void RenderContext::DrawCall()
{
	VertexShader::VertexOutput OutPrimitiveData[PRIMITIVE_COUNT];

	void(FragmentShader:: * InFragmentShaderFunc)(FragmentShader::FragmentInput*, UINT) = nullptr;
	mFragmentShader->GetProcessFragmentShader(&InFragmentShaderFunc);
	mRasterizer->SetFragmentShaderFunction(InFragmentShaderFunc);

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

void RenderContext::RSSetRasterizeState(bool UseOutline, bool UseRasterization)
{
	mRasterizer->SetRasterizerState(ScreenSize, UseOutline, UseRasterization);
}

void RenderContext::IASetVertexBuffer(VertexBuffer* Buffer, UINT Stride)
{
	mInputAssembler->SetVertexBuffer(Buffer, Stride);
}

void RenderContext::IASetIndexBuffer(IndexBuffer* Buffer, UINT Stride)
{
	mInputAssembler->SetIndexBuffer(Buffer, Stride);
}

