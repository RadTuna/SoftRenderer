#include "Precompiled.h"
#include "..\Public\MovementComponent.h"

#include <cassert>

#include "InputManagerProxy.h"
#include "Entity.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "MathUtil.h"

void MovementComponent::Awake(Entity* InParent)
{
	BaseComponent::Awake(InParent);
}

void MovementComponent::Update(float DeltaTime)
{
	Vector4 MovementVector;
	Vector3 RotationVector;

	MovementVector.X = -InputManagerProxy::MoveRight() * DeltaTime * mMoveSensivity;
	MovementVector.Y = -InputManagerProxy::MoveUp() * DeltaTime * mMoveSensivity;
	MovementVector.Z = InputManagerProxy::MoveForward() * DeltaTime * mMoveSensivity;

	if (InputManagerProxy::MouseRB())
	{
		RotationVector.X = -InputManagerProxy::GetYAxis() * mRotateSensivity;
		RotationVector.Y = -InputManagerProxy::GetXAxis() * mRotateSensivity;
	}

	Matrix4x4 ViewRotationMatrix = Matrix4x4::GetRotationMatrix(-mParentEntity->GetRotation());
	MovementVector = ViewRotationMatrix * MovementVector;

	mParentEntity->Translate(MovementVector);
	mParentEntity->Rotate(RotationVector);
}

void MovementComponent::Render()
{
}

void MovementComponent::End()
{
}

void MovementComponent::SetMovementParameter(float InMove, float InRot)
{
	mMoveSensivity = InMove;
	mRotateSensivity = InRot;
}
