#include <iostream>
#include <cmath>

#ifndef H_TRIANGLES
#define H_TRIANGLES

const double epsilon = 0.0001;

namespace geometry
{
class vector
{
public:
	double x, y, z;

	vector(): x{NAN}, y{NAN}, z{NAN} {}

	vector(double x1, double y1, double z1):
		   x{x1}, y{y1}, z{z1} {}

	vector(const vector& other):
		   x{other.x}, y{other.y}, z{other.z} {}

	const vector& operator= (const vector& other)
	{
		x = other.x, y = other.y, z = other.z;
		return *this;
	}

	vector operator+  (const vector& other) const { return vector (x + other.x,  y + other.y,  z + other.z); }
	vector operator-  (const vector& other) const { return vector (x - other.x,  y - other.y,  z - other.z); }
	vector operator*  (double alpha) 	    const { return vector (x * alpha,    y * alpha,    z * alpha);   }	
	double operator*  (const vector& other) const { return 	       x * other.x + y * other.y + z * other.z;  }
	double operator() () 					const { return 		   x * x + y * y + z * z; }
	bool is_valid() 						const { return std::isfinite(x) && std::isfinite(y) && std::isfinite(z); }
	void print() 							const { std::cout << x << " " << y << " " << z << std::endl; }


    ~vector() {x = NAN, y = NAN, z = NAN;}
};

//-----------------------------------------------------------------------------------------------------------------

vector operator*    (double alpha, const vector& other) { return vector(other * alpha); }

//-----------------------------------------------------------------------------------------------------------------

double determinant  (const vector& a, const vector& b, const vector& c)
{
	return a.x * b.y * c.z - a.x  * b.z * c.y +
		   a.y * b.z * c.x - a.y  * b.x * c.z +
		   a.z * b.x * c.y - a.z  * b.y * c.x;
}

//-----------------------------------------------------------------------------------------------------------------

bool belong_triangle(const vector& p, const vector& a, const vector& b, const vector& c)
{
	vector r1 {a - p}, r2 {b - p}, r3 {c - p};

	double alpha = acos( (r1 * r2) / (r1() * r2()) );
	double gamma = acos( (r2 * r3) / (r2() * r3()) );
	double beta  = acos( (r3 * r1) / (r3() * r1()) );

	if(alpha + beta + gamma >= acos(-1) - epsilon)
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
		vector a1 {b - a}, a2 {c - a};

		A = a1.y * a2.z - a2.y * a1.z;
		B = a2.x * a1.z - a1.x * a2.z;
		C = a1.x * a2.y - a2.x * a1.y;
		D = -determinant(a, a1, a2);
		n = {A, B, C};
	};

    ~plane() { A = NAN, B = NAN, C = NAN, D = NAN; }
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

		if (a_halfplane == 0) 
			p = a;
		else if (b_halfplane == 0) 
			p = b;
		else if (a_halfplane * b_halfplane <= 0)
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

class triangle
{
	vector a, b, c;
	plane surface;
public:
    triangle(): a{}, b{}, c{}, surface{} {};

	triangle(const vector& a, const vector& b, const vector& c): 
			a{a}, b{b}, c{c}, surface{a, b, c} {};

	triangle(const triangle& other):
			a{other.a}, b{other.b}, c{other.c}, surface{other.a, other.b, other.c} {};

	bool intersection_point(const triangle& other) const
	{
		edge AB {a, b};
		if (AB.intersect_plane(other.surface))
			if (belong_triangle(AB.p, other.a, other.b, other.c)) 
				return 1;

		edge BC {b, c};
		if (BC.intersect_plane(other.surface))
			if (belong_triangle(BC.p, other.a, other.b, other.c)) 
				return 1;

		edge CA{c, a};
		if (CA.intersect_plane(other.surface))
			if (belong_triangle(CA.p, other.a, other.b, other.c)) 
				return 1;

		return 0;
	}

    ~triangle() {};
};
};

#endif /* H_TRIANGLES */