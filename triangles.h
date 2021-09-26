#include <iostream>
#include <cmath>

#ifndef H_TRIANGLES
#define H_TRIANGLES

namespace geometry
{
const double epsilon = 0.0001;

bool is_equal(double a, double b) { return fabs(a - b) <= epsilon; }

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
	double operator() () 					const { return 		   std::sqrt(x * x + y * y + z * z); }
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

/*
class sphere
{
public:
	vector O;
	double R;

	sphere(): O{}, R{NAN} {}

	sphere(const vector& O, double R): O{O}, R{R} {}
	sphere(const sphere& other): O{other.O}, R{other.R} {}

	bool belong_sphere(const vector& a) const
	{
		vector b {a - O};
		return b() <= R;
	};

	~sphere() {};
};
*/

class triangle
{
	vector a, b, c;
	plane  surface;
//	sphere tr_area;

public:
    triangle(): a{}, b{}, c{}, surface{}/* , tr_area{} */ {};

	triangle(const vector& a, const vector& b, const vector& c): 
			a{a}, b{b}, c{c}, surface{a, b, c}/* , tr_area{a, a() + b()} */ {};

	triangle(const triangle& other):
			a{other.a}, b{other.b}, c{other.c}, surface{other.surface}/* , tr_area{other.tr_area} */ {};

	bool triangles_intersection(const triangle& other) const
	{
		if(intersection_point(other)) 		return 1;
		if(other.intersection_point(*this)) return 1;
		return 0;
	}

    ~triangle() {};

private:
	bool intersection_point(const triangle& other) const
	{
		// if (!tr_area.belong_sphere(other.a) &&
		//     !tr_area.belong_sphere(other.b) &&
		//     !tr_area.belong_sphere(other.c))
		// {
		// 	return 0;
		// }

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
};

#endif /* H_TRIANGLES */