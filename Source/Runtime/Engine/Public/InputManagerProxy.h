#pragma once

#include <functional>

namespace InputManagerProxy
{
	// Axis Input
    extern std::function<float()> GetXAxis;
	extern std::function<float()> GetYAxis;
	extern std::function<float()> MoveForward;
	extern std::function<float()> MoveRight;
	extern std::function<float()> MoveUp;
	extern std::function<bool()> MouseLBDown;
	extern std::function<bool()> MouseLB;
	extern std::function<bool()> MouseRBDown;
	extern std::function<bool()> MouseRB;
};
