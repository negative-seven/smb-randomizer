#include "get_level_data.h"
#include "global.h"
#include "handle_memory.h"
#include "randomize.h"

#define RANDOMIZE_OK 0
#define RANDOMIZE_NOGAME 1
#define RANDOMIZE_INVALIDDATA 2

/*
Function returns:
Randomization successful - RANDOMIZE_OK
Window not found - RANDOMIZE_NOGAME
Invalid data found - RANDOMIZE_INVALIDDATA
*/

int randomize_levels(HANDLE handle_game, unsigned int seed)
{
	std::vector<char> bytes_original, bytes_new;
	std::vector<char> temp;
	std::vector<unsigned int> pointers_new;
	std::vector<unsigned int> offsets;

	const unsigned int baseAddress = getBaseAddress(handle_game, "SuperMeatBoy.exe");
	unsigned int pointer1, pointer2, pointer3, offset_temp;

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

	temp = readMemoryBytes(handle_game, baseAddress + 0x120D6, bytes_original.size());
	if (temp == bytes_original)
	{
		writeMemoryBytes(handle_game, baseAddress + 0x120D6, bytes_new);
	}
	else if (temp != bytes_new)
	{
		MessageBox(NULL, "Error while editing memory.", "Error", MB_ICONEXCLAMATION);
		return RANDOMIZE_INVALIDDATA;
	}

	//// Overwrite level name pointers

	// Find first level name pointer

	const std::vector<std::string> levelNames = getNormalLevelNames();

	pointer1 = readMemoryInt(handle_game, baseAddress + 0x2DA120);
	pointer2 = readMemoryInt(handle_game, pointer1 + 0x18);

	offset_temp = 0x0;

	while (offset_temp < 0x1000) // Limit search
	{
		pointer3 = readMemoryInt(handle_game, pointer2);

		if (readMemoryString(handle_game, pointer3, 7) == "Levels/")
		{
			break;
		}

		offset_temp += 0x4;
	}

	// Find all level name pointers

	while (offset_temp < 0x1000) // Limit search
	{
		pointer3 = readMemoryInt(handle_game, pointer2);

		for (unsigned int i = 0; i < levelNames.size(); i++)
		{
			if (readMemoryString(handle_game, pointer3 + 7) == levelNames[i])
			{
				offsets.push_back(offset_temp);
				pointers_new.push_back(pointer3);
				break;
			}
		}

		if (readMemoryString(handle_game, pointer3, 7) != "Levels/")
		{
			break;
		}

		offset_temp += 0x4;
	}

	// Write to level name pointers

	pointers_new = randomizeInts(pointers_new, seed);

	for (unsigned int i = 0; i < pointers_new.size(); i++)
	{
		writeMemoryInt(handle_game, pointer2 + offsets[i], pointers_new[i]);
	}

	return RANDOMIZE_OK;
}

