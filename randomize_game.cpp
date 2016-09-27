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

	//// Overwrite code for saving level times.

	/*
	Find code at 0x120D6:
	jne SuperMeatBoy.exe+0x12139
	*/

	bytes_old = { '\x75', '\x61' };
	bytes_new = { '\x90', '\x90' };

	offset_base = 0x120D6;

	temp = readMemoryBytes(handle_process, baseAddress + offset_base, 2);
	if (temp != bytes_old && temp != bytes_new)
	{
		MessageBox(NULL, "Error while editing memory.", "Error", MB_ICONEXCLAMATION);
		return;
	}
	else
	{
		/*
		Replace code with:
		nop
		nop
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