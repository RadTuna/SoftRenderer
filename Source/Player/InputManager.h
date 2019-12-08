#pragma once

#include <functional>

struct InputManager
{
	// Axis Input
	std::function<float()> GetXAxis;
	std::function<float()> GetYAxis;
	std::function<float()> MoveForward;
	std::function<float()> MoveRight;
	std::function<float()> MoveUp;
	std::function<bool()> MouseLBDown;
	std::function<bool()> MouseLB;
	std::function<bool()> MouseRBDown;
	std::function<bool()> MouseRB;
};
