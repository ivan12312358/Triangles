#include <iostream>
#include <vector>
#include "triangles.h"

int main()
{
	size_t size{10};

	geometry::triangle v_triangle[size] = { 
											{ {  0,   0,   0}, { 10,   0,   0}, { 0, 10,   0}, 0},
											{ {  0,   0,   0}, { 10,   0,   0}, { 0, 10,   0}, 1},
											{ {  1,   1,   0}, {  1,   2,   0}, { 2,  1,   0}, 2},
											{ {  0,   0,   0}, { 10,   0,   0}, { 5, 10,   0}, 3},
											{ {  0,   0,   0}, { 10,   5,   0}, { 5, 10,   0}, 4},
											{ {-10,   0,   0}, {-10,  10,   0}, { 4,  4,   0}, 5},
											{ {  0, -10,   0}, { 10, -10,   0}, { 5,  -1,  0}, 6},
											{ {  0,   0, -10}, { 10,   0, -10}, { 0, 10, -10}, 7},
											{ {  3,   3,  -1}, { 10,   0,  10}, { 0, 10,  10}, 8},
											{ {  4,   4,   0}, { 10,   0,  10}, { 0, 10,  10}, 9},
										  };

	geometry::area prism{{-10, -10, -10},
						 { 10,  10,  10}};

	prism.min = prism.min - geometry::vector{0.1, 0.1, 0.1};

	geometry::node tree{prism};

	for (int i = 0; i < size; i++)
		tree.insert(v_triangle[i]);

	std::cout << "programme answers:" << std::endl;

	tree.solution();

	std::cout << "true answers:" << std::endl;
	std::cout << "0\n1\n2\n3\n4\n5\n8\n9" << std::endl;

	return 0;
}