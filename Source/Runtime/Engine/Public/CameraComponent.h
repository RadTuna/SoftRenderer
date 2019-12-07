#pragma once

#include "BaseComponent.h"
#include "Matrix4x4.h"

class CameraComponent : public BaseComponent
{
public:

	CameraComponent() = default;
	virtual ~CameraComponent() = default;

	void Awake(class Entity* InParent) override;
	void Update(float DeltaTime) override;
	void Render() override;
	void End() override;

	void GetViewMatrix(Matrix4x4& OutMatrix) const;
	void GetProjectionMatrix(Matrix4x4& OutMatrix, float ScreenAspect) const;

private:

	float CameraFov = 60.0f;
	float CameraNearPlane = 0.3f;
	float CameraFarPlane = 1000.0f;

};