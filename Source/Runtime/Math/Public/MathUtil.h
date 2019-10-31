#pragma once

#include <intrin.h>
#include <math.h>
#include "CoreDefinition.h"

struct Math
{
	static const float PI;
	static const float TwoPI;
	static const float HalfPI;
	static const float InvPI;
	static const int IntMin;
	static const int IntMax;

	static FORCEINLINE int TruncToInt(float InFloat)
	{
		return _mm_cvtt_ss2si(_mm_set_ss(InFloat));
	}

	static FORCEINLINE int RoundToInt(float InFloat)
	{
		// Note: the x2 is to workaround the rounding-to-nearest-even-number issue when the fraction is .5
		return _mm_cvt_ss2si(_mm_set_ss(InFloat + InFloat + 0.5f)) >> 1;
	}

	static FORCEINLINE int FloorToInt(float InFloat)
	{
		return TruncToInt(floorf(InFloat));
	}

	static FORCEINLINE int CeilToInt(float InFloat)
	{
		return TruncToInt(ceilf(InFloat));
	}

	template<class T>
	static constexpr FORCEINLINE T Square(const T InNum)
	{
		return InNum * InNum;
	}

	static constexpr FORCEINLINE float Deg2Rad(float InDegree)
	{
		return InDegree * PI / 180.f;
	}

	template<class T>
	static constexpr FORCEINLINE T Max(const T A, const T B)
	{
		return (A >= B) ? A : B;
	}

	template<class T>
	static constexpr FORCEINLINE T Min(const T A, const T B)
	{
		return (A <= B) ? A : B;
	}

	template<class T>
	static constexpr FORCEINLINE T Abs(const T A)
	{
		return (A >= (T)0) ? A : -A;
	}

	template<class T>
	static constexpr FORCEINLINE T Clamp(const T X, const T Min, const T Max)
	{
		return X < Min ? Min : X < Max ? X : Max;
	}

	static FORCEINLINE void GetSinCos(float& OutSin, float& OutCos, float InDegree)
	{
		if (InDegree == 0.f)
		{
			OutSin = 0.f;
			OutCos = 1.f;
		}
		else if (InDegree == 90.f)
		{
			OutSin = 1.f;
			OutCos = 0.f;
		}
		else if (InDegree == 180.f)
		{
			OutSin = 0.f;
			OutCos = -1.f;
		}
		else if (InDegree == 270.f)
		{
			OutSin = -1.f;
			OutCos = 0.f;
		}
		else
		{
			Math::SinCos(&OutSin, &OutCos, Math::Deg2Rad(InDegree));
		}
	}

	static FORCEINLINE void SinCos(float* ScalarSin, float* ScalarCos, float Value)
	{
		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		float quotient = (Math::InvPI * 0.5f)*Value;
		if (Value >= 0.0f)
		{
			quotient = (float)((int)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int)(quotient - 0.5f));
		}
		float y = Value - (2.0f* Math::PI)*quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		float sign;
		if (y > Math::HalfPI)
		{
			y = PI - y;
			sign = -1.0f;
		}
		else if (y < -Math::HalfPI)
		{
			y = -PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		// 11-degree minimax approximation
		*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*ScalarCos = sign * p;
	}
};

