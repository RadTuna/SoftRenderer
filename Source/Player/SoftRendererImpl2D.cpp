
#include "Precompiled.h"
#include "SoftRendererImpl2D.h"
#include "SoftRenderer.h"
#include "MathUtil.h"

SoftRendererImpl2D::SoftRendererImpl2D(SoftRenderer* InOwner)
{
	RSI = InOwner->RSI.get();
	ScreenSize = InOwner->CurrentScreenSize;
	InputManager = InOwner->GetInputManager();
}

SoftRendererImpl2D::~SoftRendererImpl2D()
{
}

void SoftRendererImpl2D::DrawGrid2D()
{
	// Colors to use
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// Render Start
	RSI->Clear(LinearColor::White);

	// Draw Grid Line
	ScreenPoint screenHalfSize = ScreenSize.GetHalf();

	for (int x = screenHalfSize.X; x <= ScreenSize.X; x += Grid2DUnit)
	{
		RSI->DrawFullVerticalLine(x, gridColor);
		if (x > screenHalfSize.X)
		{
			RSI->DrawFullVerticalLine(2 * screenHalfSize.X - x, gridColor);
		}
	}

	for (int y = screenHalfSize.Y; y <= ScreenSize.Y; y += Grid2DUnit)
	{
		RSI->DrawFullHorizontalLine(y, gridColor);
		if (y > screenHalfSize.Y)
		{
			RSI->DrawFullHorizontalLine(2 * screenHalfSize.Y - y, gridColor);
		}
	}

	// Draw World Axis
	RSI->DrawFullHorizontalLine(screenHalfSize.Y, LinearColor::Red);
	RSI->DrawFullVerticalLine(screenHalfSize.X, LinearColor::Green);
}

void SoftRendererImpl2D::DrawLine(const Vector2& StartPoint, const Vector2& EndPoint, const LinearColor& InColor)
{
	ScreenPoint CurrentPoint = ScreenPoint::ToScreenCoordinate(ScreenSize, StartPoint);

	int DeltaX = Math::FloorToInt(Math::Abs(EndPoint.X - StartPoint.X));
	int DeltaY = Math::FloorToInt(Math::Abs(EndPoint.Y - StartPoint.Y));
	int DirX = Math::Sign(EndPoint.X - StartPoint.X);
	int DirY = Math::Sign(EndPoint.Y - StartPoint.Y);

	bool IsSwapped = false;
	if (DeltaY > DeltaX) // 1 ~ 의 기울기
	{
		Math::Swap(DeltaX, DeltaY);
		IsSwapped = true;
	}

	int Judge = 2 * DeltaX - DeltaY;

	for (int CurX = 0; CurX < DeltaX; ++CurX)
	{
		RSI->DrawPoint(CurrentPoint, InColor);

		if (Judge >= 0)
		{
			if (IsSwapped == true)
			{
				CurrentPoint.X += DirX;
			}
			else
			{
				CurrentPoint.Y -= DirY; // 스크린 스페이스의 Y좌표가 반전되어 있음.
			}

			Judge -= 2 * DeltaX;
		}

		if (IsSwapped == true)
		{
			CurrentPoint.Y -= DirY; // 스크린 스페이스의 Y좌표가 반전되어 있음.
		}
		else
		{
			CurrentPoint.X += DirX;
		}

		Judge += 2 * DeltaY;
	}
}

void SoftRendererImpl2D::RenderFrameImpl()
{
	assert(RSI != nullptr && RSI->IsInitialized() && !ScreenSize.HasZero());

	DrawGrid2D();
	DrawLine(Vector2(10.0f, 10.0f), Vector2(100.0f, 50.0f), LinearColor::Green);
	DrawLine(Vector2(10.0f, 10.0f), Vector2(50.0f, 100.0f), LinearColor::Green);

	DrawLine(Vector2(-10.0f, 10.0f), Vector2(-100.0f, 50.0f), LinearColor::Green);
	DrawLine(Vector2(-10.0f, 10.0f), Vector2(-50.0f, 100.0f), LinearColor::Green);

	DrawLine(Vector2(10.0f, -10.0f), Vector2(100.0f, -50.0f), LinearColor::Green);
	DrawLine(Vector2(10.0f, -10.0f), Vector2(50.0f, -100.0f), LinearColor::Green);

	DrawLine(Vector2(-10.0f, -10.0f), Vector2(-100.0f, -50.0f), LinearColor::Green);
	DrawLine(Vector2(-10.0f, -10.0f), Vector2(-50.0f, -100.0f), LinearColor::Green);

	RSI->DrawPoint(Vector2(50.0f, 50.0f), LinearColor::Green);

	RSI->DrawPoint(Vector2::Zero, LinearColor::Blue);
	RSI->DrawPoint(Vector2::Zero + Vector2::UnitX, LinearColor::Blue);
	RSI->DrawPoint(Vector2::Zero - Vector2::UnitX, LinearColor::Blue);
	RSI->DrawPoint(Vector2::Zero + Vector2::UnitY, LinearColor::Blue);
	RSI->DrawPoint(Vector2::Zero - Vector2::UnitY, LinearColor::Blue);
}

void SoftRendererImpl2D::UpdateImpl(float DeltaSeconds)
{
}
