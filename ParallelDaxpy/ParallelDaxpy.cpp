// ParallelDaxpy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cassert>

static const int DEFAULT_NUMBER_OF_THREADS = 1;
static const int DEFAULT_VECTOR_SIZE = 1;
static const double DEFAULT_PARAMETER_A = 1.0;
static const double DEFAULT_Y_ADDITION = 3.25;
static const bool DEBUG = false;

using namespace std;

// Print data
void debug_print_vectors(double* x, double* y, size_t n) {	
	for (size_t i = 0; i < n; i++) {
		cout << "x[" << i << "] = " << x[i];
		cout << " y[" << i << "] = " << y[i] << endl;
	}
}

// Print results
void debug_print_results(double a, double* x, double* y, size_t n) {
	for (size_t i = 0; i < n; i++) {
		cout << "y[" << i << "] = " << a << " * x[" << i << "] + y[" << i << "] = " << y[i] << endl;
		assert(a * x[i] + (x[i] + DEFAULT_Y_ADDITION) == y[i]);
	}
}

// Assert results
void assert_results(double a, double* x, double* y, size_t n) {
	for (size_t i = 0; i < n; i++) {
		assert(a * x[i] + (x[i] + DEFAULT_Y_ADDITION) == y[i]);
	}
}

int main()
{	
	int nt = DEFAULT_NUMBER_OF_THREADS; // number of threads	
	double a = DEFAULT_PARAMETER_A; // constant parameter
	size_t n = DEFAULT_VECTOR_SIZE; // Vector size
	
	// User Input data
	a = 1.37;
	nt = 1;
	n = 38525;

	if (n > 0) {
		// Allocate Data vectors
		double* x = new double[n];
		double* y = new double[n];

		// Put data into vectors
		for (size_t i = 0; i < n; i++) {
			x[i] = static_cast<double>(i);
			y[i] = static_cast<double>(i) + DEFAULT_Y_ADDITION;
		}

		if (DEBUG)
			debug_print_vectors(x, y, n); // Print input vectors

		// Calculate
		daxpy_parallel(n, a, x, y, nt);

		if (DEBUG)
			debug_print_results(a, x, y, n); // Print results

		// Assert results
		assert_results(a, x, y, n);

		// Deallocate arrays
		delete[] x;
		delete[] y;
	} else
		cout << "No data" << endl;

    return 0;
}