int randomize_chars(HANDLE handle_game) // Not useable!
{
	std::vector<char> bytes_original, bytes_new;
	int temp_int;
	std::vector<char> temp_chars;

	const unsigned int baseAddress = getBaseAddress(handle_game, "SuperMeatBoy.exe");

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

	temp_chars = readMemoryBytes(handle_game, baseAddress + 0x1252F, bytes_original.size());
	if (temp_chars == bytes_original)
	{
		writeMemoryBytes(handle_game, baseAddress + 0x1252F, bytes_new);
	}
	else if (temp_chars != bytes_new)
	{
		MessageBox(NULL, "Error while editing memory.", "Error", MB_ICONEXCLAMATION);
		return RANDOMIZE_INVALIDDATA;
	}

	/*
	Replace asm at SMB+0x12650:

	ret
	int 3 <x15>
	ret
	int 3 <x9>

	with:

	push ecx
	push edx
	push 0x0
	push 0x88
	call SMB+47D0
	movzx eax, byte [eax+SMB+0x300000]
	pop edx
	pop edx
	pop edx
	pop ecx
	ret
	*/

	bytes_original = { '\xC3', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xC3', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC', '\xCC' };
	bytes_new = { '\x51', '\x52', '\x6A', '\x00', '\x6A', '\x70', '\xE8', '\x75', '\x21', '\xFF', '\xFF', '\x0F', '\xB6', '\x80', /* bytes inserted later */ '\x5A', '\x5A', '\x5A', '\x59', '\xC3' };
	temp_int = baseAddress + 0x300000;
	temp_chars = {};
	for (int i = 0; i < 4; i++)
	{
		temp_chars.push_back(temp_int % 0x100);
		temp_int /= 0x100;
	}
	bytes_new.insert(bytes_new.begin() + 14, temp_chars.begin(), temp_chars.end());

	temp_chars = readMemoryBytes(handle_game, baseAddress + 0x12650, bytes_original.size());
	if (temp_chars == bytes_original)
	{
		writeMemoryBytes(handle_game, baseAddress + 0x12650, bytes_new);
	}
	else if (temp_chars != bytes_new)
	{
		MessageBox(NULL, "Error while editing memory.", "Error", MB_ICONEXCLAMATION);
		return RANDOMIZE_INVALIDDATA;
	}

	/*
	Replace data at SMB+0x300000

	These are the characters that will be randomly chosen from. More info in characters.txt.
	*/

	bytes_original.assign(112, '\x00');
	bytes_new = { '\x00', '\x01', '\x02', '\x03', '\x05', '\x06', '\x19', '\x1A', '\x0A', '\x0A', '\x0A', '\x0A', '\x0A', '\x0A', '\x0A', '\x0A', '\x0B', '\x0B', '\x0B', '\x0B', '\x0B', '\x0B', '\x0B', '\x0B', '\x0C', '\x0C', '\x0C', '\x0C', '\x0C', '\x0C', '\x0C', '\x0C', '\x0D', '\x0D', '\x0D', '\x0D', '\x17', '\x17', '\x17', '\x17', '\x10', '\x10', '\x10', '\x10', '\x10', '\x10', '\x10', '\x10', '\x12', '\x12', '\x12', '\x12', '\x12', '\x12', '\x12', '\x12', '\x13', '\x13', '\x13', '\x13', '\x13', '\x13', '\x13', '\x13', '\x14', '\x14', '\x14', '\x14', '\x14', '\x14', '\x14', '\x14', '\x15', '\x15', '\x15', '\x15', '\x15', '\x15', '\x15', '\x15', '\x16', '\x16', '\x16', '\x16', '\x16', '\x16', '\x16', '\x16', '\x18', '\x18', '\x18', '\x18', '\x18', '\x18', '\x18', '\x18', '\x1B', '\x1B', '\x1B', '\x1B', '\x1B', '\x1B', '\x1B', '\x1B', '\x1D', '\x1D', '\x1D', '\x1D', '\x1D', '\x1D', '\x1D', '\x1D' };

	temp_chars = readMemoryBytes(handle_game, baseAddress + 0x300000, bytes_original.size());
	if (temp_chars == bytes_original)
	{
		writeMemoryBytes(handle_game, baseAddress + 0x300000, bytes_new);
	}
	else if (temp_chars != bytes_new)
	{
		MessageBox(NULL, "Error while editing memory.", "Error", MB_ICONEXCLAMATION);
		return RANDOMIZE_INVALIDDATA;
	}
	
	return RANDOMIZE_OK;
}

int randomize_game(unsigned int seed)
{
	HANDLE handle_game;
	int temp;

	try
	{
		handle_game = openProcess("Super Meat Boy");
	}
	catch (std::exception e)
	{
		MessageBox(NULL, "Super Meat Boy window not found.\nTry again with the game open.", "Error", MB_ICONEXCLAMATION);
		return RANDOMIZE_NOGAME;
	}

	if (settings_randlevels)
	{
		temp = randomize_levels(handle_game, seed);

		if (temp != RANDOMIZE_OK)
		{
			return temp;
		}
	}

	if (settings_randchars)
	{
		temp = randomize_chars(handle_game);

		if (temp != RANDOMIZE_OK)
		{
			return temp;
		}
	}

	return RANDOMIZE_OK;
}