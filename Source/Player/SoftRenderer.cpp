
#include "Precompiled.h"
#include "SoftRenderer.h"
#include "RSIDataTypes.h"
#include "InputManagerProxy.h"


namespace InputManagerProxy
{
	std::function<float()> GetXAxis;
	std::function<float()> GetYAxis;
	std::function<float()> MoveForward;
	std::function<float()> MoveRight;
	std::function<float()> MoveUp;
	std::function<bool()> MouseLBDown;
	std::function<bool()> MouseLB;
	std::function<bool()> MouseRBDown;
	std::function<bool()> MouseRB;
}

SoftRenderer::SoftRenderer()
{
	mRendererContext = std::make_shared<RenderContext>();
	mRendererFactory = std::make_shared<RenderFactory>();
}

void SoftRenderer::OnTick()
{
	// 별도로 Initialize함수호출을 하지 않기 위해 아래와 같이함.
	if (!IsAllInitialized)
	{
		// Init Query Performance Timer
		if(PerformanceInitFunc && PerformanceMeasureFunc)
		{
			CyclesPerMilliSeconds = PerformanceInitFunc();
			IsPerformanceCheckInitialized = true;
		}
		else
		{
			return;
		}

		// Init RSI
		if (CurrentScreenSize.HasZero())
		{
			return;
		}
		if (mRendererContext->Initialize(CurrentScreenSize) == false)
		{
			return;
		}

		IsRendererInitialized = true;

		// Check Input Binding
		if (mInputManager.GetXAxis && mInputManager.GetYAxis && mInputManager.MoveForward && mInputManager.MoveRight && mInputManager.MoveUp)
		{
			InputManagerProxy::GetXAxis = mInputManager.GetXAxis;
			InputManagerProxy::GetYAxis = mInputManager.GetYAxis;
			InputManagerProxy::MoveForward = mInputManager.MoveForward;
			InputManagerProxy::MoveRight = mInputManager.MoveRight;
			InputManagerProxy::MoveUp = mInputManager.MoveUp;
			InputManagerProxy::MouseLBDown = mInputManager.MouseLBDown;
			InputManagerProxy::MouseLB = mInputManager.MouseLB;
			InputManagerProxy::MouseRBDown = mInputManager.MouseRBDown;
			InputManagerProxy::MouseRB = mInputManager.MouseRB;

			IsInputInitialized = true;
		}
		else
		{
			return;
		}

		IsAllInitialized = IsRendererInitialized && IsPerformanceCheckInitialized && IsInputInitialized;
		Awake();
	}
	else
	{
		PreUpdate();
		Update();
		PostUpdate();
	}
}

void SoftRenderer::OnResize(const ScreenPoint& InNewScreenSize)
{
	CurrentScreenSize = InNewScreenSize;

	if (IsRendererInitialized)
	{
		mRendererContext->Initialize(InNewScreenSize);
	}
}

void SoftRenderer::Shutdown()
{
	mRendererContext->Shutdown();
}

