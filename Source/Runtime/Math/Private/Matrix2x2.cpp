
#include "Precompiled.h"
#include "Matrix2x2.h"

const Matrix2x2 Matrix2x2::Identity(Vector2::UnitX, Vector2::UnitY);

Matrix2x2::Matrix2x2()
{
	*this = Matrix2x2::Identity;
}

Matrix2x2::Matrix2x2(const Vector2& InCol0, const Vector2& InCol1)
{
	Cols[0] = InCol0;
	Cols[1] = InCol1;
}

Matrix2x2::Matrix2x2(float In00, float In01, float In10, float In11)
{
	Cols[0][0] = In00;
	Cols[0][1] = In01;

	Cols[1][0] = In10;
	Cols[1][1] = In11;
}

