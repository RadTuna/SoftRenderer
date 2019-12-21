#pragma once

#include <memory>
#include <cassert>

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

	RenderContext();
	~RenderContext() = default;

	bool Initialize(const ScreenPoint& InScreenSize);
	void Shutdown();

	FORCEINLINE void DrawCall();
	FORCEINLINE void Represent();
	FORCEINLINE void ClearBackbuffer(const LinearColor& ClearColor);

	FORCEINLINE void RSSetRasterizeState(bool UseOutline, bool UseRasterization, CullingMode CullMode);
	FORCEINLINE void IASetVertexBuffer(VertexBuffer* Buffer, UINT Stride);
	FORCEINLINE void IASetIndexBuffer(IndexBuffer* Buffer, UINT Stride);
	FORCEINLINE void VSSetMatrixBuffer(void* Buffer);
	FORCEINLINE void FSSetDirectionalLightBuffer(void* Buffer);
	FORCEINLINE void FSSetTextureBuffer(void* Buffer);

	FORCEINLINE void DrawGrid2D();

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

FORCEINLINE void RenderContext::Represent()
{
	// Buffer Swap;
	mRSI->BufferSwap();
}

FORCEINLINE void RenderContext::ClearBackbuffer(const LinearColor& ClearColor)
{
	// Clear Buffer;
	mRSI->Clear(ClearColor);
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
	assert(Buffer);

	VertexShader::MatrixBuffer* pMatrixBuffer = reinterpret_cast<VertexShader::MatrixBuffer*>(Buffer);
	assert(pMatrixBuffer);

	mVertexShader->mVertexShaderMatrix.WorldMatrix = pMatrixBuffer->WorldMatrix;
	mVertexShader->mVertexShaderMatrix.ViewMatrix = pMatrixBuffer->ViewMatrix;
	mVertexShader->mVertexShaderMatrix.ProjectionMatrix = pMatrixBuffer->ProjectionMatrix;
}

FORCEINLINE void RenderContext::FSSetDirectionalLightBuffer(void* Buffer)
{
	assert(Buffer);

	FragmentShader::DirectionalLightBuffer* pDirectionalLightBuffer = reinterpret_cast<FragmentShader::DirectionalLightBuffer*>(Buffer);
	assert(pDirectionalLightBuffer);

	mFragmentShader->mDirectionalLightBuffer.LightColor = pDirectionalLightBuffer->LightColor;
	mFragmentShader->mDirectionalLightBuffer.LightDirection = pDirectionalLightBuffer->LightDirection;
	mFragmentShader->mDirectionalLightBuffer.LightIntensity = pDirectionalLightBuffer->LightIntensity;
}

FORCEINLINE void RenderContext::FSSetTextureBuffer(void* Buffer)
{
	assert(Buffer);

	FragmentShader::TextureBuffer* pTextureBuffer = reinterpret_cast<FragmentShader::TextureBuffer*>(Buffer);
	assert(pTextureBuffer);

	mFragmentShader->mTextureBuffer.AlbedoMap = pTextureBuffer->AlbedoMap;
}


FORCEINLINE void RenderContext::DrawGrid2D()
{
	const static int Grid2DUnit = 10;

	// Colors to use
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// Draw Grid Line
	ScreenPoint screenHalfSize = mScreenSize.GetHalf();

	for (int x = screenHalfSize.X; x <= mScreenSize.X; x += Grid2DUnit)
	{
		mRSI->DrawFullVerticalLine(x, gridColor);
		if (x > screenHalfSize.X)
		{
			mRSI->DrawFullVerticalLine(2 * screenHalfSize.X - x, gridColor);
		}
	}

	for (int y = screenHalfSize.Y; y <= mScreenSize.Y; y += Grid2DUnit)
	{
		mRSI->DrawFullHorizontalLine(y, gridColor);
		if (y > screenHalfSize.Y)
		{
			mRSI->DrawFullHorizontalLine(2 * screenHalfSize.Y - y, gridColor);
		}
	}

	// Draw World Axis
	mRSI->DrawFullHorizontalLine(screenHalfSize.Y, LinearColor::Red);
	mRSI->DrawFullVerticalLine(screenHalfSize.X, LinearColor::Green);
}

