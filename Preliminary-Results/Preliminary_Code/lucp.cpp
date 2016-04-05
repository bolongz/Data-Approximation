#include <iostream>
#include <vector>
#include <memory>

// LAPACK function
extern "C" void dlagge_(int* M, int* N, int* KL, int* KU, double* D, 
			double* A, int* LDA, int* ISEED, double* WORK, int* INFO);

class lucp {

  public:

    template <class T>
    static void printMatrix(const T* M, int m, int n) {
      // prints integer matrix to terminal
      for (int iRow=0; iRow<m; iRow++){
      for (int iCol=0; iCol<n; iCol++){
        std::cout << M[iRow+iCol*m] << " ";
      }
      std::cout << std::endl;
      }
    };

    static void matGen(std::shared_ptr<double> A_ptr, int m, int n, int* iseed) {
      // generates a random matrix
      int kl = m - 1;
      int ku = n - 1;
      int r = std::min(m, n);
      std::vector<double> D (r, 1.0);
      double* work = new double[m + n];
      std::shared_ptr<double> work_ptr (work);
      int info;
      dlagge_(&m, &n, &kl, &ku, &D[0], A_ptr.get(), &m, iseed, work, &info);
    };
};


