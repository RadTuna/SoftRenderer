#include "Precompiled.h"
#include "..\Public\LightComponent.h"

#include "RenderContext.h"
#include "Entity.h"

LightComponent::LightComponent(const std::shared_ptr<RenderContext>& InRenderer)
{
	mRenderer = InRenderer;
}

LightComponent::~LightComponent()
{
}

void LightComponent::Awake(Entity* InParent)
{
	BaseComponent::Awake(InParent);
}

void LightComponent::Update(float DeltaTime)
{
	LightBufferType LightBuffer;

	LightBuffer.LightColor = mLightColor;
	LightBuffer.LightDirection = GetLightDirection();
	LightBuffer.LightIntensity = mLightIntensity;

	mRenderer->FSSetDirectionalLightBuffer(&LightBuffer);
}

void LightComponent::Render()
{
}

void LightComponent::End()
{
}

void LightComponent::SetLightParameter(const Vector4& InColor, float InIntensity)
{
	mLightColor = InColor;
	mLightIntensity = InIntensity;
}

Vector3 LightComponent::GetLightDirection()
{
	Vector4 ForwardVector(0.0f, 0.0f, 1.0f, 0.0f);
	Matrix4x4 RotationMatrix = Matrix4x4::GetRotationMatrix(mParentEntity->GetRotation());
	
	return (RotationMatrix * ForwardVector).ToVector3();
}
