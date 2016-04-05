#include <iostream>
#include <stdlib.h>
#include "lucp.cpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <sys/time.h>

// call to truncated LU functions (written in fortran)
extern "C" int test_(int* M, int* N, double* A, int* LDA, int* IPIV, int* INFO, int* RANK, int* BS);

double read_timer() {
  // a simple timer function
  static bool initialized = false;
  static struct timeval start;
  struct timeval end;
  if(!initialized) {
      gettimeofday(&start, NULL);
      initialized = true;
  }
  gettimeofday(&end, NULL);
  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}


int main(int argc, char* argv[]) {
  // A truncated version of DGETRF is timed for various sizes of 
  // random matrices.  These tests will serve as a benchmark for
  // the method currently under review.

  // problem size
  int dims[] = {1000, 2000, 3000, 4000};
  std::vector<int> num_rows (dims, dims + sizeof(dims) / sizeof(int));
  std::vector<int> num_cols (dims, dims + sizeof(dims) / sizeof(int));

  // seed for random number generator
  int iseed[] = {0, 0, 0, 1};

  // generate data
  std::cout << "Generating random data..." << std::endl;
  int row_largest = num_rows[std::max_element(num_rows.begin(), num_rows.end()) - num_rows.begin()];
  int col_largest = num_cols[std::max_element(num_cols.begin(), num_cols.end()) - num_cols.begin()];
  double* A = new double[row_largest * col_largest];
  std::shared_ptr<double> A_ptr (A);
  lucp::matGen(A_ptr, row_largest, col_largest, iseed);
  std::cout << "Running experiments..." << std::endl;

  // run experiments
  for (auto n_row : num_rows) {
    for (auto n_col : num_cols) {

      // truncated DGETRF benchmarking
      double seconds = read_timer();
      int* ipiv = new int[std::min(n_row, n_col)];
      int INFO;
      int r = 300; // truncation rank
      int nb = 64;
      test_(&n_row, &n_col, A, &row_largest, ipiv, &INFO, &r, &nb);
      seconds = read_timer() - seconds;
      std::cout << "DGETRF_TRUNC time: " << std::setprecision(4) << seconds 
                << "\t seconds on " << n_row << " rows and " << n_col 
                << " cols" << std::endl;
    }
  }
  return 0;
}
