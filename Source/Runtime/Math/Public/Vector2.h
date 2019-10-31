#pragma once

#include "CoreDefinition.h"

struct Vector2
{
public:
	Vector2() { }
	FORCEINLINE Vector2(int InX, int InY) : X((float)InX), Y((float)InY) { }
	FORCEINLINE Vector2(float InX, float InY) : X(InX), Y(InY) { }

	FORCEINLINE float SizeSquared() const;
	FORCEINLINE float Dot(const Vector2& InV) const;
	FORCEINLINE Vector2 Normalize() const;

	FORCEINLINE bool IsZero() const
	{
		return X == 0.f && Y == 0.f;
	}

	FORCEINLINE float operator[](int InIndex) const;
	FORCEINLINE float& operator[](int InIndex);

	FORCEINLINE Vector2 operator*(float InScalar) const;
	FORCEINLINE Vector2 operator/(float InScalar) const;
	FORCEINLINE Vector2 operator+(const Vector2& InV) const;
	FORCEINLINE Vector2 operator-(const Vector2& InV) const;
	FORCEINLINE Vector2& operator+=(const Vector2& InV);

	static const Vector2 UnitX;
	static const Vector2 UnitY;
	static const Vector2 One;
	static const Vector2 Zero;

public:
	float X = 0.f;
	float Y = 0.f;
};

FORCEINLINE Vector2 Vector2::operator*(float InScalar) const
{
	return Vector2(X * InScalar, Y * InScalar);
}

FORCEINLINE Vector2 Vector2::operator/(float InScalar) const
{
	return Vector2(X / InScalar, Y / InScalar);
}

FORCEINLINE Vector2 Vector2::operator+(const Vector2& InV) const
{
	return Vector2(X + InV.X, Y + InV.Y);
}

FORCEINLINE Vector2 Vector2::operator-(const Vector2& InV) const
{
	return Vector2(X - InV.X, Y - InV.Y);
}

FORCEINLINE Vector2& Vector2::operator+=(const Vector2& InV)
{
	X += InV.X;
	Y += InV.Y;
	return *this;
}


FORCEINLINE float Vector2::SizeSquared() const
{
	return X * X + Y * Y;
}

FORCEINLINE float Vector2::Dot(const Vector2& InV) const
{
	return X * InV.X + Y * InV.Y;
}

FORCEINLINE float Vector2::operator[](int InIndex) const 
{
	return ((float *)this)[InIndex];
}

FORCEINLINE float &Vector2::operator[](int InIndex)
{
	return ((float *)this)[InIndex];
}

FORCEINLINE Vector2 Vector2::Normalize() const
{
	float squareSum = SizeSquared();
	if (squareSum == 1.f)
	{
		return *this;
	}
	else if (squareSum == 0.f)
	{
		return Vector2::Zero;
	}

	float invLength = 1 / sqrtf(squareSum);
	return Vector2(X * invLength, Y * invLength);
}