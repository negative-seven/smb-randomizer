#include <psapi.h>
#include <vector>
#include <windows.h>

HANDLE openProcess(std::string windowName)
{
	HWND window = FindWindow(0, windowName.c_str());

  	if (window == 0)
	{
  		throw std::runtime_error("Window not found.");
  	}
	else
	{
		DWORD processId;
		GetWindowThreadProcessId(window, &processId);
		HANDLE handle_process = OpenProcess(PROCESS_ALL_ACCESS, false, processId);

		if (!handle_process)
		{
			throw std::runtime_error("Process not found.");
		}
		else
		{
			return handle_process;
		}
	}
  	return 0;
}

void closeProcess(HANDLE handle_process)
{
	CloseHandle(handle_process);
}

int getBaseAddress(HANDLE handle_process, std::string processName)
{
	// I am not sure if I know how this works...

	HMODULE hMods[1024];
	DWORD cbNeeded;
	TCHAR path_module[MAX_PATH];

	if (EnumProcessModules(handle_process, hMods, 1024, &cbNeeded))
	{
		for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			if (GetModuleFileNameEx(handle_process, hMods[i], path_module, sizeof(path_module) / sizeof(TCHAR)))
			{
				std::string path_module_string = path_module;

				if (path_module_string.find(processName) != std::string::npos)
				{
					return (int)hMods[i];
				}
			}
		}
	}

	return -1;
}

int readMemoryInt(HANDLE handle_process, int address, unsigned int dataSize = 4)
{
	int value = 0;

	if (!ReadProcessMemory(handle_process, (LPVOID)address, &value, dataSize, NULL))
	{
		throw std::runtime_error("Could not read from memory.");
	}

	return value;
}

std::vector<char> readMemoryBytes(HANDLE handle_process, int address, unsigned int dataSize = 1)
{
	std::vector<char> values;
	values.resize(dataSize);

	for (unsigned int i = 0; i < dataSize; i++)
	{
		if (!ReadProcessMemory(handle_process, (LPVOID)(address + i), &values[i], 1, NULL))
		{
			throw std::runtime_error("Could not read from memory.");
		}
	}

	return values;
}

std::string readMemoryString(HANDLE handle_process, int address, unsigned int dataSize = -1)
{
	std::string value = "";
	char temp;

	for (unsigned int i = 0; i < dataSize; i++)
	{
		if (!ReadProcessMemory(handle_process, (LPVOID)address, &temp, 1, NULL))
		{
			throw std::runtime_error("Could not read from memory.");
		}

		if (temp == 0)
		{
			break;
		}

		value += temp;
		address++;
	}

	return value;
}

void writeMemoryInt(HANDLE handle_process, int address, int value)
{
	if (!WriteProcessMemory(handle_process, (LPVOID)address, &value, 4, NULL))
	{
		throw std::runtime_error("Could not write to memory.");
	}
}

void writeMemoryBytes(HANDLE handle_process, int address, std::vector<char> values)
{
	for (unsigned int i = 0; i < values.size(); i++)
	{
		if (!WriteProcessMemory(handle_process, (LPVOID)(address + i), &values[i], 1, NULL))
		{
			throw std::runtime_error("Could not write to memory.");
		}
	}
}