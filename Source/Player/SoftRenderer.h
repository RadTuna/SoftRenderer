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

	float MoveSensivity = 100.0f;
	float RotateSensivity = 50.0f;

	// CameraLocation
	CameraLocation.Z += InputManager.MoveForward() * DeltaSeconde * MoveSensivity;
	CameraLocation.X -= InputManager.MoveRight() * DeltaSeconde * MoveSensivity;
	CameraLocation.Y -= InputManager.MoveUp() * DeltaSeconde * MoveSensivity;

	// CameraRotation
	CameraRotation.Y += InputManager.GetXAxis() * DeltaSeconde * RotateSensivity;
	CameraRotation.X -= InputManager.GetYAxis() * DeltaSeconde * RotateSensivity;

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

	SetupRenderParameter();
	static float MeshRotation = 0.0f;
	float DeltaTime = FrameTime / 1000.0f;
	MeshRotation += DeltaTime * 5000.0f;

	MatrixBufferType* MatrixBuffer = new MatrixBufferType;
	MatrixBuffer->WorldMatrix = Matrix4x4::GetRotationMatrix(Vector4(0.0f, Math::Deg2Rad(MeshRotation), 0.0f, 0.0f));

	// CalculrateWorldMatrix(MatrixBuffer->WorldMatrix);
	CalculrateViewMatrix(MatrixBuffer->ViewMatrix);

	// Matrix, Fov, Near, Far
	CalculrateProjectionMatrix(MatrixBuffer->ProjectionMatrix, Math::Deg2Rad(60.0f), 1.0f, 1000.0f);

	RendererContext->VSSetMatrixBuffer(MatrixBuffer);

	RendererContext->DrawCall();

	delete MatrixBuffer;
	MatrixBuffer = nullptr;
}

void SoftRenderer::SetupRenderParameter()
{
	RendererContext->RSSetRasterizeState(true, false, CullingMode::CULL_BACK);

	VertexDataType* Vertices = new VertexDataType[8];

	Vertices[0].Position = Vector4(-10.0f, 10.0f, -10.0f, 0.0f);
	Vertices[1].Position = Vector4(10.0f, 10.0f, -10.0f, 0.0f);
	Vertices[2].Position = Vector4(10.0f, 10.0f, 10.0f, 0.0f);
	Vertices[3].Position = Vector4(-10.0f, 10.0f, 10.0f, 0.0f);
	Vertices[4].Position = Vector4(-10.0f, -10.0f, -10.0f, 0.0f);
	Vertices[5].Position = Vector4(10.0f, -10.0f, -10.0f, 0.0f);
	Vertices[6].Position = Vector4(10.0f, -10.0f, 10.0f, 0.0f);
	Vertices[7].Position = Vector4(-10.0f, -10.0f, 10.0f, 0.0f);

	UINT* Indices = new UINT[36]{
		1, 0, 3, 1, 3, 2,
		0, 1, 4, 1, 5, 4,
		0, 4, 3, 3, 4, 7,
		2, 3, 7, 2, 7, 6,
		2, 6, 5, 2, 5, 1,
		4, 5, 7, 5, 6, 7 };

	VertexBuffer* RenderVertexBuffer = nullptr;
	RendererFactory->CreateVertexBuffer(sizeof(VertexDataType) * 8, Vertices, &RenderVertexBuffer);

	IndexBuffer* RenderIndexBuffer = nullptr;
	RendererFactory->CreateIndexBuffer(sizeof(UINT) * 36, Indices, &RenderIndexBuffer);

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
	Vector3 V3CameraLocation = CameraLocation.ToVector3();
	Vector3 LookVector(0.0f, 0.0f, 1.0f);
	Vector3 UpVector(0.0f, 1.0f, 0.0f);

	Matrix4x4 RotationMatrix = Matrix4x4::GetRotationMatrix(CameraRotation);
	
	LookVector *= RotationMatrix;
	UpVector *= RotationMatrix;

	LookVector += V3CameraLocation;

	Vector3 ZAxisVector = LookVector - V3CameraLocation;

	Vector3 XAxisVector = ZAxisVector.Cross(UpVector);
	UpVector = ZAxisVector.Cross(XAxisVector);

	ZAxisVector.Normalize();
	UpVector.Normalize();
	XAxisVector.Normalize();

	Matrix4x4 GenViewMatrix(
		Vector4(XAxisVector.X, UpVector.X, ZAxisVector.X, 0.0f),
		Vector4(XAxisVector.Y, UpVector.Y, ZAxisVector.Y, 0.0f),
		Vector4(XAxisVector.Z, UpVector.Z, ZAxisVector.Z, 0.0f),
		Vector4(-XAxisVector.Dot(V3CameraLocation), -UpVector.Dot(V3CameraLocation), -ZAxisVector.Dot(V3CameraLocation), 1.0f));

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

