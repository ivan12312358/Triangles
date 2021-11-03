#include <iostream>
#include <unordered_set>
#include <set>
#include <cmath>
#include <vector>

#ifndef H_TRIANGLES
#define H_TRIANGLES

namespace geometry
{
const double epsilon = 0.000001;

bool is_equal(double a, double b) { return fabs(a - b) <= epsilon; }

//-----------------------------------------------------------------------------------------------------------------

struct vector
{
	double x, y, z;

	vector(double x = NAN, double y = NAN, double z = NAN):
		   x{x}, y{y}, z{z} {}

	vector operator*  (double alpha) 	    const { return vector {x * alpha,    y * alpha,    z * alpha};   }	
	vector operator&  (const vector& other) const { return vector {x * other.x,  y * other.y,  z * other.z}; }
	vector operator+  (const vector& other) const { return vector {x + other.x,  y + other.y,  z + other.z}; }
	vector operator-  (const vector& other) const { return vector {x - other.x,  y - other.y,  z - other.z}; }
	vector operator^  (const vector& other)	const { return vector {{y * other.z - z * other.y},
																   {z * other.x - x * other.z},
																   {x * other.y - y * other.x}}; }

	double operator*  (const vector& other) const { return 	       x * other.x + y * other.y + z * other.z;  }
	double operator() () 					const { return 		   std::sqrt(x * x + y * y + z * z); }
	bool   operator<  (const vector& other) const { return x <  other.x && y <  other.y && z <  other.z; }
	bool   operator<= (const vector& other) const { return x <= other.x && y <= other.y && z <= other.z; }
	bool   operator== (const vector& other) const { return is_equal(x, other.x) && is_equal(y, other.y) && is_equal(z, other.z);}
	bool   operator!= (const vector& other) const { return !(*this == other); }
	bool   is_valid() 						const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z); }
	operator bool () const { return *this != vector{0, 0, 0}; }

	bool belong_vector(const vector& min, const vector& max) const
	{
		vector r1{*this - min}, r2{max - min}, r3{*this - max};

		return is_equal(r1() + r3(), r2());
	}
};

//-----------------------------------------------------------------------------------------------------------------

vector operator* (double alpha, const vector& other) { return vector(other * alpha); }

//-----------------------------------------------------------------------------------------------------------------

double determinant(const vector& a, const vector& b, const vector& c)
{
	return a.x * b.y * c.z - a.x  * b.z * c.y +
		   a.y * b.z * c.x - a.y  * b.x * c.z +
		   a.z * b.x * c.y - a.z  * b.y * c.x;
}

//-----------------------------------------------------------------------------------------------------------------

