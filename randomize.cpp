#include <random>
#include <time.h>
#include <vector>

int randomInt()
{
	std::mt19937 randGen(time(NULL));
	return randGen();
}

std::vector<unsigned int> randomizeInts(std::vector<unsigned int> ints, int seed)
{
	unsigned int intsLeft = ints.size();
	unsigned int rand;
	std::vector<unsigned int> randomizedInts;
	std::mt19937 randGen(seed);

	while (intsLeft > 0)
	{
		unsigned int comparison = (unsigned int)(pow(2, 32) / intsLeft) * intsLeft;
		rand = randGen();

		if (rand < comparison || comparison == 0)
		{
			rand %= intsLeft;
			randomizedInts.push_back(ints[rand]);
			ints.erase(ints.begin() + rand);
			intsLeft--;
		}
	}

	return randomizedInts;
}