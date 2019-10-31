#pragma once

#include "Vector4.h"
#include "Matrix2x2.h"
#include "Matrix3x3.h"

struct Matrix4x4
{
public:
	Matrix4x4();
	Matrix4x4(const Vector4& InCol0, const Vector4& InCol1, const Vector4& InCol2, const Vector4& InCol3);

	FORCEINLINE void SetIdentity();
	FORCEINLINE Matrix4x4 Tranpose() const;

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
