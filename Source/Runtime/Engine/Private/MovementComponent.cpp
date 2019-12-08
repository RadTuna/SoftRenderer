#include "Precompiled.h"
#include "..\Public\MovementComponent.h"

#include <cassert>

#include "InputManagerProxy.h"
#include "Entity.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix4x4.h"

void MovementComponent::Awake(Entity* InParent)
{
	BaseComponent::Awake(InParent);
}

void MovementComponent::Update(float DeltaTime)
{
	Vector4 MovementVector;
	Vector3 RotationVector;

	MovementVector.X = -InputManagerProxy::MoveRight() * DeltaTime * MoveSensivity;
	MovementVector.Y = -InputManagerProxy::MoveUp() * DeltaTime * MoveSensivity;
	MovementVector.Z = InputManagerProxy::MoveForward() * DeltaTime * MoveSensivity;

	if (InputManagerProxy::MouseRB())
	{
		RotationVector.X = InputManagerProxy::GetYAxis() * DeltaTime * RotateSensivity;
		RotationVector.Y = InputManagerProxy::GetXAxis() * DeltaTime * RotateSensivity;
	}

	Matrix4x4 ViewRotationMatrix = Matrix4x4::GetRotationMatrix(mParentEntity->GetRotation());
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
	MoveSensivity = InMove;
	RotateSensivity = InRot;
}
