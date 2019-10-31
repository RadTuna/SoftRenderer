#pragma once

#include "CoreDefinition.h"
#include "Color32.h"
#include "Math.h"

struct LinearColor
{
public:
	FORCEINLINE LinearColor() {}
	constexpr FORCEINLINE LinearColor(float InR, float InG, float InB, float InA = 1.f) : R(InR), G(InG), B(InB), A(InA) {}
	LinearColor(const Color32& InColor32, const bool bSRGB = false);

	FORCEINLINE bool EqualsInRange(const LinearColor& InColor, float InRange) const;
	Color32 ToColor32(const bool bSRGB = false) const;

	FORCEINLINE LinearColor operator+(const LinearColor& InColor) const;
	FORCEINLINE LinearColor operator-(const LinearColor& InColor) const;
	FORCEINLINE LinearColor operator*(const LinearColor& InColor) const;
	FORCEINLINE LinearColor operator*(float InScalar) const;
	FORCEINLINE bool operator==(const LinearColor& InColor) const;
	FORCEINLINE bool operator!=(const LinearColor& InColor) const;

	static const float OneOver255;
	static const LinearColor Error;
	static const LinearColor White;
	static const LinearColor Black;
	static const LinearColor Red;
	static const LinearColor Green;
	static const LinearColor Blue;
	static const LinearColor Yellow;

public:
	float R = 0.f;
	float G = 0.f;
	float B = 0.f;
	float A = 1.f;
};

FORCEINLINE bool LinearColor::EqualsInRange(const LinearColor& InColor, float InRange) const
{
	return (Math::Abs(this->R - InColor.R) < InRange) && 
		(Math::Abs(this->G - InColor.G) < InRange) &&
		(Math::Abs(this->B - InColor.B) < InRange) && 
		(Math::Abs(this->A - InColor.A) < InRange);
}

FORCEINLINE LinearColor LinearColor::operator+(const LinearColor& InColor) const
{
	return LinearColor(
		R + InColor.R,
		G + InColor.G,
		B + InColor.B,
		A + InColor.A
	);
}

FORCEINLINE LinearColor LinearColor::operator-(const LinearColor& InColor) const
{
	return LinearColor(
		R - InColor.R,
		G - InColor.G,
		B - InColor.B,
		A - InColor.A
	);
}

FORCEINLINE LinearColor LinearColor::operator*(const LinearColor& InColor) const
{
	return LinearColor(
		R * InColor.R,
		G * InColor.G,
		B * InColor.B,
		A * InColor.A
	);
}

FORCEINLINE LinearColor LinearColor::operator*(float InScalar) const
{
	return LinearColor(
		R * InScalar,
		G * InScalar,
		B * InScalar,
		A * InScalar
	);
}

FORCEINLINE bool LinearColor::operator==(const LinearColor& InColor) const
{
	return this->R == InColor.R && this->G == InColor.G && this->B == InColor.B && this->A == InColor.A;
}

FORCEINLINE bool LinearColor::operator!=(const LinearColor& InColor) const
{
	return this->R != InColor.R || this->G != InColor.G || this->B != InColor.B || this->A != InColor.A;
}