#include <iostream>
#include <vector>
#include "triangles.h"

int main()
{
	// size_t size{};
	// std::cin >> size;

	// std::vector<geometry::vector> 	point(3);
	// std::vector<geometry::triangle> v_triangle(size);

	// for (int i = 0; i < size; i++)
	// {
	// 	for (int j = 0; j < 3; j++)
	// 		std::cin >> point[j].x >> point[j].y >> point[j].z;

	// 	v_triangle[i] = { point[0],
	// 					  point[1],
	// 					  point[2] };
	// }

	// for (int i = 0;	i < size; i++)
	// for (int j = 0; j < size; j++)
	// {
	// 	if (i != j && v_triangle[i].triangles_intersection(v_triangle[j]))
	// 	{
	// 		std::cout << i << std::endl;
	// 		break;
	// 	}
	// }

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

	geometry::area prism { {-10, -10, -10},
						   { 10,  10,  10} };

	geometry::node tree{prism};

	for (int i = 0; i < 10; i++)
	 	tree.insert(v_triangle[i]);

	tree.print();

	return 0;
}