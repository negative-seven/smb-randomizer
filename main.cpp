#include <iostream>
#include <vector>
#include <windows.h>

#include "get_level_data.cpp"
#include "handle_memory.cpp"
#include "randomize.cpp"

int main()
{
	HANDLE handle_process;
	const std::string windowName = "Super Meat Boy";
	const std::string processName = "SuperMeatBoy.exe";
	std::vector<std::string> levelNames;
	unsigned int baseAddress, pointer1, pointer2, pointer3;
	const unsigned int offset_base = 0x2DA120;
	const unsigned int offset1 = 0x18;
	unsigned int offset2 = 0x0;
	unsigned int offset3 = 0x0;
	std::vector<unsigned int> newPointers;
	std::vector<unsigned int> offsets;
	const unsigned int searchLimit = 0x1000;

	try
	{
		 handle_process = openProcess(windowName);
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
		std::getchar();
		return 0;
	}

	baseAddress = getBaseAddress(handle_process, processName);
	levelNames = getNormalLevelNames();

	pointer1 = readMemoryInt(handle_process, baseAddress + offset_base);
	pointer2 = readMemoryInt(handle_process, pointer1 + offset1);

	// Find first level name

	while (offset2 < searchLimit) 
	{
		pointer3 = readMemoryInt(handle_process, pointer2 + offset2);

		if (readMemoryString(handle_process, pointer3 + offset3, 7) == "Levels/")
		{
			break;
		}

		offset2 += 0x4;
	}

	// Find all level names

	while (offset2 < searchLimit)
	{
		pointer3 = readMemoryInt(handle_process, pointer2 + offset2);

		for (int i = 0; i < levelNames.size(); i++)
		{
			if (readMemoryString(handle_process, pointer3 + offset3 + 7) == levelNames[i])
			{
				offsets.push_back(offset2);
				newPointers.push_back(pointer3);
				break;
			}
		}

		if (readMemoryString(handle_process, pointer3 + offset3, 7) != "Levels/")
		{
			break;
		}

		offset2 += 0x4;
	}

	newPointers = randomizeInts(newPointers, randomInt());

	for (int i = 0; i < newPointers.size(); i++)
	{
		std::cout << pointer1 + newPointers[i] << " " << offsets[i] << "\n";
		writeMemoryInt(handle_process, pointer2 + offsets[i], newPointers[i]);
	}

	closeProcess(handle_process);

	std::cout << "end";
	std::getchar();
	return 0;
}
