#include <iostream>
#include <vector>
#include "triangles.h"

int main()
{
	size_t size{};

	std::cin >> size;
	geometry::triangle* v_triangle = new geometry::triangle[size];

	double x1{}, y1{}, z1{},
		   x2{}, y2{}, z2{},
		   x3{}, y3{}, z3{};

	for(int i = 0; i < size; i++)
	{
		std::cin >> x1 >> y1 >> z1;
		std::cin >> x2 >> y2 >> z2;
		std::cin >> x3 >> y3 >> z3;

		v_triangle[i] = { {x1, y1, z1},
						  {x2, y2, z2},
						  {x3, y3, z3} };
	}

	std::vector<std::vector<int>> res;

	res.resize(size);
	for(int i = 0; i < size; i++)
		res[i].resize(size);

	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			if(i != j)
			{
				res[i][j] = v_triangle[i].intersection_point
						   (v_triangle[j]);
			}

	for(int i = 0; i < size; i++)
		for(int j = 0; j < size; j++)
			if(i != j && res[i][j] == 1)
			{
				res[j][i] = 0;
				std::cout << "(" << i + 1 << ", " << j + 1 << ") ";
			}

	std::cout << std::endl;

	delete[] v_triangle;

	return 0;
}