bool belong_triangle(const vector& p, const vector& a, const vector& b, const vector& c)
{
	vector r1 {a - p}, r2 {b - p}, r3 {c - p};

	if (is_equal(r1(), 0) || is_equal(r2(), 0) || is_equal(r3(), 0))
		return 1;

	double alpha = acos( (r1 * r2) / (r1() * r2()) );
	double gamma = acos( (r2 * r3) / (r2() * r3()) );
	double beta  = acos( (r3 * r1) / (r3() * r1()) );

	if (is_equal(alpha + beta + gamma, 2 * acos(-1)))
		return 1;

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------

struct plane
{
	double A, B, C, D;
	vector n;

    plane(): A{NAN}, B{NAN}, C{NAN}, D{NAN}, n{} {};

	plane(const plane& other): 
		A{other.A}, B{other.B}, C{other.C}, D{other.D}, n{other.n} {}

	plane(const vector& a, const vector& b, const vector& c)
	{
		vector r1 {b - a}, r2 {c - a};

		A = r1.y * r2.z - r2.y * r1.z;
		B = r2.x * r1.z - r1.x * r2.z;
		C = r1.x * r2.y - r2.x * r1.y;
		n = {A, B, C};
		D = -(a * n);
	};
};

//-----------------------------------------------------------------------------------------------------------------

struct edge
{
	vector a, b, p;

	edge() = default;

	edge(const vector& a, const vector& b): a{a}, b{b}, p{} {};

	int intersect_plane(const plane& plane)
	{
		double a_halfplane = plane.n * a + plane.D,
			   b_halfplane = plane.n * b + plane.D;

		if (is_equal(a_halfplane, 0)) 
			p = a;
		else if (is_equal(a_halfplane, 0)) 
			p = b;
		else if (a_halfplane * b_halfplane < 0)
		{
			vector c {b - a};
			double lambda = - (plane.n * a + plane.D) / (plane.n * c);

			p = { a.x + c.x * lambda,
		  		  a.y + c.y * lambda,
				  a.z + c.z * lambda };
		}
		else return 0;

		if (is_equal(a_halfplane, 0) && is_equal(b_halfplane, 0))
			return -1;

		return 1;
	}

	bool edge_intersect(const edge& tr_edge) const
	{
		vector dir1 =  a - b,
			   dir2 =  tr_edge.a - tr_edge.b;

		if (!is_equal(determinant(dir1, dir2, a - tr_edge.a), 0))
			return 0;

		vector inter_point = dir1 ^ dir2;

		double alpha{};

		if (inter_point.x != 0)
			alpha = (dir1.z * (tr_edge.a.y - a.y) - dir1.y * (tr_edge.a.z - a.z)) / inter_point.x;
		else if (inter_point.y != 0)
			alpha = (dir1.x * (tr_edge.a.z - a.z) - dir1.z * (tr_edge.a.x - a.x)) / inter_point.y;
		else if (inter_point.z != 0)
			alpha = (dir1.y * (tr_edge.a.x - a.x) - dir1.x * (tr_edge.a.y - a.y)) / inter_point.z;
		else
		{
			return  a.belong_vector(tr_edge.a, tr_edge.b) ||
					b.belong_vector(tr_edge.a, tr_edge.b);
		}

		vector r = tr_edge.a + alpha * dir2;
	
		return r.belong_vector(a, b) && 
			   r.belong_vector(tr_edge.a, tr_edge.b);
	}
};

//-----------------------------------------------------------------------------------------------------------------

struct area
{
	vector min;
	vector max;

	area() = default;
	
	area(const vector& min, const vector& max): 
		min{min}, max{max} {}

	void new_area(const area& prism, int l_id)
	{
		vector id_bits {(double)((l_id & 4) >> 2),
						(double)((l_id & 2) >> 1),
						(double) (l_id & 1)};

		min = prism.min + (0.5 * (prism.max - prism.min) & id_bits);
		max = 0.5 * (prism.max + prism.min + ((prism.max - prism.min) & id_bits));
	}

	void main_area(const vector& point)
	{
		if (!min.is_valid() || !max.is_valid())
			min = max = point;
		else
		{
			min.x = min.x < point.x ? min.x : point.x;
			min.y = min.y < point.y ? min.y : point.y;
			min.z = min.z < point.z ? min.z : point.z;
			max.x = max.x > point.x ? max.x : point.x;
			max.y = max.y > point.y ? max.y : point.y;
			max.z = max.z > point.z ? max.z : point.z;
		}
	}
};

//-----------------------------------------------------------------------------------------------------------------

class triangle
{
	vector a, b, c;
	plane  surface;

public:
	int index;

    triangle() = default;

	triangle(const vector& a, const vector& b, const vector& c, int index = 0): 
			a{a}, b{b}, c{c}, surface{a, b, c}, index{index} {};

	bool is_point () const { return a == b && a == c; }
	bool is_vector() const { return !((a - b) ^ (a - c)); }

	bool intersect(const triangle& other) const
	{
		if(!surface.n && !other.surface.n)
			return degenerate_case(other);

		if (triangle_intersect(other)) 		 return 1;
		if (other.triangle_intersect(*this)) return 1;

		return 0;
	}

	bool belong_area(const area& prism) const
	{
		return 	prism.min < a && a <= prism.max &&
				prism.min < b && b <= prism.max &&
				prism.min < c && c <= prism.max;
	}

	int belong_child(const area& prism) const
	{
		area child_prism[8];

		for (int i = 0; i < 8; i++)
		{
			child_prism[i].new_area(prism, i);
			if (belong_area(child_prism[i]))
				return i;
		}

		return -1;
	}

private:
	bool triangle_intersect(const triangle& other) const
	{
		edge AB {other.a, other.b};
		if (intersect(AB))
			return 1;

		edge BC {other.b, other.c};
		if (intersect(BC))
			return 1;

		edge CA {other.c, other.a};
		if (intersect(BC))
			return 1;

		return 0;
	}

	bool intersect(edge& tr_edge) const
	{
		int is_intersect = tr_edge.intersect_plane(surface);

		if (is_intersect)
		{
			if (is_intersect == -1 && edge_intersect(tr_edge))
				return 1;

			if (belong_triangle(tr_edge.p, a, b, c))
				return 1;
		}

		return 0;
	}

	bool edge_intersect(const edge& tr_edge) const
	{
		if (tr_edge.edge_intersect(edge{a, b}))
			return 1;

		if (tr_edge.edge_intersect(edge{b, c}))
			return 1;

		if (tr_edge.edge_intersect(edge{c, a}))
			return 1;

		return 0;
	}

	bool degenerate_case(const triangle& tr) const
	{
		bool p1 = 	 is_point(),
			 p2 = tr.is_point();

		if (p1 && p2) 
			return a == tr.a;

		bool v1 = 	 is_vector(),
			 v2 = tr.is_vector();

		std::pair<vector, vector> max1, max2;

		   max_vector(max1);
		tr.max_vector(max2);

		if (p1 && v2)
			return a.belong_vector(max2.first, max2.second);

		if (p2 && v1)
			return tr.a.belong_vector(max1.first, max1.second);

		edge ab    {max1.first, max1.second},
			 tr_ab {max2.first, max2.second};
		
		return ab.edge_intersect(tr_ab);
	}

	void max_vector(std::pair<vector, vector>& max) const
	{
		if(a.belong_vector(b, c))
			max.first = b, max.second = c;
		else if(b.belong_vector(a, c))
			max.first = a, max.second = c;
		else
			max.first = a, max.second = b;
	}
};

//-----------------------------------------------------------------------------------------------------------------

class node
{
	area prism;
	std::vector<triangle> v_triangle;
	std::vector<node> 	  leaf;
	static const size_t   l_size {8};

public:
	node() = default;

	node(const area& prism):
		prism{prism}, v_triangle{}, leaf{} {}

	void insert(const triangle& triang)
	{
		int area_index = triang.belong_child(prism);

		if (area_index == -1 || (leaf.empty() && triang.is_point()))
		{
			v_triangle.push_back(triang);
			return;
		}

		if (leaf.empty())
		{
			leaf.resize(l_size);
			for (int i = 0; i < l_size; i++)
				leaf[i].prism.new_area(prism, i);
		}

		leaf[area_index].insert(triang);
	}

	void intersect(std::vector<int>& intersecting_triangles) const
	{
		for (int i = 0; 	i < v_triangle.size(); i++)
		for (int j = i + 1; j < v_triangle.size(); j++)
			if (v_triangle[i].intersect(v_triangle[j]))
			{
				intersecting_triangles.push_back(v_triangle[i].index);
				intersecting_triangles.push_back(v_triangle[j].index);
			}

		for (int k = 0; k < leaf.size(); k++)
		for (int i = 0; i < v_triangle.size(); i++)
			leaf[k].node_intersect(v_triangle[i], intersecting_triangles);

		for (int j = 0; j < leaf.size(); j++)
			leaf[j].intersect(intersecting_triangles);
	}

	void solution() const
	{
		std::vector<int> res;

		intersect(res);

		std::set<int> sorted_res(res.begin(), res.end());

		for (auto it: sorted_res)
			std::cout << it << std::endl;
	}

private:
	void node_intersect(const triangle& tr, std::vector<int>& intersecting_triangles) const
	{
		for (int j = 0; j < v_triangle.size(); j++)
			if (tr.intersect(v_triangle[j]))
			{
				intersecting_triangles.push_back(tr.index);
				intersecting_triangles.push_back(v_triangle[j].index);
			}

		for(int k = 0; k < leaf.size(); k++)
			leaf[k].node_intersect(tr, intersecting_triangles);
	}
};
};

#endif /* H_TRIANGLES */