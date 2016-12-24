#include "get_level_data.h"
#include "handle_memory.h"
#include "randomize.h"

void randomize_levels(unsigned int seed)
{
	HANDLE handle_process;
	const std::string windowName = "Super Meat Boy";
	const std::string processName = "SuperMeatBoy.exe";
	std::vector<std::string> levelNames;
	std::vector<char> temp;
	unsigned int baseAddress, pointer1, pointer2, pointer3, offset_temp;
	std::vector<unsigned int> pointers_new;
	std::vector<unsigned int> offsets;
	std::vector<char> bytes_original, bytes_new;

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

	//// Overwrite asm for saving level times.

	/*
	Replace asm at SMB+0x120D6:

	jne SMB+0x12139

	with:

	nop
	nop
	*/

	bytes_original = { '\x75', '\x61' };
	bytes_new = { '\x90', '\x90' };

	temp = readMemoryBytes(handle_process, baseAddress + 0x120D6, 2);
	if (temp == bytes_original)
	{
		writeMemoryBytes(handle_process, baseAddress + 0x120D6, bytes_new);
	}
	else if (temp != bytes_new)
	{
		MessageBox(NULL, "Error while editing memory.", "Error", MB_ICONEXCLAMATION);
		return;
	}

	//// Overwrite level name pointers

	// Find first level name pointer

	levelNames = getNormalLevelNames();

	pointer1 = readMemoryInt(handle_process, baseAddress + 0x2DA120);
	pointer2 = readMemoryInt(handle_process, pointer1 + 0x18);

	offset_temp = 0x0;

	while (offset_temp < 0x1000) // Limit search
	{
		pointer3 = readMemoryInt(handle_process, pointer2);

		if (readMemoryString(handle_process, pointer3, 7) == "Levels/")
		{
			break;
		}

		offset_temp += 0x4;
	}

	// Find all level name pointers

	while (offset_temp < 0x1000) // Limit search
	{
		pointer3 = readMemoryInt(handle_process, pointer2);

		for (unsigned int i = 0; i < levelNames.size(); i++)
		{
			if (readMemoryString(handle_process, pointer3 + 7) == levelNames[i])
			{
				offsets.push_back(offset_temp);
				pointers_new.push_back(pointer3);
				break;
			}
		}

		if (readMemoryString(handle_process, pointer3, 7) != "Levels/")
		{
			break;
		}

		offset_temp += 0x4;
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

void randomize_chars() // Not useable!
{
	HANDLE handle_process;
	const std::string windowName = "Super Meat Boy";
	const std::string processName = "SuperMeatBoy.exe";
	unsigned int baseAddress;
	std::vector<char> bytes_original, bytes_new, temp;

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

	//// Randomize played character

	/*
	Replace asm at SMB+0x1252F:

	mov eax, [esi+0x2E4]
	
	with:

	call SMB+0x12650
	nop
	*/

	bytes_original = { '\x8B', '\x86', '\xE4', '\x02', '\x00', '\x00' };
	bytes_new = { '\xE8', '\x1C', '\x01', '\x00', '\x00', '\x90' };

	temp = readMemoryBytes(handle_process, baseAddress + 0x120D6, 2);
	if (temp == bytes_original)
	{
		writeMemoryBytes(handle_process, baseAddress + 0x120D6, bytes_new);
	}
	else if (temp != bytes_new)
	{
		MessageBox(NULL, "Error while editing memory.", "Error", MB_ICONEXCLAMATION);
		return;
	}
}