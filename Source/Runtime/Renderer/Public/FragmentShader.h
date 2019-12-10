#pragma once

#include <memory>

#include "RenderingSoftwareInterface.h"

class FragmentShader final
{
public:

	struct DirectionalLightBuffer
	{
		Vector4 LightColor;
		Vector3 LightDirection;
		float LightIntensity;
	};

	// Structure의 이름은 고정.
	struct FragmentInput
	{
		Vector4 Position;
		Vector3 WorldPosition;
		Vector3 WorldNormal;
	};

	DirectionalLightBuffer mDirectionalLightBuffer;

public:

	FragmentShader(const std::shared_ptr<RenderingSoftwareInterface>& InRSI, const ScreenPoint& InScreenSize) : mRSI(InRSI), mScreenSize(InScreenSize) {}
	~FragmentShader() = default;

	FORCEINLINE void ProcessFragmentShader(FragmentInput& InFragment);

private:

	FORCEINLINE const Vector4 FragmentMain(FragmentInput& InputData);

private:

	std::shared_ptr<RenderingSoftwareInterface> mRSI;
	ScreenPoint mScreenSize;

};

FORCEINLINE void FragmentShader::ProcessFragmentShader(FragmentInput& InFragment)
{
	LinearColor ShaderColor = FragmentMain(InFragment);

	ScreenPoint CurrentDrawPoint = ScreenPoint::ToScreenCoordinate(mScreenSize, InFragment.Position.ToVector2());

	mRSI->DrawPoint(CurrentDrawPoint, ShaderColor);
}

FORCEINLINE const Vector4 FragmentShader::FragmentMain(FragmentInput& InputData)
{
	Vector4 BaseColor = Vector4::Zero;
	Vector4 AmbientLight(0.1f, 0.1f, 0.1f, 1.0f);

	Vector3 InvLightDirection = -mDirectionalLightBuffer.LightDirection;
	float LDotN = Math::Saturate(InvLightDirection.Dot(InputData.WorldNormal));
	BaseColor += mDirectionalLightBuffer.LightColor * LDotN * mDirectionalLightBuffer.LightIntensity;

	BaseColor += AmbientLight;

	return Vector4::Saturate(BaseColor);
}

