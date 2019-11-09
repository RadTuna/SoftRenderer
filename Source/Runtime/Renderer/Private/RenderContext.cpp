
#include "Precompiled.h"
#include "..\Public\RenderContext.h"

bool RenderContext::Initialize(RenderingSoftwareInterface* InRSI, const ScreenPoint& InScreenSize)
{
	ScreenSize = InScreenSize;
	RSI = InRSI;

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

	mRasterizer = std::make_unique<Rasterizer>(RSI);
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

