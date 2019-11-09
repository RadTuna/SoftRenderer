#pragma once

#include "Vector4.h"
#include "Matrix2x2.h"
#include "Matrix3x3.h"
#include "MathUtil.h"

struct Matrix4x4
{
public:
	Matrix4x4();
	Matrix4x4(const Vector4& InCol0, const Vector4& InCol1, const Vector4& InCol2, const Vector4& InCol3);

	FORCEINLINE void SetIdentity();
	FORCEINLINE Matrix4x4 Tranpose() const;
	FORCEINLINE static Matrix4x4 GetRotationMatrix(const Vector4& Rotation);
	FORCEINLINE static Matrix4x4 GetScaleMatrix(const Vector4& Scale);
	FORCEINLINE static Matrix4x4 GetLocationMatrix(const Vector4& Location);

	FORCEINLINE const Vector4& operator[](int InIndex) const;
	FORCEINLINE Vector4& operator[](int InIndex);

	FORCEINLINE Matrix4x4 operator*(const Matrix4x4& InM) const;
	FORCEINLINE Vector4 operator*(const Vector4& InV) const;
	FORCEINLINE friend Vector4 operator*=(Vector4& InV, const Matrix4x4& InM)
	{
		InV = InM * InV;
		return InV;
	}

	FORCEINLINE Vector3 operator*(const Vector3& InV) const;
	FORCEINLINE friend Vector3 operator*=(Vector3& InV, const Matrix4x4& InM)
	{
		InV = InM * InV;
		return InV;
	}

	FORCEINLINE Matrix4x4 operator*(float InS) const;

private:

	Vector4 Cols[4];

};

FORCEINLINE void Matrix4x4::SetIdentity()
{
	Cols[0] = Vector4::UnitX;
	Cols[1] = Vector4::UnitY;
	Cols[2] = Vector4::UnitZ;
	Cols[3] = Vector4::UnitW;
}

FORCEINLINE Matrix4x4 Matrix4x4::Tranpose() const
{
	return Matrix4x4(
		Vector4(Cols[0].X, Cols[1].X, Cols[2].X, Cols[3].X),
		Vector4(Cols[0].Y, Cols[1].Y, Cols[2].Y, Cols[3].Y),
		Vector4(Cols[0].Z, Cols[1].Z, Cols[2].Z, Cols[3].Z),
		Vector4(Cols[0].W, Cols[1].W, Cols[2].W, Cols[3].W)
	);
}

inline Matrix4x4 Matrix4x4::GetRotationMatrix(const Vector4& Rotation)
{
	float XSin, XCos;
	Math::SinCos(&XSin, &XCos, Rotation.X);
	Matrix4x4 XRotMatrix(
		Vector4(1.0f, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, XCos, -XSin, 0.0f),
		Vector4(0.0f, XSin, XCos, 0.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	float YSin, YCos;
	Math::SinCos(&YSin, &YCos, Rotation.Y);
	Matrix4x4 YRotMatrix(
		Vector4(YCos, 0.0f, YSin, 0.0f),
		Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		Vector4(-YSin, 0.0f, YCos, 0.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	float ZSin, ZCos;
	Math::SinCos(&ZSin, &ZCos, Rotation.Z);
	Matrix4x4 ZRotMatrix(
		Vector4(ZCos, -ZSin, 0.0f, 0.0f),
		Vector4(ZSin, ZCos, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, 1.0f, 0.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	return XRotMatrix * YRotMatrix * ZRotMatrix;
}

inline Matrix4x4 Matrix4x4::GetScaleMatrix(const Vector4& Scale)
{
	Matrix4x4 ScaleMatrix(
		Vector4(Scale.X, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, Scale.Y, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, Scale.Z, 0.0f),
		Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	return ScaleMatrix;
}

inline Matrix4x4 Matrix4x4::GetLocationMatrix(const Vector4& Location)
{
	Matrix4x4 LocationMatrix(
		Vector4(1.0f, 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, 1.0f, 0.0f, 0.0f),
		Vector4(0.0f, 0.0f, 1.0f, 0.0f),
		Vector4(Location.X, Location.Y, Location.Z, 1.0f));

	return LocationMatrix;
}

FORCEINLINE const Vector4& Matrix4x4::operator[](int InIndex) const
{
	return Cols[InIndex];
}

FORCEINLINE Vector4& Matrix4x4::operator[](int InIndex)
{
	return Cols[InIndex];
}


FORCEINLINE Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &InM) const
{
	Matrix4x4 tpMat = Tranpose();
	return Matrix4x4(
		Vector4(tpMat[0].Dot(InM[0]), tpMat[1].Dot(InM[0]), tpMat[2].Dot(InM[0]), tpMat[3].Dot(InM[0])),
		Vector4(tpMat[0].Dot(InM[1]), tpMat[1].Dot(InM[1]), tpMat[2].Dot(InM[1]), tpMat[3].Dot(InM[1])),
		Vector4(tpMat[0].Dot(InM[2]), tpMat[1].Dot(InM[2]), tpMat[2].Dot(InM[2]), tpMat[3].Dot(InM[2])),
		Vector4(tpMat[0].Dot(InM[3]), tpMat[1].Dot(InM[3]), tpMat[2].Dot(InM[3]), tpMat[3].Dot(InM[3]))
	);
}

FORCEINLINE Vector4 Matrix4x4::operator*(const Vector4& InV) const
{
	Matrix4x4 tpMat = Tranpose();
	return Vector4(
		tpMat[0].Dot(InV),
		tpMat[1].Dot(InV),
		tpMat[2].Dot(InV),
		tpMat[3].Dot(InV)
	);
}

FORCEINLINE Vector3 Matrix4x4::operator*(const Vector3& InV) const
{
	Vector4 V4(InV);
	V4 *= *this;

	return V4.ToVector3();
}

FORCEINLINE Matrix4x4 Matrix4x4::operator*(float InS) const
{
	return Matrix4x4(
		Cols[0] * InS,
		Cols[1] * InS,
		Cols[2] * InS,
		Cols[3] * InS
	);
}
