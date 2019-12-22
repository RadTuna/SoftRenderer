#pragma once

#include <memory>
#include "WindowsGDI.h"
#include "RenderingSoftwareInterface.h"

#define _WINDOWS_RENDERING_INTERFACE

class WindowsRSI;
class WindowsRSI : public WindowsGDI, public RenderingSoftwareInterface
{
public:

	WindowsRSI() = default;
	~WindowsRSI();

public:

	bool Init(const ScreenPoint& InScreenSize) override;
	void Shutdown() override;
	bool IsInitialized() const override { return IsGDIInitialized; }
	void SetBlendingMode(BlendingModes InNewBlendingMode) override;

	void Clear(const LinearColor& InClearColor) override;
	void BeginFrame() override;
	void BufferSwap() override;

	void DrawPoint(const Vector2& InVectorPos, const LinearColor& InColor) override;
	void DrawPoint(const ScreenPoint& InScreenPos, const LinearColor& InColor) override;

	bool SetDepthPoint(const Vector2& InVectorPos, float InDepth) override;
	bool SetDepthPoint(const ScreenPoint& InScreenPos, float InDepth) override;

	void DrawFullVerticalLine(int InX, const LinearColor& InColor) override;
	void DrawFullHorizontalLine(int InY, const LinearColor& InColor) override;

private:
	void SetPixel(const ScreenPoint& InPos, const LinearColor& InColor);

	BlendingModes BlendingMode = BlendingModes::Opaque;
};
