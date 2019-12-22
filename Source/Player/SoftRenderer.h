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
#include "LightComponent.h"


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
	std::shared_ptr<RenderContext> mRendererContext;
	std::shared_ptr<RenderFactory> mRendererFactory;


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

	for (auto Iter = mSceneLevel->GetEntities().begin(); Iter != mSceneLevel->GetEntities().end(); ++Iter)
	{
		(*Iter)->Update(FrameTime / 1000.0f);
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

	std::unique_ptr<ModelRenderComponent> ModelRenderComp = std::make_unique<ModelRenderComponent>(mRendererContext, mRendererFactory);
	std::unique_ptr<Entity> TestModelEntity = std::make_unique<Entity>("Model");

	TestModelEntity->AddComponent(std::move(ModelRenderComp));

	std::unique_ptr<CameraComponent> CameraComp = std::make_unique<CameraComponent>();
	std::unique_ptr<MovementComponent> MovementComp = std::make_unique<MovementComponent>();
	std::unique_ptr<Entity> CameraEntity = std::make_unique<Entity>("Camera");
	CameraComp->SetCameraParameter(60.0f, 5.0f, 500.0f);
	CameraEntity->SetLocation(Vector4(0.0f, 0.0f, -150.0f, 1.0f));
	CameraEntity->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
	MovementComp->SetMovementParameter(100.0f, 50.0f);
	mCurrentCameraComp = CameraComp.get();

	CameraEntity->AddComponent(std::move(CameraComp));
	CameraEntity->AddComponent(std::move(MovementComp));

	std::unique_ptr<LightComponent> LightComp = std::make_unique<LightComponent>(mRendererContext);
	std::unique_ptr<Entity> LightEntity = std::make_unique<Entity>("Light");
	LightComp->SetLightParameter(Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
	LightEntity->SetRotation(Vector3(30.0f, 30.0f, 0.0f));

	LightEntity->AddComponent(std::move(LightComp));

	mSceneLevel->AddEntity(std::move(TestModelEntity));
	mSceneLevel->AddEntity(std::move(CameraEntity));
	mSceneLevel->AddEntity(std::move(LightEntity));
}

FORCEINLINE void SoftRenderer::SetRenderParameter(ModelRenderComponent* InComp)
{
	assert(InComp);

	mRendererContext->RSSetRasterizeState(false, true, CullingMode::CULL_BACK);

	float ScreenAspect = static_cast<float>(CurrentScreenSize.X) / static_cast<float>(CurrentScreenSize.Y);

	MatrixBufferType RenderMatrixBuffer;
	InComp->GetWorldMatrix(RenderMatrixBuffer.WorldMatrix);
	mCurrentCameraComp->GetViewMatrix(RenderMatrixBuffer.ViewMatrix);
	mCurrentCameraComp->GetProjectionMatrix(RenderMatrixBuffer.ProjectionMatrix, ScreenAspect);

	mRendererContext->VSSetMatrixBuffer(&RenderMatrixBuffer);
}

