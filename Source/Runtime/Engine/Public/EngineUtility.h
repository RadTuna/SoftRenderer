#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace EngineUtility
{
	template<typename T>
	std::vector<T> SplitString(const std::string& InString, const char Delimiter = ' ');

	template<typename T>
	std::vector<T> SplitString(const std::string& InString, const char Delimiter)
	{
		std::vector<std::string> OutStrings;
		std::string Token;
		std::stringstream StringStream(InString);

		while (std::getline(StringStream, Token, Delimiter))
		{
			OutStrings.emplace_back(static_cast<T>(Token));
		}

		return OutStrings;
	}

	template<>
	std::vector<int> SplitString(const std::string& InString, const char Delimiter)
	{
		std::vector<int> OutStrings;
		std::string Token;
		std::stringstream StringStream(InString);

		while (std::getline(StringStream, Token, Delimiter))
		{
			OutStrings.emplace_back(std::stoi(Token));
		}

		return OutStrings;
	}

	template<>
	std::vector<unsigned int> SplitString(const std::string& InString, const char Delimiter)
	{
		std::vector<unsigned int> OutStrings;
		std::string Token;
		std::stringstream StringStream(InString);

		while (std::getline(StringStream, Token, Delimiter))
		{
			OutStrings.emplace_back(std::stoul(Token));
		}

		return OutStrings;
	}
}
