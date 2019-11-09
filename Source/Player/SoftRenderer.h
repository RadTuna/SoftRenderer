#pragma once

#include <memory>
#include <functional>
#include "InputManager.h" 
#include "RenderingSoftwareInterface.h"
#include "RenderContext.h"
#include "RenderFactory.h"

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

	FORCEINLINE void PreUpdate();
	FORCEINLINE void PostUpdate();
	FORCEINLINE void Update();
	FORCEINLINE void RenderFrame();
	FORCEINLINE void SetupRenderParameter();
	FORCEINLINE void DrawGrid2D();

private:

	struct VertexDataType
	{
		Vector4 Position;
	};

private:

	// Variable to detect initialization
	bool IsPerformanceCheckInitialized = false;
	bool IsRendererInitialized = false;
	bool IsInputInitialized = false;
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

	// Rebderer Handler
	std::unique_ptr<RenderContext> RendererContext;
	std::unique_ptr<RenderFactory> RendererFactory;

	// Input Manager
	InputManager InputManager;
};


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

void SoftRenderer::Update()
{
	// GameLogic Implement;
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
	// Rendering Implement;
	DrawGrid2D();

	RendererContext->DrawCall();
}

void SoftRenderer::SetupRenderParameter()
{
	RendererContext->RSSetRasterizeState(true, false);

	VertexDataType* Vertices = new VertexDataType[4];

	Vertices[0].Position = Vector4(-100.0f, 100.0f, 0.0f, 0.0f);
	Vertices[1].Position = Vector4(100.0f, 100.0f, 0.0f, 0.0f);
	Vertices[2].Position = Vector4(100.0f, -100.0f, 0.0f, 0.0f);
	Vertices[3].Position = Vector4(-100.0f, -100.0f, 0.0f, 0.0f);

	UINT* Indices = new UINT[6]{ 0, 1, 3, 1, 2, 3 };

	VertexBuffer* RenderVertexBuffer = nullptr;
	RendererFactory->CreateVertexBuffer(sizeof(VertexDataType) * 4, Vertices, &RenderVertexBuffer);

	IndexBuffer* RenderIndexBuffer = nullptr;
	RendererFactory->CreateIndexBuffer(sizeof(UINT) * 6, Indices, &RenderIndexBuffer);

	RendererContext->IASetVertexBuffer(RenderVertexBuffer, sizeof(VertexDataType));
	RendererContext->IASetIndexBuffer(RenderIndexBuffer, sizeof(UINT));

	delete[] Vertices;
	Vertices = nullptr;

	delete[] Indices;
	Indices = nullptr;
}

void SoftRenderer::DrawGrid2D()
{
	const static int Grid2DUnit = 10;

	// Colors to use
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// Render Start
	RSI->Clear(LinearColor::White);

	// Draw Grid Line
	ScreenPoint screenHalfSize = CurrentScreenSize.GetHalf();

	for (int x = screenHalfSize.X; x <= CurrentScreenSize.X; x += Grid2DUnit)
	{
		RSI->DrawFullVerticalLine(x, gridColor);
		if (x > screenHalfSize.X)
		{
			RSI->DrawFullVerticalLine(2 * screenHalfSize.X - x, gridColor);
		}
	}

	for (int y = screenHalfSize.Y; y <= CurrentScreenSize.Y; y += Grid2DUnit)
	{
		RSI->DrawFullHorizontalLine(y, gridColor);
		if (y > screenHalfSize.Y)
		{
			RSI->DrawFullHorizontalLine(2 * screenHalfSize.Y - y, gridColor);
		}
	}

	// Draw World Axis
	RSI->DrawFullHorizontalLine(screenHalfSize.Y, LinearColor::Red);
	RSI->DrawFullVerticalLine(screenHalfSize.X, LinearColor::Green);
}

