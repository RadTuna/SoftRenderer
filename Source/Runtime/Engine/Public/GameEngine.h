#pragma once

#include <string>
#include <map>

class GameEngine
{
public:
	GameEngine() = default;

public:
	bool IsInitialized() const { return IsEngineInitialized; }
	void Init();
	void Shutdown();

private:
	bool IsEngineInitialized = false;
};

