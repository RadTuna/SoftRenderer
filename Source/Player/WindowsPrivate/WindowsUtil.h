#pragma once

#include <Windows.h>

namespace WindowsUtil
{
	POINT CurrentMousePos;
	POINT PrevMousePos;

	FORCEINLINE void GetWindowSize(HWND handle, float* width, float* height)
	{
		RECT rect;
		::GetClientRect(handle, &rect);
		*width = static_cast<float>(rect.right - rect.left);
		*height = static_cast<float>(rect.bottom - rect.top);
	}

	FORCEINLINE void Show(HWND handle)
	{
		::ShowWindow(handle, SW_SHOW);
		::SetForegroundWindow(handle);
		::SetFocus(handle);
	}

	FORCEINLINE void CenterWindow(HWND handle)
	{
		// center on parent or screen
		RECT rc, rcOwner, rcWindow;
		HWND ownerHandle = GetDesktopWindow();
		GetWindowRect(ownerHandle, &rcOwner);
		GetWindowRect(handle, &rcWindow);
		CopyRect(&rc, &rcOwner);

		OffsetRect(&rcWindow, -rcWindow.left, -rcWindow.top);
		OffsetRect(&rc, -rc.left, -rc.top);
		OffsetRect(&rc, -rcWindow.right, -rcWindow.bottom);

		::SetWindowPos(handle, HWND_TOP,
			rcOwner.left + (rc.right / 2),
			rcOwner.top + (rc.bottom / 2),
			0, 0, 
			SWP_NOSIZE);
	}

	FORCEINLINE float GetCyclesPerMilliSeconds()
	{
		// Init Query Performance Timer
		LARGE_INTEGER frequency;
		if (!QueryPerformanceFrequency(&frequency))
		{
			return 0.f;
		}

		return (float)(frequency.QuadPart / 1000.f);
	}

	FORCEINLINE long long GetCurrentTimeStamp()
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		return currentTime.QuadPart;
	}

	#define ISPRESSED(KeyCode) (::GetKeyState(KeyCode) & 0x8000) != 0
	enum { VK_KeyA = 0x41, VK_KeyS = 0x53, VK_KeyD = 0x44, VK_KeyW = 0x57 };

	FORCEINLINE void UpdateMousePos()
	{
		PrevMousePos = CurrentMousePos;
		GetCursorPos(&CurrentMousePos);
	}

	FORCEINLINE float GetXAxisInput()
	{
		float OutX;
		OutX = static_cast<float>(CurrentMousePos.x) - static_cast<float>(PrevMousePos.x);

		return OutX * 0.1f;
	}

	FORCEINLINE float GetYAxisInput()
	{
		float OutY;
		OutY = static_cast<float>(CurrentMousePos.y) - static_cast<float>(PrevMousePos.y);

		return OutY * 0.1f;
	}

	FORCEINLINE float GetForwardInput()
	{
		bool isForward = GetAsyncKeyState(VK_KeyW);
		bool isReverse = GetAsyncKeyState(VK_KeyS);
		if (isForward ^ isReverse)
		{
			return isReverse ? -1.0f : 1.0f;
		}
		return 0.0f;
	}

	FORCEINLINE float GetRightInput()
	{
		bool isRight = GetAsyncKeyState(VK_KeyD);
		bool isLeft = GetAsyncKeyState(VK_KeyA);
		if (isRight ^ isLeft)
		{
			return isLeft ? -1.0f : 1.0f;
		}
		return 0.0f;
	}

	FORCEINLINE float GetUpInput()
	{
		bool isUp = GetAsyncKeyState(VK_SPACE);
		bool isDown = GetAsyncKeyState(VK_CONTROL);
		if (isUp ^ isDown)
		{
			return isDown ? -1.0f : 1.0f;
		}
		return 0.0f;
	}

	FORCEINLINE bool GetMouseLBDownInput()
	{
		static bool Prev = false;
		bool Cur = GetAsyncKeyState(VK_LBUTTON);

		// »ó½Â¿§Áö¸¦ °ËÃâ
		if (Prev == false && Cur == true)
		{
			Prev = Cur;
			return true;
		}
		Prev = Cur;

		return false;
	}

	FORCEINLINE bool GetMouseLBInput()
	{
		return GetAsyncKeyState(VK_LBUTTON);
	}

	FORCEINLINE bool GetMouseRBDownInput()
	{
		static bool Prev = false;
		bool Cur = GetAsyncKeyState(VK_RBUTTON);

		// »ó½Â¿§Áö¸¦ °ËÃâ
		if (Prev == false && Cur == true)
		{
			Prev = Cur;
			return true;
		}
		Prev = Cur;

		return false;
	}

	FORCEINLINE bool GetMouseRBInput()
	{
		return GetAsyncKeyState(VK_RBUTTON);
	}

}