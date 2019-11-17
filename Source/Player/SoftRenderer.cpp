
#include "Precompiled.h"
#include "SoftRenderer.h"
#include "RSIDataTypes.h"

SoftRenderer::SoftRenderer()
{
	mRendererContext = std::make_unique<RenderContext>();
	mRendererFactory = std::make_unique<RenderFactory>();

	mCameraLocation = Vector4(0.0f, 0.0f, -200.0f, 0.0f);
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
			IsInputInitialized = true;
		}
		else
		{
			return;
		}

		IsAllInitialized = IsRendererInitialized && IsPerformanceCheckInitialized && IsInputInitialized;
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

void SoftRenderer::CalculrateOrthographicMatrix(Matrix4x4& OrthographicMatrix)
{
}

