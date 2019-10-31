
#include "Precompiled.h"
#include "WindowsGDI.h"

WindowsGDI::~WindowsGDI()
{
}

bool WindowsGDI::InitializeGDI(const ScreenPoint& InScreenSize)
{
	ReleaseGDI();

	Handle = ::GetActiveWindow();
	if (Handle == NULL)
	{
		return false;
	}

	if (IsGDIInitialized)
	{
		DeleteObject(DefaultBitmap);
		DeleteObject(DIBitmap);
		ReleaseDC(Handle, ScreenDC);
		ReleaseDC(Handle, MemoryDC);
	}

	ScreenDC = GetDC(Handle);
	if (ScreenDC == NULL)
	{
		return false;
	}

	MemoryDC = CreateCompatibleDC(ScreenDC);
	if (MemoryDC == NULL)
	{
		return false;
	}

	ScreenSize = InScreenSize;

	// Color & Bitmap Setting
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = ScreenSize.X;
	bmi.bmiHeader.biHeight = -ScreenSize.Y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	DIBitmap = CreateDIBSection(MemoryDC, &bmi, DIB_RGB_COLORS, (void**)&ScreenBuffer, NULL, 0);
	if (DIBitmap == NULL)
	{
		return false;
	}

	DefaultBitmap = (HBITMAP)SelectObject(MemoryDC, DIBitmap);
	if (DefaultBitmap == NULL)
	{
		return false;
	}

	// Create Depth Buffer
	CreateDepthBuffer();

	IsGDIInitialized = true;
	return true;
}

void WindowsGDI::ReleaseGDI()
{
	if (IsGDIInitialized)
	{
		DeleteObject(DefaultBitmap);
		DeleteObject(DIBitmap);
		ReleaseDC(Handle, ScreenDC);
		ReleaseDC(Handle, MemoryDC);
	}

	if (DepthBuffer != nullptr)
	{
		delete[] DepthBuffer;
		DepthBuffer = nullptr;
	}

	IsGDIInitialized = false;
}


void WindowsGDI::FillBuffer(Color32 InColor)
{
	if (!IsGDIInitialized || (ScreenBuffer == NULL))
	{
		return;
	}

	Color32* dest = ScreenBuffer;
	UINT totalCount = ScreenSize.X * ScreenSize.Y;
	CopyBuffer<Color32>(ScreenBuffer, &InColor, totalCount);
	return;
}

template <class T>
T* WindowsGDI::CopyBuffer(T* InDst, T* InSrc, int InCount)
{
	if (InCount == 0)
	{
		return NULL;
	}

	if (InCount == 1)
	{
		memcpy(InDst, InSrc, sizeof(T));
	}
	else
	{
		int half = Math::FloorToInt(InCount * 0.5f);
		CopyBuffer<T>(InDst, InSrc, half);
		memcpy(InDst + half, InDst, half * sizeof(T));

		if (InCount % 2)
		{
			memcpy(InDst + (InCount - 1), InSrc, sizeof(T));
		}
	}

	return InDst;
}

LinearColor WindowsGDI::GetPixel(const ScreenPoint& InPos)
{
	if (!IsInScreen(InPos))
	{
		return LinearColor::Error;
	}

	Color32* dest = ScreenBuffer;
	Color32 bufferColor = *(dest + GetScreenBufferIndex(InPos));
	return LinearColor(bufferColor);
}

FORCEINLINE bool WindowsGDI::IsInScreen(const ScreenPoint& InPos) const
{
	int position = GetScreenBufferIndex(InPos);
	if (position < 0 || position >= (ScreenSize.X * ScreenSize.Y))
	{
		return false;
	}

	return true;
}

Color32* WindowsGDI::GetScreenBuffer() const
{
	return ScreenBuffer;
}

void WindowsGDI::SwapBuffer()
{
	if (!IsGDIInitialized)
	{
		return;
	}

	BitBlt(ScreenDC, 0, 0, ScreenSize.X, ScreenSize.Y, MemoryDC, 0, 0, SRCCOPY);
}

void WindowsGDI::SetPixelOpaque(const ScreenPoint& InPos, const LinearColor& InColor)
{
	if (!IsInScreen(InPos))
	{
		return;
	}

	Color32* dest = ScreenBuffer;
	*(dest + GetScreenBufferIndex(InPos)) = InColor.ToColor32();
	return;
}

void WindowsGDI::SetPixelAlphaBlending(const ScreenPoint & InPos, const LinearColor & InColor)
{
	LinearColor bufferColor = GetPixel(InPos);
	if (!IsInScreen(InPos))
	{
		return;
	}

	Color32* dest = ScreenBuffer;
	*(dest + GetScreenBufferIndex(InPos)) = (InColor * InColor.A + bufferColor * (1.f - InColor.A)).ToColor32();
}

void WindowsGDI::CreateDepthBuffer()
{
	DepthBuffer = new float[ScreenSize.X * ScreenSize.Y];
}

void WindowsGDI::ClearDepthBuffer()
{
	if (DepthBuffer != nullptr)
	{
		float* dest = DepthBuffer;
		float defValue = INFINITY;
		UINT totalCount = ScreenSize.X * ScreenSize.Y;
		CopyBuffer<float>(DepthBuffer, &defValue, totalCount);
	}
}

float WindowsGDI::GetDepthBufferValue(const ScreenPoint& InPos) const
{
	if (DepthBuffer == nullptr)
	{
		return INFINITY;
	}

	if (!IsInScreen(InPos))
	{
		return INFINITY;
	}

	return *(DepthBuffer + GetScreenBufferIndex(InPos));
}

void WindowsGDI::SetDepthBufferValue(const ScreenPoint& InPos, float InDepthValue)
{
	if (DepthBuffer == nullptr)
	{
		return;
	}

	if (!IsInScreen(InPos))
	{
		return;
	}

	*(DepthBuffer + GetScreenBufferIndex(InPos)) = InDepthValue;
}