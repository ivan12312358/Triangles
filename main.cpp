#include <iostream>
#include <vector>
#include "triangles.hpp"

void main_area(geometry::vector& vertex, const geometry::vector& point, int mode);

int main()
{
	size_t size{};
	std::cin >> size;

	std::vector<geometry::vector> 	point(3);
	std::vector<geometry::triangle> v_triangle(size);
	geometry::area prism;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cin >> point[j].x >> point[j].y >> point[j].z;

			prism.main_area(point[j]);
		}

		v_triangle[i] = { point[0],
						  point[1],
						  point[2], i};
	}

	prism.min = prism.min - geometry::vector{0.1, 0.1, 0.1};

	geometry::node tree{prism};

	for (int i = 0; i < v_triangle.size(); i++)
		tree.insert(v_triangle[i]);

	tree.solution();

	return 0;
}
