all: main.cpp triangles.h
	g++ -O2 main.cpp -o main

check: tests.cpp triangles.h
	g++ -O2 tests.cpp -o tests