#include <string>

#include "utils.h"

std::string limitInputbox(std::string string)
{
	// Remove non-numerical characters.

	for (int i = string.length() - 1; i >= 0; i--)
	{
		if (string[i] < '0' || string[i] > '9')
		{
			string.erase(i);
		}
	}

	// Set to max value if above max value.

	try
	{
		std::stoul(string);
	}
	catch (std::out_of_range)
	{
		string = std::to_string(UINT_MAX);
	}
	catch (std::invalid_argument) {}

	return string;
}