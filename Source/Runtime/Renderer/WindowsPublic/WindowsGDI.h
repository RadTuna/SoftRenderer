
#pragma once

#include <windows.h>
#include <functional>
#include "ScreenPoint.h"
#include "LinearColor.h"

class WindowsGDI
{
public:
	WindowsGDI() = default;
	~WindowsGDI();

public:
	bool InitializeGDI(const ScreenPoint& InScreenSize);
	void ReleaseGDI();

	void FillBuffer(Color32 InColor);

	LinearColor GetPixel(const ScreenPoint& InPos);
	//std::function<void(const ScreenPoint &, const LinearColor &)> SetPixel;

	void SetPixelOpaque(const ScreenPoint& InPos, const LinearColor& InColor);
	void SetPixelAlphaBlending(const ScreenPoint& InPos, const LinearColor& InColor);

	void CreateDepthBuffer();
	void ClearDepthBuffer();
	float GetDepthBufferValue(const ScreenPoint& InPos) const;
	void SetDepthBufferValue(const ScreenPoint& InPos, float InDepthValue);

	Color32* GetScreenBuffer() const;

	void SwapBuffer();

protected:
	bool IsGDIInitialized = false;

	FORCEINLINE bool IsInScreen(const ScreenPoint& InPos) const;
	FORCEINLINE int GetScreenBufferIndex(const ScreenPoint& InPos) const
	{
		return InPos.Y * ScreenSize.X + InPos.X;
	}

	template <class T>
	T* CopyBuffer(T* InDst, T* InSrc, int InCount);

	HWND Handle = 0;
	HDC	ScreenDC = 0, MemoryDC = 0;
	HBITMAP DefaultBitmap = 0, DIBitmap = 0;

	Color32* ScreenBuffer = nullptr;
	float* DepthBuffer = nullptr;

	ScreenPoint ScreenSize;
};