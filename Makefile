all : cnpy 
	echo "Now run make run"


cnpy : cnpy.cpp cnpy.hpp
	g++ -Wall -std=c++11 $^ -o cnpy 

run : cnpy
	./cnpy
	echo "Now run make test"

test:
	python ./test.py
	
clean :
	git clean -fxd

.PHONY : clean
