#include <iostream>
#include <vector>
#include "triangles.h"

int main()
{
	size_t size{};
	std::cin >> size;

	geometry::triangle* v_triangle = new geometry::triangle[size];

	std::vector<geometry::vector> point;
	point.resize(3);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < 3; j++)
			std::cin >> point[j].x >> point[j].y >> point[j].z;

		v_triangle[i] = { point[0],
						  point[1],
						  point[2] };
	}

	for (int i = 0;	i < size; i++)
	for (int j = 0; j < size; j++)
	{
		if (i != j && v_triangle[i].triangles_intersection(v_triangle[j]))
		{
			std::cout << i << std::endl;
			break;
		}
	}

	delete[] v_triangle;

	return 0;
}