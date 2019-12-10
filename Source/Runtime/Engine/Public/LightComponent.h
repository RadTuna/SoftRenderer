#pragma once

#include <memory>

#include "BaseComponent.h"
#include "LinearColor.h"

class RenderContext;

struct LightBufferType
{
	Vector4 LightColor;
	Vector3 LightDirection;
	float LightIntensity;
};

class LightComponent : public BaseComponent
{
public:

	LightComponent(const std::shared_ptr<RenderContext>& InRenderer);
	~LightComponent();

	void Awake(class Entity* InParent) override;
	void Update(float DeltaTime) override;
	void Render() override;
	void End() override;

	void SetLightParameter(const Vector4& InColor, float InIntensity);
	Vector3 GetLightDirection();

private:

	std::shared_ptr<RenderContext> mRenderer;
	Vector4 mLightColor;
	float mLightIntensity;

};
