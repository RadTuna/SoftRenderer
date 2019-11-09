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

	FORCEINLINE void CalculrateWorldMatrix(Matrix4x4& WorldMatrix);
	FORCEINLINE void CalculrateViewMatrix(Matrix4x4& ViewMatrix);
	FORCEINLINE void CalculrateProjectionMatrix(Matrix4x4& ProjectionMatrix, float Fov, float Near, float Far);
	FORCEINLINE void CalculrateOrthographicMatrix(Matrix4x4& OrthographicMatrix);

private:

	struct VertexDataType
	{
		Vector4 Position;
	};

	struct MatrixBufferType
	{
		Matrix4x4 WorldMatrix;
		Matrix4x4 ViewMatrix;
		Matrix4x4 ProjectionMatrix;
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

private:

	// Temporary Variables!!!
	Vector4 CameraLocation;
	Vector4 CameraRotation;

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
	// GameLogic Implement
	float DeltaSeconde = FrameTime / 1000.0f;

	// CameraLocation
	CameraLocation.Z += InputManager.MoveForward();
	CameraLocation.X += InputManager.MoveRight();
	CameraLocation.Y += InputManager.MoveUp();

	// CameraRotation
	CameraRotation.X += InputManager.GetXAxis() * DeltaSeconde;
	CameraRotation.Y += InputManager.GetYAxis() * DeltaSeconde;

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

	MatrixBufferType* MatrixBuffer = new MatrixBufferType;
	CalculrateWorldMatrix(MatrixBuffer->WorldMatrix);
	CalculrateViewMatrix(MatrixBuffer->ViewMatrix);
	CalculrateProjectionMatrix(MatrixBuffer->ProjectionMatrix, 90.0f, 0.1f, 1000.0f);

	RendererContext->VSSetMatrixBuffer(MatrixBuffer);

	RendererContext->DrawCall();

	delete MatrixBuffer;
	MatrixBuffer = nullptr;
}

void SoftRenderer::SetupRenderParameter()
{
	RendererContext->RSSetRasterizeState(true, false);

	VertexDataType* Vertices = new VertexDataType[4];

	Vertices[0].Position = Vector4(-100.0f, 100.0f, 1.0f, 0.0f);
	Vertices[1].Position = Vector4(100.0f, 100.0f, 1.0f, 0.0f);
	Vertices[2].Position = Vector4(100.0f, -100.0f, 1.0f, 0.0f);
	Vertices[3].Position = Vector4(-100.0f, -100.0f, 1.0f, 0.0f);

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

void SoftRenderer::CalculrateWorldMatrix(Matrix4x4& WorldMatrix)
{
	// 단위 행렬로 반환함.
	WorldMatrix.SetIdentity();
}

void SoftRenderer::CalculrateViewMatrix(Matrix4x4& ViewMatrix)
{
	Vector3 LookVector(0.0f, 0.0f, -1.0f);
	Vector3 UpVector(0.0f, 1.0f, 0.0f);

	Matrix4x4 RotationMatrix = Matrix4x4::GetRotationMatrix(CameraRotation);
	
	LookVector *= RotationMatrix;
	UpVector *= RotationMatrix;

	Vector3 XAxisVector = LookVector.Cross(UpVector);
	UpVector = LookVector.Cross(XAxisVector);

	LookVector.Normalize();
	UpVector.Normalize();
	XAxisVector.Normalize();

	Matrix4x4 GenViewMatrix(
		Vector4(XAxisVector.X, UpVector.X, LookVector.X, 0.0f),
		Vector4(XAxisVector.Y, UpVector.Y, LookVector.Y, 0.0f),
		Vector4(XAxisVector.Z, UpVector.Z, LookVector.Z, 0.0f),
		Vector4(-CameraLocation.X, -CameraLocation.Y, -CameraLocation.Z, 1.0f));

	ViewMatrix = GenViewMatrix;
}

void SoftRenderer::CalculrateProjectionMatrix(Matrix4x4& ProjectionMatrix, float Fov, float Near, float Far)
{
	float ScreenAspect = static_cast<float>(CurrentScreenSize.X) / static_cast<float>(CurrentScreenSize.Y);

	float YScale = 1.0f / std::tanf(Fov / 2.0f);
	float XScale = YScale * ScreenAspect;

	float ZScale = Far / (Far - Near);
	float ZMove = -Near * ZScale;

	Matrix4x4 GenProjectionMatrix(
		Vector4(XScale, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, YScale, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, ZScale, 1.0f),
		Vector4(0.0f, 0.0f, ZMove, 0.0f));

	ProjectionMatrix = GenProjectionMatrix;
}

