# cnpy2

This library writes a given STL vector of `double` of size N to a numpy ndarray
of shape = `( N/cols, cols)`. Two modes are supported:

 - Write mode "w" (the file will be overwritten)
 - Append mode "a" (this will append the new data to npy file and modify the
   haeder as well).


## Use case

See file `example1.cpp`. In this  example, I create a binary numpy file with 4
entries (2x2 shape), and then we append 100x100 entries in a for-loop.

This writes to numpy format version 2 (record arrays). See the `test.py` file.

There is much more complete project https://github.com/rogersce/cnpy which
writes/reads your data to numpy version 1. 
