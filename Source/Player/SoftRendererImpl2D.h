#pragma once

#include "RenderingSoftwareInterface.h"
#include "InputManager.h"

class SoftRendererImpl2D
{
public:
	SoftRendererImpl2D(class SoftRenderer* InOwner);
	~SoftRendererImpl2D();

public:
	void RenderFrameImpl();
	void UpdateImpl(float DeltaSeconds);

private:
	FORCEINLINE void DrawGrid2D();
	FORCEINLINE void DrawLine(const Vector2& StartPoint, const Vector2& EndPoint, const LinearColor& InColor);

private:
	RenderingSoftwareInterface* RSI = nullptr;

	// 2D Grid Options
	int Grid2DUnit = 10;
	ScreenPoint ScreenSize;

	// Input Manager
	InputManager InputManager;
};
