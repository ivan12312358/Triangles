all: main.cpp
	g++ -O2 main.cpp -o main

check: tests.cpp
	g++ -O2 tests.cpp -o tests