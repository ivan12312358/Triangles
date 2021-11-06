#include <iostream>
#include <cmath>
#include <ctime>

int main()
{
	size_t size{10};
	std::cout << size << std::endl;

	std::srand(std::time(nullptr));

	for (int i = 0; i < size * 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			for (int j = 0; j < 3; j++)
				std::cout << rand() % 200 - 100 << " ";
		
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	return 0;
}