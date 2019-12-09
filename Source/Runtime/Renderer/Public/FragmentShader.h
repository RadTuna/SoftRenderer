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

	struct PointLightBuffer
	{
		Vector4 LightColor;
		Vector4 LightPosition;
		float LightIntensity;
		float LightRadius;
	};

	// Structure의 이름은 고정.
	struct FragmentInput
	{
		Vector4 Position;
		Vector3 WorldPosition;
		Vector3 WorldNormal;
	};

	DirectionalLightBuffer* mDirectionalLightBuffers = nullptr;
	UINT mDirectionalLightBufferLength = 0;
	PointLightBuffer* mPointLightBuffers = nullptr;
	UINT mPointLightBufferLength = 0;

public:

	FragmentShader(const std::shared_ptr<RenderingSoftwareInterface>& InRSI, const ScreenPoint& InScreenSize) : mRSI(InRSI), mScreenSize(InScreenSize) {}
	~FragmentShader() = default;

	FORCEINLINE void ProcessFragmentShader(FragmentInput& InFragment);

private:

	FORCEINLINE const Vector4 DirectionalLightPass(FragmentInput& InputData, UINT NumPass);
	FORCEINLINE const Vector4 PointLightPass(FragmentInput& InputData, UINT NumPass);
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

FORCEINLINE const Vector4 FragmentShader::DirectionalLightPass(FragmentInput& InputData, UINT NumPass)
{
	// Diffuse
	Vector3 InvLightDir = -mDirectionalLightBuffers[NumPass].LightDirection;
	InvLightDir = InvLightDir.Normalize();
	float LDotN = InvLightDir.Dot(InputData.WorldNormal);

	Vector4 Diffuse = mDirectionalLightBuffers[NumPass].LightColor * LDotN;

	return mDirectionalLightBuffers[NumPass].LightColor * LDotN * mPointLightBuffers[NumPass].LightIntensity;
}

FORCEINLINE const Vector4 FragmentShader::PointLightPass(FragmentInput& InputData, UINT NumPass)
{
	Vector3 InvLightDir = (mPointLightBuffers[NumPass].LightPosition - InputData.WorldPosition).ToVector3();
	InvLightDir = InvLightDir.Normalize();
	float LDotN = InvLightDir.Dot(InputData.WorldNormal);

	return mPointLightBuffers[NumPass].LightColor * LDotN * mPointLightBuffers[NumPass].LightIntensity;
}

FORCEINLINE const Vector4 FragmentShader::FragmentMain(FragmentInput& InputData)
{
	Vector4 BaseColor = Vector4::Zero;
	Vector4 AmbientLight(0.1f, 0.1f, 0.1f, 1.0f);

	// Directional light multipass
	for (int Dir = 0; Dir < mDirectionalLightBufferLength; ++Dir)
	{
		BaseColor += DirectionalLightPass(InputData, Dir);
	}

	// Point light multipass
	for (int Point = 0; Point < mPointLightBufferLength; ++Point)
	{
		BaseColor += PointLightPass(InputData, Point);
	}

	BaseColor += AmbientLight;

	return Math::Saturate(BaseColor);
}

