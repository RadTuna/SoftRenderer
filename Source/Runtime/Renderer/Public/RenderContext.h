#pragma once

#include <memory>
#include "InputAssembler.h"
#include "VertexShader.h"
#include "Rasterizer.h"
#include "FragmentShader.h"
#include "OutputMerger.h"
#include "RSIDataTypes.h"

class RenderContext final
{
public:

	RenderContext() = default;
	~RenderContext() = default;

	bool Initialize();
	FORCEINLINE void DrawCall();
	FORCEINLINE void IASetVertexBuffer(VertexBuffer* Buffer, UINT Stride);
	FORCEINLINE void IASetIndexBuffer(IndexBuffer* Buffer, UINT Stride);

private:

private:

	std::unique_ptr<InputAssembler> mInputAssembler;
	std::unique_ptr<VertexShader> mVertexShader;
	std::unique_ptr<Rasterizer> mRasterizer;
	std::unique_ptr<FragmentShader> mFragmentShader;
	std::unique_ptr<OutputMerger> mOutputMerger;

};

