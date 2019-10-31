
#include "Precompiled.h"
#include "Matrix3x3.h"

Matrix3x3::Matrix3x3()
{
	SetIdentity();
}

Matrix3x3::Matrix3x3(const Vector3& InCol0, const Vector3& InCol1, const Vector3& InCol2)
{
	Cols[0] = InCol0;
	Cols[1] = InCol1;
	Cols[2] = InCol2;
}

Matrix3x3::Matrix3x3(float In00, float In01, float In02, float In10, float In11, float In12, float In20, float In21, float In22)
{
	Cols[0][0] = In00;
	Cols[0][1] = In01;
	Cols[0][2] = In02;

	Cols[1][0] = In10;
	Cols[1][1] = In11;
	Cols[1][2] = In12;

	Cols[2][0] = In20;
	Cols[2][1] = In21;
	Cols[2][2] = In22;
}
