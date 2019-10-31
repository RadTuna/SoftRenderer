#pragma once

#include "Vector3.h"

struct Vector4
{
public:
	Vector4() { }
	FORCEINLINE Vector4(const Vector2& InV, bool IsPoint = true) : X(InV.X), Y(InV.Y), Z(0.f) { W = IsPoint ? 1.f : 0.f; }
	FORCEINLINE Vector4(const Vector3& InV, bool IsPoint = true) : X(InV.X), Y(InV.Y), Z(InV.Z) { W = IsPoint ? 1.f : 0.f; }
	FORCEINLINE Vector4(float InX, float InY, float InZ, float InW) : X(InX), Y(InY), Z(InZ), W(InW) { }

	FORCEINLINE float SizeSquared() const;
	FORCEINLINE float Dot(const Vector4& InV) const;
	FORCEINLINE bool IsZero() const
	{
		return X == 0.f && Y == 0.f && Z == 0.f && W == 0.f;
	}

	FORCEINLINE Vector2 ToVector2() const;
	FORCEINLINE Vector3 ToVector3() const;

	FORCEINLINE float operator[](int InIndex) const;
	FORCEINLINE float& operator[](int InIndex);

	FORCEINLINE Vector4 operator*(float InScale) const;
	FORCEINLINE Vector4 operator+(const Vector4& InV) const;
	FORCEINLINE Vector4 operator-(const Vector4& InV) const;
	FORCEINLINE Vector4 operator/=(float InScale);
	FORCEINLINE Vector4& operator+=(const Vector4& InV);

	static const Vector4 UnitX;
	static const Vector4 UnitY;
	static const Vector4 UnitZ;
	static const Vector4 UnitW;
	static const Vector4 One;
	static const Vector4 Zero;

public:
	float X = 0;
	float Y = 0;
	float Z = 0;
	float W = 0;
};

FORCEINLINE Vector4 Vector4::operator*(float InScale) const
{
	return Vector4(X * InScale, Y * InScale, Z * InScale, W * InScale);
}

FORCEINLINE Vector4 Vector4::operator+(const Vector4& InV) const
{
	return Vector4(X + InV.X, Y + InV.Y, Z + InV.Z, W + InV.W);
}

FORCEINLINE Vector4 Vector4::operator-(const Vector4& InV) const
{
	return Vector4(X - InV.X, Y - InV.Y, Z - InV.Z, W - InV.W);
}

FORCEINLINE Vector4 Vector4::operator/=(float InScale)
{
	X /= InScale;
	Y /= InScale;
	Z /= InScale;
	W /= InScale;
	return *this;
}

FORCEINLINE Vector4& Vector4::operator+=(const Vector4& InV)
{
	X += InV.X;
	Y += InV.Y;
	Z += InV.Z;
	W += InV.W;
	return *this;
}

FORCEINLINE float Vector4::SizeSquared() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

FORCEINLINE float Vector4::Dot(const Vector4& InV) const
{
	return X * InV.X + Y * InV.Y + Z * InV.Z + W * InV.W;
}

FORCEINLINE Vector2 Vector4::ToVector2() const
{
	return Vector2(X, Y);
}

FORCEINLINE Vector3 Vector4::ToVector3() const
{
	return Vector3(X, Y, Z);
}

FORCEINLINE float Vector4::operator[](int InIndex) const
{
	return ((float *)this)[InIndex];
}

FORCEINLINE float &Vector4::operator[](int InIndex)
{
	return ((float *)this)[InIndex];
}
