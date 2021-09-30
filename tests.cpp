#include <iostream>
#include <vector>
#include "triangles.h"

int main()
{
	size_t size{10};

	geometry::triangle v_triangle[size] = { 
											{ {  0,   0,   0}, { 10,   0,   0}, { 0, 10,   0} },
											{ {  0,   0,   0}, { 10,   0,   0}, { 0, 10,   0} },
											{ {  1,   1,   0}, {  1,   2,   0}, { 2,  1,   0} },
											{ {  0,   0,   0}, { 10,   0,   0}, { 5, 10,   0} },
											{ {  0,   0,   0}, { 10,   5,   0}, { 5, 10,   0} },
											{ {-10,   0,   0}, {-10,  10,   0}, { 4,  4,   0} },
											{ {  0, -10,   0}, { 10, -10,   0}, { 5,  -1,  0} },
											{ {  0,   0, -10}, { 10,   0, -10}, { 0, 10, -10} },
											{ {  3,   3,  -1}, { 10,   0,  10}, { 0, 10,  10} },
											{ {  4,   4,   0}, { 10,   0,  10}, { 0, 10,  10} },
										  };

	std::cout << "programme answers:" << std::endl;

	for (int i = 0;	i < size; i++)
	for (int j = 0; j < size; j++)
	{
		if (i != j && v_triangle[i].triangles_intersection(v_triangle[j]))
		{
			std::cout << i << std::endl;
			break;
		}
	}

	std::cout << "true answers:" << std::endl;
	std::cout << "0\n1\n2\n3\n4\n5\n8\n9" << std::endl;

	return 0;
}