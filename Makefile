all : cnpy run
	echo "Now run make test"


cnpy : cnpy.cpp cnpy.hpp
	g++ -Wall -std=c++11 $^ -o cnpy 

run : cnpy
	./cnpy

test:
	python ./test.py
	
clean :
	git clean -fxd

.PHONY : clean
