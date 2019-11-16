#pragma once

#include <memory>

#include "RenderingSoftwareInterface.h"

class FragmentShader final
{
public:

	// Structure의 이름은 고정.
	struct FragmentInput
	{
		Vector4 Position;
		Vector3 WorldPosition;
		Vector3 WorldNormal;
	};

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
	Vector4 ImagineLightPosition(500.0f, 500.0f, 1000.0f, 0.0f);

	Vector3 InvLightDir = (ImagineLightPosition - InputData.WorldPosition).ToVector3();
	InvLightDir = InvLightDir.Normalize();
	float LDotN = InvLightDir.Dot(InputData.WorldNormal);

	Vector4 DiffuseColor = Vector4(0.7f, 0.7f, 0.3f, 1.0f) * LDotN;

	// return Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	return DiffuseColor;
}

