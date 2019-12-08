#include "Precompiled.h"
#include "..\Public\CameraComponent.h"

#include <cmath>

#include "Entity.h"
#include "RenderContext.h"
#include "MathUtil.h"

void CameraComponent::Awake(Entity* InParent)
{
	BaseComponent::Awake(InParent);
}

void CameraComponent::Update(float DeltaTime)
{
}

void CameraComponent::Render()
{
}

void CameraComponent::End()
{
}

void CameraComponent::SetCameraParameter(float Fov, float Near, float Far)
{
	mCameraFov = Fov;
	mCameraNearPlane = Near;
	mCameraFarPlane = Far;
}

void CameraComponent::GetViewMatrix(Matrix4x4& OutMatrix) const
{
	Vector3 V3CameraLocation = mParentEntity->GetLocation().ToVector3();
	Vector3 LookVector(0.0f, 0.0f, 1.0f);
	Vector3 UpVector(0.0f, 1.0f, 0.0f);

	Matrix4x4 RotationMatrix = Matrix4x4::GetRotationMatrix(mParentEntity->GetRotation());

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

	OutMatrix = GenViewMatrix;
}

void CameraComponent::GetProjectionMatrix(Matrix4x4& OutMatrix, float ScreenAspect) const
{
	float YScale = 1.0f / std::tanf(Math::Deg2Rad(mCameraFov) / 2.0f);
	float XScale = YScale * ScreenAspect;

	float ZScale = mCameraFarPlane / (mCameraFarPlane - mCameraNearPlane);
	float ZMove = -mCameraNearPlane * ZScale;

	Matrix4x4 GenProjectionMatrix(
		Vector4(XScale, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, YScale, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, ZScale, 1.0f),
		Vector4(0.0f, 0.0f, ZMove, 0.0f));

	OutMatrix = GenProjectionMatrix;
}
