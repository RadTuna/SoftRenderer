
#include "Precompiled.h"
#include "SoftRenderer.h"
#include "RSIDataTypes.h"

SoftRenderer::SoftRenderer(RenderingSoftwareInterface* InRSI) : RSI(InRSI)
{
	RendererContext = std::make_unique<RenderContext>();
	RendererFactory = std::make_unique<RenderFactory>();

	CameraLocation = Vector4(0.0f, 0.0f, -200.0f, 0.0f);
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
		if (!RSI->Init(CurrentScreenSize))
		{
			return;
		}
		if (RendererContext->Initialize(RSI, CurrentScreenSize) == false)
		{
			return;
		}

		IsRendererInitialized = true;

		// Check Input Binding
		if (InputManager.GetXAxis && InputManager.GetYAxis && InputManager.MoveForward && InputManager.MoveRight && InputManager.MoveUp)
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
		RSI->Init(InNewScreenSize);
	}
}

void SoftRenderer::Shutdown()
{
	RSI->Shutdown();
}

void SoftRenderer::CalculrateOrthographicMatrix(Matrix4x4& OrthographicMatrix)
{
}

