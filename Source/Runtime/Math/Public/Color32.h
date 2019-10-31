#pragma once

#include "CoreDefinition.h"
#include "MathUtil.h"

struct Color32
{
public:

	Color32() { R = G = B = A = 0; }
	constexpr FORCEINLINE Color32(BYTE InR, BYTE InG, BYTE InB, BYTE InA = 255) : B(InB), G(InG), R(InR), A(InA) { }
	FORCEINLINE explicit Color32(unsigned long InColor)
	{
		GetColorRef() = InColor;
	}
	const unsigned long& GetColorRef() const { return *((unsigned long*)this); }
	unsigned long& GetColorRef() { return *((unsigned long*)this); }

	FORCEINLINE bool operator==(const Color32& InC) const;
	FORCEINLINE bool operator!=(const Color32& InC) const;
	FORCEINLINE void operator+=(const Color32& InC);

	static const Color32 Error;

public:
	// Little Endian Order
	union { struct { BYTE B, G, R, A; }; BYTE Dummy; };

};

FORCEINLINE bool Color32::operator==(const Color32& InC) const
{
	return GetColorRef() == InC.GetColorRef();
}

FORCEINLINE bool Color32::operator!=(const Color32& InC) const
{
	return GetColorRef() != InC.GetColorRef();
}

FORCEINLINE void Color32::operator+=(const Color32& InC)
{
	R = (BYTE)Math::Clamp((BYTE)R + (BYTE)InC.R, 0, 255);
	G = (BYTE)Math::Clamp((BYTE)G + (BYTE)InC.G, 0, 255);
	B = (BYTE)Math::Clamp((BYTE)B + (BYTE)InC.B, 0, 255);
	A = (BYTE)Math::Clamp((BYTE)A + (BYTE)InC.A, 0, 255);
}

