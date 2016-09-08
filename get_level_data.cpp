#include <string>
#include <vector>

std::vector<std::string> getNormalLevelNames()
{
	const std::vector<int> levelCounts = { 20, 20, 20, 20, 20, 5 };
	const std::string extension = ".smbl";

	std::vector<std::string> levelNames;

	// Worlds 1-6

	for (int i = 0; i < 6; i++)
	{
		for (int j = 1; j < levelCounts[i] + 1; j++)
		{
			levelNames.push_back(std::to_string(i + 1) + "-" + std::to_string(j) + extension);
			levelNames.push_back(std::to_string(i + 1) + "-" + std::to_string(j) + "x" + extension);
		}
	}

	// World 7

	for (int i = 1; i < 21; i++)
	{
		levelNames.push_back("bonus1-" + std::to_string(i) + extension);
		levelNames.push_back("bonus1-" + std::to_string(i) + "x" + extension);
	}

	// Warp zone levels

	for (int i = 1; i < 4; i++)
	{
		levelNames.push_back("hretro" + std::to_string(i) + extension);
	}

	for (int i = 1; i < 8; i++)
	{
		for (int j = 1; j < 4; j++)
		{
			levelNames.push_back("retro" + std::to_string(i) + "-" + std::to_string(j) + extension);
		}
	}

	for (int i = 1; i < 4; i++)
	{
		levelNames.push_back("4bit" + std::to_string(i) + extension);
	}

	for (int i = 2; i < 5; i++)
	{
		for (int j = 1; j < 4; j++)
		{
			levelNames.push_back("4bit" + std::to_string(i) + "-" + std::to_string(j) + extension);
		}
	}

	for (int i = 1; i < 4; i++)
	{
		levelNames.push_back("gameboy" + std::to_string(i) + extension);
	}

	for (int i = 2; i < 5; i++)
	{
		for (int j = 1; j < 4; j++)
		{
			levelNames.push_back("gameboy" + std::to_string(i) + "-" + std::to_string(j) + extension);
		}
	}

	// Glitch levels

	for (int i = 1; i < 7; i++)
	{
		levelNames.push_back("-" + std::to_string(i) + extension);
	}

	return levelNames;
}