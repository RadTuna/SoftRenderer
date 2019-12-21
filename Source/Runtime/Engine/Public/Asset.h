#pragma once

#include <string>

class Asset
{
public:

	explicit Asset() noexcept = default;
	virtual ~Asset() noexcept = default;

	virtual void Load(const std::string& InPath) = 0;

private:
};
