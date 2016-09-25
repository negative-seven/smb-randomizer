#pragma once

#include <vector>
#include <windows.h>

HANDLE openProcess(std::string);
void closeProcess(HANDLE);
int getBaseAddress(HANDLE, std::string);
int readMemoryInt(HANDLE, int, unsigned int dataSize = 4);
std::vector<char> readMemoryBytes(HANDLE, int, unsigned int dataSize = 1);
std::string readMemoryString(HANDLE, int, unsigned int dataSize = -1);
void writeMemoryInt(HANDLE, int, int);
void writeMemoryBytes(HANDLE, int, std::vector<char>);