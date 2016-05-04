all : cnpy run
	python test.py


cnpy : cnpy.cpp cnpy.hpp
	g++ -std=c++11 $^ -o cnpy

run : cnpy
	./cnpy

	
clean :
	git clean -fxd

.PHONY : clean
