
#include "Precompiled.h"
#include "Math.h"
#include "LinearColor.h"

const float LinearColor::OneOver255 = 1.f / 255.f;
const LinearColor LinearColor::Error(1.f, 0.f, 1.f, 1.f);
const LinearColor LinearColor::White(1.f, 1.f, 1.f, 1.f);
const LinearColor LinearColor::Black(0.f, 0.f, 0.f, 1.f);
const LinearColor LinearColor::Red(1.f, 0.f, 0.f, 1.f);
const LinearColor LinearColor::Green(0.f, 1.f, 0.f, 1.f);
const LinearColor LinearColor::Blue(0.f, 0.f, 1.f, 1.f);
const LinearColor LinearColor::Yellow(1.f, 1.f, 0.f, 1.f);


LinearColor::LinearColor(const Color32& InColor32, const bool bSRGB)
{
	R = float(InColor32.R) * OneOver255;
	G = float(InColor32.G) * OneOver255;
	B = float(InColor32.B) * OneOver255;
	A = float(InColor32.A) * OneOver255;
}

Color32 LinearColor::ToColor32(const bool bSRGB) const
{
	float FloatR = Math::Clamp(R, 0.f, 1.f);
	float FloatG = Math::Clamp(G, 0.f, 1.f);
	float FloatB = Math::Clamp(B, 0.f, 1.f);
	float FloatA = Math::Clamp(A, 0.f, 1.f);

	return Color32(
		(int)(FloatR * 255.999f),
		(int)(FloatG * 255.999f),
		(int)(FloatB * 255.999f),
		(int)(FloatA * 255.999f)
	);
}

