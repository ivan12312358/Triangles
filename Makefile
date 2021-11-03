CXX = g++
CXXFLAGS = -O2 -o $@

triangles: main.cpp triangles.hpp
	$(CXX) $(CXXFLAGS) $<

gentest: ./Tests/gentest.cpp triangles.hpp
	$(CXX) $(CXXFLAGS) $<
