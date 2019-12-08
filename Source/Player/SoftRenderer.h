#pragma once

#include <memory>
#include <functional>

#include "InputManager.h" 
#include "RenderingSoftwareInterface.h"
#include "RenderContext.h"
#include "RenderFactory.h"
#include "Scene.h"
#include "ModelRenderComponent.h"
#include "CameraComponent.h"
#include "MovementComponent.h"


struct MatrixBufferType
{
	Matrix4x4 WorldMatrix;
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjectionMatrix;
};

class SoftRenderer
{
public:

	SoftRenderer();

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

	FORCEINLINE InputManager& GetInputManager() { return mInputManager; }

private:

	FORCEINLINE void Awake();
	FORCEINLINE void PreUpdate();
	FORCEINLINE void PostUpdate();
	FORCEINLINE void Update();
	FORCEINLINE void RenderFrame();
	FORCEINLINE void InitializeScene();
	FORCEINLINE void SetRenderParameter(ModelRenderComponent* InComp);

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

	// Renderer Handler
	std::unique_ptr<RenderContext> mRendererContext;
	std::unique_ptr<RenderFactory> mRendererFactory;


	std::unique_ptr<Scene> mSceneLevel;
	CameraComponent* mCurrentCameraComp;

	InputManager mInputManager;

};


FORCEINLINE void SoftRenderer::Awake()
{
	InitializeScene();
	
	for (auto Iter = mSceneLevel->GetEntities().begin(); Iter != mSceneLevel->GetEntities().end(); ++Iter)
	{
		(*Iter)->Awake();
	}
}

FORCEINLINE void SoftRenderer::PreUpdate()
{
	FrameTimeStamp = PerformanceMeasureFunc();
	if (FrameCount == 0)
	{
		StartTimeStamp = FrameTimeStamp;
	}
}

FORCEINLINE void SoftRenderer::Update()
{
	// GameLogic Implement
	float DeltaSeconde = FrameTime / 1000.0f;

	//float MoveSensivity = 100.0f;
	//float RotateSensivity = 50.0f;

	//// CameraLocation
	//mCameraLocation.Z += mInputManager.MoveForward() * DeltaSeconde * MoveSensivity;
	//mCameraLocation.X -= mInputManager.MoveRight() * DeltaSeconde * MoveSensivity;
	//mCameraLocation.Y -= mInputManager.MoveUp() * DeltaSeconde * MoveSensivity;

	//// CameraRotation
	//mCameraRotation.Y += mInputManager.GetXAxis() * DeltaSeconde * RotateSensivity;
	//mCameraRotation.X -= mInputManager.GetYAxis() * DeltaSeconde * RotateSensivity;

	for (auto Iter = mSceneLevel->GetEntities().begin(); Iter != mSceneLevel->GetEntities().end(); ++Iter)
	{
		(*Iter)->Update(DeltaSeconde);
	}
}

