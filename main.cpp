#include <iostream>
#include <vector>
#include "triangles.h"

void main_area(geometry::vector& vertex, const geometry::vector& point, int mode);

int main()
{
	size_t size{};
	std::cin >> size;

	std::vector<geometry::vector> 	point{3};
	std::vector<geometry::triangle> v_triangle{size};
	geometry::area prism;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cin >> point[j].x >> point[j].y >> point[j].z;

			main_area(prism.min, point[j], 0);
			main_area(prism.max, point[j], 1);
		}

		v_triangle[i] = { point[0],
						  point[1],
						  point[2], i};
	}

	geometry::node tree{prism};

	for (int i = 0; i < v_triangle.size(); i++)
		tree.insert(v_triangle[i]);

	tree.solution();

	return 0;
}

void main_area(geometry::vector& vertex, const geometry::vector& point, int mode)
{
	if (!vertex.is_valid())
		vertex = point;
	else if (mode == 0)
	{
		vertex.x = vertex.x < point.x ? vertex.x : point.x;
		vertex.y = vertex.y < point.y ? vertex.y : point.y;
		vertex.z = vertex.z < point.z ? vertex.z : point.z;
	}
	else if (mode == 1)
	{
		vertex.x = vertex.x > point.x ? vertex.x : point.x;
		vertex.y = vertex.y > point.y ? vertex.y : point.y;
		vertex.z = vertex.z > point.z ? vertex.z : point.z;
	}
}
