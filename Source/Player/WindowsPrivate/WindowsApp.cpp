
#include "Precompiled.h"
#include "WindowsRSI.h"
#include "WindowsPlayer.h"
#include "SoftRenderer.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	ScreenPoint defScreenSize(800, 600);
	SoftRenderer renderer(new WindowsRSI());
	WindowsPlayer::gOnResizeFunc = [&renderer](const ScreenPoint& InNewScreenSize) { 
		if (InNewScreenSize.HasZero()) {
			return;
		}
		renderer.OnResize(InNewScreenSize); 
	};
	renderer.PerformanceInitFunc = WindowsUtil::GetCyclesPerMilliSeconds;
	renderer.PerformanceMeasureFunc = WindowsUtil::GetCurrentTimeStamp;
	renderer.GetInputManager().GetXAxis = WindowsUtil::GetXAxisInput;
	renderer.GetInputManager().GetYAxis = WindowsUtil::GetYAxisInput;
	renderer.GetInputManager().SpacePressed = WindowsUtil::SpacePressedInput;

	if (!WindowsPlayer::Create(hInstance, defScreenSize))
	{
		return -1;
	}

	WindowsUtil::Show(WindowsPlayer::gHandle);
	WindowsUtil::CenterWindow(WindowsPlayer::gHandle);

	static float previousTimer = 0.f;
	static float updatePeriod = 500.f;
	while (WindowsPlayer::Tick())
	{
		renderer.OnTick();

		float currentTime = renderer.GetElapsedTime();
		if (currentTime - previousTimer > updatePeriod)
		{
			float frameFPS = renderer.GetFrameFPS();
			WindowsPlayer::SetWindowsStatTitle(frameFPS);
			previousTimer = currentTime;
		}
	}

	renderer.Shutdown();
	WindowsPlayer::Destroy();
	return 0;
}
