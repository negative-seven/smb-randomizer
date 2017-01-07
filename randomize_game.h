#pragma once

#include <windows.h>

#define RANDOMIZE_OK (0 << 16)
#define RANDOMIZE_NOGAME (1 << 16)
#define RANDOMIZE_INVALIDDATA (2 << 16)

int randomize_game(unsigned int seed);