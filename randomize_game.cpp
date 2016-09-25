#include "get_level_data.h"
#include "handle_memory.h"
#include "randomize.h"

void randomize_game(unsigned int seed)
{
	HANDLE handle_process;
	const std::string windowName = "Super Meat Boy";
	const std::string processName = "SuperMeatBoy.exe";
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
		MessageBox(NULL, "Super Meat Boy window not found.\nTry again with the game open.", "Error", MB_ICONEXCLAMATION);
		return;
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
		MessageBox(NULL, "Error while editing memory.", "Error", MB_ICONEXCLAMATION);
		return;
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
	Find code at 0x12010:
	ret
	int3  <x13>
	*/

	bytes_old = { '\xC3', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC' };
	bytes_new = { '\xC7', '\x44', '\x24', '\x28', '\x00', '\x00', '\x80', '\x47', '\xD9', '\x80', '\x7C', '\x03', '\x00', '\x00', '\xC3' };

	offset_base = 0x12010;

	temp = readMemoryBytes(handle_process, baseAddress + offset_base, 15);
	if (temp != bytes_old && temp != bytes_new)
	{
		MessageBox(NULL, "Error while editing memory.", "Error", MB_ICONEXCLAMATION);
		return;
	}
	else
	{
		/*
		Replace code with:
		mov [esp+0x28], 0x47800000
		fld dword ptr [eax+0x37C]
		ret
		*/

		writeMemoryBytes(handle_process, baseAddress + offset_base, bytes_new);
	}

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

		for (unsigned int i = 0; i < levelNames.size(); i++)
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

	for (unsigned int i = 0; i < pointers_new.size(); i++)
	{
		writeMemoryInt(handle_process, pointer2 + offsets[i], pointers_new[i]);
	}

	//// Close process

	closeProcess(handle_process);

	MessageBox(NULL, "Randomization finished.", "Success", MB_OK);
}