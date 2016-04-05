David G. Anderson
2016

The current data approximation paper and code are under review 
and will be made available publicly shortly.  The files here 
are a base case that can be used to measure the performance 
of our new method.

Files:

 - dgetrf_trunc.f
     This is the standard BLAS 3 LU decomposition, but the 
     factorization is truncated at a target rank

 - CURtest.cpp
     Main script that generates random data and performs 
     benchmarking tests on dgetrf_trunc.f

 - lucp.cpp
     For now this file only has basic helper functions

 - makefile
     A standard makefile.  For compilation on NERSC’s Edison 
     or Cori, using MKL
