#pragma once

#include "BaseComponent.h"

#include "LinearColor.h"

enum class LightMode
{
	Directional,
	Point
};

class LightComponent : public BaseComponent
{
public:

	LightComponent();
	~LightComponent();

private:

	LightMode mLightMode;
	LinearColor mLightColor;
	float mLightIntensity;
	float mLightRadius; // Only PointLight;

};
