/*
120C9 to 120CE -> E8 42 FF FF FF 90

Changes:
fld dword ptr [eax+0x37C]
to:
call SuperMeatBoy.exe+0x12010
nop

12010 to 1201D -> C7 44 24 28 FF FF 7F 7F D9 80 7C 03 00 00

Writes function:
mov [esp+0x28], 7F7FFFFF
fld dword ptr [eax+0x37C]
ret
*/

#include <conio.h>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

#include "get_level_data.cpp"
#include "handle_memory.h"
#include "randomize.h"

int main()
{
	HANDLE handle_process;
	const std::string windowName = "Super Meat Boy";
	const std::string processName = "SuperMeatBoy.exe";
	std::string seed_input;
	unsigned int seed;
	std::vector<std::string> levelNames;
	std::vector<char> temp;
	unsigned int baseAddress, pointer1, pointer2, pointer3;
	unsigned int offset_base = 0x2DA120;
	unsigned int offset1 = 0x18;
	unsigned int offset2 = 0x0;
	unsigned int offset3 = 0x0;
	std::vector<unsigned int> pointers_new;
	std::vector<unsigned int> offsets;
	const unsigned int searchLimit = 0x1000;
	std::vector<char> bytes_old, bytes_new;

	//// Open process

	try
	{
		 handle_process = openProcess(windowName);
	}
	catch (std::exception e)
	{
		std::cout << "Super Meat Boy window not found.\nRestart this program with the game open.\n";
		_getch();
		return 0;
	}

	//// Get seed as input.

	std::cout << "Input seed [\"r\" = random seed]:\n";
	std::cin >> seed_input;
	try
	{
		if (seed_input == "r")
		{
			seed = randomInt();
			std::cout << "Using seed: " << seed << ".\n";
		}

		else
		{
			seed = std::stoul(seed_input);

			if (seed > UINT_MAX || seed_input.substr(0, 1) == "-")
			{
				throw std::invalid_argument("");
			}
		}
	}
	catch (std::invalid_argument)
	{
		std::cout << "Input must be \"r\" or an integer between 0 and 4294967295.\n";
		_getch();
		return 0;
	}
	catch (std::out_of_range)
	{
		std::cout << "Input must be \"r\" or an integer between 0 and 4294967295.\n";
		_getch();
		return 0;
	}

	baseAddress = getBaseAddress(handle_process, processName);

	//// Overwrite part of code for saving level times.

	/*
	Find code at 0x120C9:
	fld dword ptr [eax+0x37C]
	*/

	bytes_old = { '\xD9', '\x80', '\x7C', '\x03', '\x00', '\x00' };
	bytes_new = { '\xE8', '\x42', '\xFF', '\xFF', '\xFF', '\x90' };

	offset_base = 0x120C9;

	temp = readMemoryBytes(handle_process, baseAddress + offset_base, 6);
	if (temp != bytes_old && temp != bytes_new)
	{
		std::cout << "Error while editing memory.\n";
	}
	else
	{
		/*
		Replace code with:
		call SuperMeatBoy.exe+0x12010
		nop
		*/

		writeMemoryBytes(handle_process, baseAddress + offset_base, bytes_new);
	}

	/*
	Find code at 0x120C9:
	ret
	int3  <x13>
	*/

	bytes_old = { '\xC3', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC' };
	bytes_new = { '\xC7', '\x44', '\x24', '\x28', '\xFF', '\xFF', '\x7F', '\x7F', '\xD9', '\x80', '\x7C', '\x03', '\x00', '\x00', '\xC3' };

	offset_base = 0x12010;

	temp = readMemoryBytes(handle_process, baseAddress + offset_base, 15);
	if (temp != bytes_old && temp != bytes_new)
	{
		for (int i = 0; i < temp.size(); i++)
		{
			std::cout << std::hex << (int)(unsigned char)temp[i] << " " << (int)(unsigned char)bytes_old[i] << "\n";
		}

		std::cout << "Error while editing memory.\n";
	}
	else
	{
		/*
		Replace code with:
		mov [esp+0x28], 0x7F7FFFFF
		fld dword ptr [eax+0x37C]
		ret
		*/

		writeMemoryBytes(handle_process, baseAddress + offset_base, bytes_new);
	}

	std::cout << "Steam leaderboard updates disabled...\n";

	//// Overwrite level name pointers

	// Find first level name pointer

	levelNames = getNormalLevelNames();

	offset_base = 0x2DA120;
	offset1 = 0x18;
	offset2 = 0x0;
	offset3 = 0x0;

	pointer1 = readMemoryInt(handle_process, baseAddress + offset_base);
	pointer2 = readMemoryInt(handle_process, pointer1 + offset1);

	while (offset2 < searchLimit) 
	{
		pointer3 = readMemoryInt(handle_process, pointer2 + offset2);

		if (readMemoryString(handle_process, pointer3 + offset3, 7) == "Levels/")
		{
			break;
		}

		offset2 += 0x4;
	}

	// Find all level name pointers

	while (offset2 < searchLimit)
	{
		pointer3 = readMemoryInt(handle_process, pointer2 + offset2);

		for (int i = 0; i < levelNames.size(); i++)
		{
			if (readMemoryString(handle_process, pointer3 + offset3 + 7) == levelNames[i])
			{
				offsets.push_back(offset2);
				pointers_new.push_back(pointer3);
				break;
			}
		}

		if (readMemoryString(handle_process, pointer3 + offset3, 7) != "Levels/")
		{
			break;
		}

		offset2 += 0x4;
	}

	// Write to level name pointers

	pointers_new = randomizeInts(pointers_new, seed);

	for (int i = 0; i < pointers_new.size(); i++)
	{
		writeMemoryInt(handle_process, pointer2 + offsets[i], pointers_new[i]);
	}

	std::cout << "Levels randomised...\n";

	//// Close process

	closeProcess(handle_process);

	std::cout << "Randomization finished.\nPress any key to exit.\n";
	_getch();
	return 0;
}
