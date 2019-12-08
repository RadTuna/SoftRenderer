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

	void SetCameraParameter(float Fov, float Near, float Far);

	void GetViewMatrix(Matrix4x4& OutMatrix) const;
	void GetProjectionMatrix(Matrix4x4& OutMatrix, float ScreenAspect) const;

private:

	float mCameraFov = 1.0472f; // 60µµ
	float mCameraNearPlane = 0.3f;
	float mCameraFarPlane = 1000.0f;

};