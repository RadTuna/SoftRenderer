#pragma once

#include <memory>
#include <functional>
#include "InputManager.h" 
#include "SoftRendererImpl2D.h"

class SoftRenderer
{
public:
	SoftRenderer(RenderingSoftwareInterface* InRSI);

public:
	const ScreenPoint& GetScreenSize() { return CurrentScreenSize; }

public:
	void OnTick();
	void OnResize(const ScreenPoint& InNewScreenSize);
	void Shutdown();

	float GetFrameFPS() const { return FrameFPS; }
	float GetElapsedTime() const { return ElapsedTime; }

public:
	// Delegates
	std::function<float()> PerformanceInitFunc;
	std::function<long long()> PerformanceMeasureFunc;

	// Input Manager
	InputManager& GetInputManager() { return InputManager; }

private:
	FORCEINLINE void BindImplClass();

	FORCEINLINE void PreUpdate();
	FORCEINLINE void PostUpdate();
	FORCEINLINE void Update();
	FORCEINLINE void RenderFrame();

private:
	// Variable to detect initialization
	bool IsPerformanceCheckInitialized = false;
	bool IsRendererInitialized = false;
	bool IsInputInitialized = false;
	bool IsImplBinded = false;
	bool IsAllInitialized = false;

	// Screen Size
	ScreenPoint CurrentScreenSize;

	// Performace counter
	long long StartTimeStamp = 0;
	long long FrameTimeStamp = 0;
	long FrameCount = 0;
	float CyclesPerMilliSeconds = 0.f;
	float FrameTime = 0.f;
	float ElapsedTime = 0.f;
	float AverageFPS = 0.f;
	float FrameFPS = 0.f;

	// Renderer Interface
	std::unique_ptr<RenderingSoftwareInterface> RSI;

	// Renderer Implement Class
	friend class SoftRendererImpl2D;
	std::unique_ptr<SoftRendererImpl2D> Impl2D;

	// Impl Functions for Rendering Logic
	std::function<void()> RenderFrameFunc;
	std::function<void(float DeltaSeconds)> UpdateFunc;

	// Input Manager
	InputManager InputManager;
};
