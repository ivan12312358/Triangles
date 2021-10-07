#include <iostream>
#include <unordered_set>
#include <set>
#include <cmath>

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

	vector(): x{NAN}, y{NAN}, z{NAN} {}

	vector(double x, double y, double z):
		   x{x}, y{y}, z{z} {}

	vector(const vector& other):
		   x{other.x}, y{other.y}, z{other.z} {}

	const vector& operator= (const vector& other)
	{
		x = other.x, y = other.y, z = other.z;
		return *this;
	}

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

    ~vector() {}
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
		D = -determinant(a, r1, r2);
		n = {A, B, C};
	};

    ~plane() { A = NAN, B = NAN, C = NAN, D = NAN; }
};

//-----------------------------------------------------------------------------------------------------------------

struct edge
{
	vector a, b, p;

	edge(): a{}, b{}, p{} {};

	edge(const vector& a, const vector& b): a{a}, b{b}, p{} {};

	edge(const edge& other): a{other.a}, b{other.b}, p{other.p} {};

	bool intersect_plane(const plane& plane)
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

		return 1;
	}

    ~edge() {};
};

//-----------------------------------------------------------------------------------------------------------------

struct area
{
	vector min;
	vector max;

	area(): min{}, max{} {}
	
	area(const vector& min, const vector& max): 
		min{min}, max{max} {}

	area(const area& other): 
		min{other.min}, max{other.max} {}

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

	~area() {};
};

//-----------------------------------------------------------------------------------------------------------------

class triangle
{
	vector a, b, c;
	plane  surface;

public:
	int index;

    triangle(): a{}, b{}, c{}, surface{}, index{} {};

	triangle(const vector& a, const vector& b, const vector& c, int index = 0): 
			a{a}, b{b}, c{c}, surface{a, b, c}, index{index} {};

	triangle(const triangle& other):
			a{other.a}, b{other.b}, c{other.c}, surface{other.surface}, index{other.index} {};

	bool is_point () const { return a == b && a == c; }
	bool is_vector() const { return ((a - b) ^ (a - c)) == 0; }

	bool intersect(const triangle& other) const
	{
		if(surface.n == 0 && other.surface.n == 0)
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

    ~triangle() {};

private:
	bool triangle_intersect(const triangle& other) const
	{
		edge AB {other.a, other.b};
		if (AB.intersect_plane(surface))
			if (belong_triangle(AB.p, a, b, c)) 
				return 1;

		edge BC {other.b, other.c};
		if (BC.intersect_plane(surface))
			if (belong_triangle(BC.p, a, b, c)) 
				return 1;

		edge CA {other.c, other.a};
		if (CA.intersect_plane(surface))
			if (belong_triangle(CA.p, a, b, c)) 
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

		vector v_r1[2], v_r2[2];

		   direct_vector(v_r1);
		tr.direct_vector(v_r2);

		if (p1 && v2)
			return a.belong_vector(v_r2[0], v_r2[1]);

		if (p2 && v1)
			return tr.a.belong_vector(v_r1[0], v_r1[1]);

		vector r1 =  v_r1[1] - v_r1[0],
			   r2 =  v_r2[1] - v_r2[0];

		if (!is_equal(determinant(r1, r2, a - tr.a), 0))
			return 0;

		if ((r1 ^ r2) == 0)
		{
			return  v_r1[0].belong_vector(v_r2[0], v_r2[1]) ||
					v_r1[1].belong_vector(v_r2[0], v_r2[1]);
		}

		double alpha = ((tr.a.x - a.x) * r1.y - (tr.a.y - a.y) * r1.x) / 
						(r1.x * r2.y - r2.x * r1.y);

		vector r = tr.a + alpha * r2;
	
		return r.belong_vector(v_r1[0], v_r1[1]) && r.belong_vector(v_r2[0], v_r2[1]);
	}

	void direct_vector(vector* v_r) const
	{
		if(v_r == nullptr)
			return;

		if(a.belong_vector(b, c))
			v_r[0] = b, v_r[1] = c;
		else if(b.belong_vector(a, c))
			v_r[0] = a, v_r[1] = c;
		else
			v_r[0] = a, v_r[1] = b;
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
	node(): prism{}, v_triangle{}, leaf{} {}

	node(const area& prism):
		prism{prism}, v_triangle{}, leaf{} {}

	node(const node& other):
		prism{other.prism}, v_triangle{other.v_triangle}, leaf{other.leaf} {}

	void insert(const triangle& triang)
	{
		int area_index = triang.belong_child(prism);

		if (area_index == -1)
		{
			v_triangle.push_back(triang);
			return;
		}

		if (leaf.empty() && !triang.is_point())
		{
			leaf.resize(l_size);
			for (int i = 0; i < l_size; i++)
				leaf[i].prism.new_area(prism, i);
		}

		leaf[area_index].insert(triang);
	}

	void intersect(std::unordered_set<int>& intersecting_triangles) const
	{
		for (int i = 0; 	i < v_triangle.size(); i++)
		for (int j = i + 1; j < v_triangle.size(); j++)
			if (v_triangle[i].intersect(v_triangle[j]))
			{
				intersecting_triangles.insert(v_triangle[i].index);
				intersecting_triangles.insert(v_triangle[j].index);
			}

		for (int k = 0; k < leaf.size(); k++)
		for (int i = 0; i < v_triangle.size(); i++)
			leaf[k].node_intersect(v_triangle[i], intersecting_triangles);

		for (int j = 0; j < leaf.size(); j++)
			leaf[j].intersect(intersecting_triangles);
	}

	void solution() const
	{
		std::unordered_set<int> res;

		intersect(res);

		std::set<int> sorted_res(res.begin(), res.end());

		for (auto it: sorted_res)
			std::cout << it << std::endl;
	}

	~node() {};

private:
	void node_intersect(const triangle& tr, std::unordered_set<int>& intersecting_triangles) const
	{
		for (int j = 0; j < v_triangle.size(); j++)
			if (tr.intersect(v_triangle[j]))
			{
				intersecting_triangles.insert(tr.index);
				intersecting_triangles.insert(v_triangle[j].index);
			}

		for(int k = 0; k < leaf.size(); k++)
			leaf[k].node_intersect(tr, intersecting_triangles);
	}
};
};

#endif /* H_TRIANGLES */