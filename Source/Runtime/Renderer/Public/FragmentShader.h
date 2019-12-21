#pragma once

#include <memory>

#include "RenderingSoftwareInterface.h"

class FragmentShader final
{
public:

	struct TextureBuffer
	{
		RenderTexture AlbedoMap;
	};

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
		Vector2 UV;
		Vector3 WorldPosition;
		Vector3 WorldNormal;
	};

	DirectionalLightBuffer mDirectionalLightBuffer;
	TextureBuffer mTextureBuffer;

public:

	FragmentShader(const std::shared_ptr<RenderingSoftwareInterface>& InRSI, const ScreenPoint& InScreenSize) : mRSI(InRSI), mScreenSize(InScreenSize) {}
	~FragmentShader() = default;

	FORCEINLINE void ProcessFragmentShader(FragmentInput& InFragment);

private:

	FORCEINLINE const Vector4 FragmentMain(FragmentInput& InputData);

	FORCEINLINE Vector4 SampleTexture(const RenderTexture& InTexture, const Vector2& InUV);

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
	Vector4 AlbedoColor = SampleTexture(mTextureBuffer.AlbedoMap, InputData.UV);
	Vector4 AmbientLight(0.1f, 0.1f, 0.1f, 1.0f);

	Vector3 InvLightDirection = -mDirectionalLightBuffer.LightDirection;
	float LDotN = Math::Saturate(InvLightDirection.Dot(InputData.WorldNormal));
	Vector4 DiffuseColor = mDirectionalLightBuffer.LightColor * LDotN * mDirectionalLightBuffer.LightIntensity;

	AlbedoColor = AlbedoColor + AmbientLight;

	return Vector4::Saturate(AlbedoColor);
}

Vector4 FragmentShader::SampleTexture(const RenderTexture& InTexture, const Vector2& InUV)
{
	UINT TexX = Math::Clamp<UINT>(Math::RoundToInt(InUV.X * (InTexture.TexWidth - 1)), 0, InTexture.TexWidth - 1);
	UINT TexY = Math::Clamp<UINT>(Math::RoundToInt(InUV.Y * (InTexture.TexHeight - 1)), 0, InTexture.TexHeight - 1);

	return InTexture.ColorData[InTexture.TexHeight * TexX + TexY].ToVector4();
}