FORCEINLINE void SoftRenderer::PostUpdate()
{
	// Unload Level
	RenderFrame();

	// Render Finish
	mRendererContext->Represent();

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

FORCEINLINE void SoftRenderer::RenderFrame()
{
	// Clear Buffer
	mRendererContext->ClearBackbuffer(LinearColor::White);
	mRendererContext->DrawGrid2D();

	for (auto Iter = mSceneLevel->GetEntities().begin(); Iter != mSceneLevel->GetEntities().end(); ++Iter)
	{
		(*Iter)->Render();
		ModelRenderComponent* CurrentRenderComp = (*Iter)->GetComponent<ModelRenderComponent>();
		if (CurrentRenderComp == nullptr)
		{
			continue;
		}

		SetRenderParameter(CurrentRenderComp);
		mRendererContext->DrawCall();
	}
}

FORCEINLINE void SoftRenderer::InitializeScene()
{
	mSceneLevel = std::make_unique<Scene>();

	std::unique_ptr<ModelRenderComponent> ModelRenderComp = std::make_unique<ModelRenderComponent>(mRendererContext.get(), mRendererFactory.get());
	std::unique_ptr<Entity> TestModelEntity = std::make_unique<Entity>("Model");

	// Temporary Meshdata Injection code [Start];
	UINT VertexBufferLength = 8;
	UINT IndexBufferLength = 36;

	std::unique_ptr<VertexDataType[]> Vertices = std::make_unique<VertexDataType[]>(VertexBufferLength);
	Vertices[0].Position = Vector4(-10.0f, 10.0f, -10.0f, 0.0f);
	Vertices[0].Normal = Vector3(-1.0f, 1.0f, -1.0f);
	Vertices[1].Position = Vector4(10.0f, 10.0f, -10.0f, 0.0f);
	Vertices[1].Normal = Vector3(1.0f, 1.0f, -1.0f);
	Vertices[2].Position = Vector4(10.0f, 10.0f, 10.0f, 0.0f);
	Vertices[2].Normal = Vector3(1.0f, 1.0f, 1.0f);
	Vertices[3].Position = Vector4(-10.0f, 10.0f, 10.0f, 0.0f);
	Vertices[3].Normal = Vector3(-1.0f, 1.0f, 1.0f);
	Vertices[4].Position = Vector4(-10.0f, -10.0f, -10.0f, 0.0f);
	Vertices[4].Normal = Vector3(-1.0f, -1.0f, -1.0f);
	Vertices[5].Position = Vector4(10.0f, -10.0f, -10.0f, 0.0f);
	Vertices[5].Normal = Vector3(1.0f, -1.0f, -1.0f);
	Vertices[6].Position = Vector4(10.0f, -10.0f, 10.0f, 0.0f);
	Vertices[6].Normal = Vector3(1.0f, -1.0f, 1.0f);
	Vertices[7].Position = Vector4(-10.0f, -10.0f, 10.0f, 0.0f);
	Vertices[7].Normal = Vector3(-1.0f, -1.0f, 1.0f);

	std::unique_ptr<UINT[]> Indices(new UINT[IndexBufferLength]{
		1, 0, 3, 1, 3, 2,
		0, 1, 4, 1, 5, 4,
		0, 4, 3, 3, 4, 7,
		2, 3, 7, 2, 7, 6,
		2, 6, 5, 2, 5, 1,
		4, 5, 7, 5, 6, 7 });

	ModelRenderComp->SetMeshData(std::move(Vertices), VertexBufferLength);
	ModelRenderComp->SetIndexData(std::move(Indices), IndexBufferLength);
	// Temporary Meshdata Injection code [End];

	TestModelEntity->AddComponent(std::move(ModelRenderComp));

	std::unique_ptr<CameraComponent> CameraComp = std::make_unique<CameraComponent>();
	std::unique_ptr<MovementComponent> MovementComp = std::make_unique<MovementComponent>();
	std::unique_ptr<Entity> CameraEntity = std::make_unique<Entity>("Camera");
	CameraComp->SetCameraParameter(60.0f, 0.3f, 1000.0f);
	CameraEntity->SetLocation(Vector4(0.0f, 0.0f, -150.0f, 1.0f));
	CameraEntity->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
	MovementComp->SetMovementParameter(100.0f, 100.0f);
	mCurrentCameraComp = CameraComp.get();

	CameraEntity->AddComponent(std::move(CameraComp));
	CameraEntity->AddComponent(std::move(MovementComp));

	mSceneLevel->AddEntity(std::move(TestModelEntity));
	mSceneLevel->AddEntity(std::move(CameraEntity));
}

FORCEINLINE void SoftRenderer::SetRenderParameter(ModelRenderComponent* InComp)
{
	assert(InComp);

	mRendererContext->RSSetRasterizeState(false, true, CullingMode::CULL_BACK);

	float ScreenAspect = static_cast<float>(CurrentScreenSize.X) / static_cast<float>(CurrentScreenSize.Y);

	std::unique_ptr<MatrixBufferType> RenderMatrixBuffer = std::make_unique<MatrixBufferType>();
	InComp->GetWorldMatrix(RenderMatrixBuffer->WorldMatrix);
	mCurrentCameraComp->GetViewMatrix(RenderMatrixBuffer->ViewMatrix);
	mCurrentCameraComp->GetProjectionMatrix(RenderMatrixBuffer->ProjectionMatrix, ScreenAspect);

	mRendererContext->VSSetMatrixBuffer(RenderMatrixBuffer.get());
}

