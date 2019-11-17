
#include "Precompiled.h"
#include "..\Public\RenderContext.h"
#include "WindowsRSI.h"


RenderContext::RenderContext()
{
	#ifdef _WINDOWS_RENDERING_INTERFACE
	mRSI = std::make_shared<WindowsRSI>();
	#endif
	assert(mRSI);

	mInputAssembler = std::make_unique<InputAssembler>();
	assert(mInputAssembler);

	mVertexShader = std::make_unique<VertexShader>();
	assert(mVertexShader);

	mRasterizer = std::make_unique<Rasterizer>(mRSI);
	assert(mRasterizer);
}

bool RenderContext::Initialize(const ScreenPoint& InScreenSize)
{
	bool result = false;

	mScreenSize = InScreenSize;

	mFragmentShader = std::make_shared<FragmentShader>(mRSI, mScreenSize);
	assert(mFragmentShader);

	result = mRSI->Init(mScreenSize);
	if (result == false)
	{
		return false;
	}

	return true;
}

void RenderContext::Shutdown()
{
	mRSI->Shutdown();
}

