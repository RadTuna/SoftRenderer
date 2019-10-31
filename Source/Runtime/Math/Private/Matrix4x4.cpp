
#include "Precompiled.h"
#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	SetIdentity();
}

Matrix4x4::Matrix4x4(const Vector4& InCol0, const Vector4& InCol1, const Vector4& InCol2, const Vector4& InCol3)
{
	Cols[0] = InCol0;
	Cols[1] = InCol1;
	Cols[2] = InCol2;
	Cols[3] = InCol3;
}

