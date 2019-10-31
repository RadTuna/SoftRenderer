
#include "Precompiled.h"
#include "SoftRenderer.h"

SoftRenderer::SoftRenderer(RenderingSoftwareInterface* InRSI) : RSI(InRSI)
{
}

void SoftRenderer::OnTick()
{
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

		IsRendererInitialized = true;

		// Check Input Binding
		if (InputManager.GetXAxis && InputManager.GetYAxis && InputManager.SpacePressed)
		{
			IsInputInitialized = true;
		}
		else
		{
			return;
		}

		IsAllInitialized = IsRendererInitialized && IsPerformanceCheckInitialized && IsInputInitialized;
		if (IsAllInitialized)
		{
			BindImplClass();
		}
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

	if (IsImplBinded)
	{
		Impl2D.reset();
		Impl2D = std::make_unique<SoftRendererImpl2D>(this);
	}
}

void SoftRenderer::Shutdown()
{
	RSI->Shutdown();
}

void SoftRenderer::PreUpdate()
{
	FrameTimeStamp = PerformanceMeasureFunc();
	if (FrameCount == 0)
	{
		StartTimeStamp = FrameTimeStamp;
	}

	// Clear Background
	RSI->Clear(LinearColor::White);
}

void SoftRenderer::PostUpdate()
{
	// Unload Level
	RenderFrame();

	// Render Finish
	RSI->EndFrame();

	// Measure Performance
	FrameCount++;
	long long currentTimeStamp = PerformanceMeasureFunc();
	LONGLONG frameCycles = currentTimeStamp - FrameTimeStamp;
	LONGLONG elapsedCycles = currentTimeStamp - StartTimeStamp;
	FrameTime = frameCycles / CyclesPerMilliSeconds;
	ElapsedTime = elapsedCycles / CyclesPerMilliSeconds;
	FrameFPS = FrameTime == 0.f ? 0.f : 1000.f / FrameTime;
	AverageFPS = ElapsedTime == 0.f ? 0.f : 1000.f / ElapsedTime * FrameCount;
}

void SoftRenderer::RenderFrame()
{
	if (IsImplBinded)
	{
		RenderFrameFunc();
	}
}

void SoftRenderer::Update()
{
	if (IsImplBinded)
	{
		UpdateFunc(FrameTime / 1000.f);
	}
}

void SoftRenderer::BindImplClass()
{
	Impl2D = std::make_unique<SoftRendererImpl2D>(this);
	if (nullptr != Impl2D)
	{
		using namespace std::placeholders;
		RenderFrameFunc = std::bind(&SoftRendererImpl2D::RenderFrameImpl, Impl2D.get());
		UpdateFunc = std::bind(&SoftRendererImpl2D::UpdateImpl, Impl2D.get(), _1);
		IsImplBinded = true;
	}
}

