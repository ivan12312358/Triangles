#include <iostream>
#include <cmath>

#ifndef H_TRIANGLES
#define H_TRIANGLES

namespace geometry
{
const double epsilon = 0.0001;

bool is_equal(double a, double b) { return fabs(a - b) <= epsilon; }

//-----------------------------------------------------------------------------------------------------------------

class vector
{
public:
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

	vector operator&  (const vector& other) const { return vector (x * other.x,  y * other.y,  z * other.z); }
	vector operator+  (const vector& other) const { return vector (x + other.x,  y + other.y,  z + other.z); }
	vector operator-  (const vector& other) const { return vector (x - other.x,  y - other.y,  z - other.z); }
	vector operator*  (double alpha) 	    const { return vector (x * alpha,    y * alpha,    z * alpha);   }	
	double operator*  (const vector& other) const { return 	       x * other.x + y * other.y + z * other.z;  }
	double operator() () 					const { return 		   std::sqrt(x * x + y * y + z * z); }
	bool   operator<  (const vector& other) const { return x <  other.x && y <  other.y && z <  other.z; }
	bool   operator<= (const vector& other) const { return x <= other.x && y <= other.y && z <= other.z; }
	bool is_valid() 						const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z); }
	void print() const 
	{
		std::cout << "Vector:" << std::endl;
		std::cout << x << " " << y << " " << z << std::endl; 
	}

    ~vector() {x = NAN, y = NAN, z = NAN;}
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

class plane
{
public:
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

	void print() const
	{
		std::cout << "Plane:" << std::endl;
		std::cout << A << " " << B << " " << C << " " << D << std::endl;
	}
};

//-----------------------------------------------------------------------------------------------------------------

class edge
{
public:
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

class area
{
public:
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

	void print() const
	{
		std::cout << "Area:" << std::endl;
		min.print(); max.print();
	}

	~area() {};
};

//-----------------------------------------------------------------------------------------------------------------

class triangle
{
	vector a, b, c;
	plane  surface;

public:
    triangle(): a{}, b{}, c{}, surface{} {};

	triangle(const vector& a, const vector& b, const vector& c): 
			a{a}, b{b}, c{c}, surface{a, b, c} {};

	triangle(const triangle& other):
			a{other.a}, b{other.b}, c{other.c}, surface{other.surface} {};

	bool triangles_intersection(const triangle& other) const
	{
		if (intersection_point(other)) 		 return 1;
		if (other.intersection_point(*this)) return 1;
		return 0;
	}

	bool belong_area(const area& prism) const
	{
		return 	prism.min <= a && a < prism.max &&
				prism.min <= b && b < prism.max &&
				prism.min <= c && c < prism.max;
	}

	void print() const
	{
		std::cout << "Triangle:" << std::endl;
		a.print(); b.print(); c.print(); surface.print();
	}

    ~triangle() {};


private:
	bool intersection_point(const triangle& other) const
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
		if (!insert_in_node(triang))
			v_triangle.push_back(triang);
	}

	void print() const
	{
		std::cout << "Node:" << std::endl;
		prism.print();

		for(auto it: v_triangle)
			it.print();

		if (leaf.size() == l_size)
		{
			for (auto it: leaf)
				it.print();
		}
	}

	~node() {};

private:
	bool insert_in_node(const triangle& triang)
	{
		if (leaf.size() != l_size)
		{
			leaf.resize(l_size);

			for (int i = 0; i < l_size; ++i)
				leaf[i].prism.new_area(prism, i);
		}

		for (int i = 0; i < l_size; ++i)
		{
			if (triang.belong_area(leaf[i].prism))
			{	
				if (!leaf[i].insert_in_node(triang))
					 v_triangle.push_back(triang);

				return 1;
			}
		}
		return 0;
	}
};
};

#endif /* H_TRIANGLES */