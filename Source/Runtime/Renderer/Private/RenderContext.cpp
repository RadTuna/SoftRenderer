
#include "Precompiled.h"
#include "..\Public\RenderContext.h"

bool RenderContext::Initialize(const std::shared_ptr<RenderingSoftwareInterface>& InRSI, const ScreenPoint& InScreenSize)
{
	mScreenSize = InScreenSize;
	mRSI = InRSI;

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

	mRasterizer = std::make_unique<Rasterizer>(mRSI);
	if (mRasterizer == nullptr)
	{
		return false;
	}

	mFragmentShader = std::make_shared<FragmentShader>(mRSI, mScreenSize);
	if (mFragmentShader == nullptr)
	{
		return false;
	}

	return true;
}

