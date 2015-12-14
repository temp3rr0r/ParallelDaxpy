// ParallelDaxpy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cassert>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Put data into the vector arrays
void populate_vector_data(double* x, double* y, size_t n) {
	for (size_t i = 0; i < n; i++) {
		x[i] = static_cast<double>(i);
		y[i] = static_cast<double>(i) + DEFAULT_Y_ADDITION;
	}
}

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

	if (n > 0) {
		// Allocate Data vectors
		double* x = new double[n];
		double* y = new double[n];

		if (DEBUG)
			debug_print_vectors(x, y, n); // Print input vectors

		// Compare running serial, with pthreads and with c++11 threads

		clock_t t1, t2;

		// C++ 11 threads
		populate_vector_data(x, y, n);
		t1 = clock();
		daxpy_parallel(n, a, x, y, nt); // Calculate using C++ 11 threads
		t2 = clock();
		assert_results(a, x, y, n);
		cout << "C++ 11 execution: " << 1000 * (float(t2 - t1) / CLOCKS_PER_SEC) << " ms" << endl;

		// Serial Execution
		populate_vector_data(x, y, n);
		t1 = clock();
		daxpy(0, n, a, x, y); // Calculate using serial execution
		t2 = clock();
		assert_results(a, x, y, n);
		cout << "Serial execution: " << 1000 * (float(t2 - t1) / CLOCKS_PER_SEC) << " ms" << endl;

		// Pthreads
		populate_vector_data(x, y, n);
		t1 = clock();
		daxpy_parallel_pthreads(n, a, x, y, nt); // Calculate using Pthreads
		t2 = clock();
		//assert_results(a, x, y, n);
		cout << "PThreads execution: " << 1000 * (float(t2 - t1) / CLOCKS_PER_SEC) << " ms" << endl;
		
		if (DEBUG)
			debug_print_results(a, x, y, n); // Print results
		
		// Deallocate arrays
		delete[] x;
		delete[] y;
	} else
		cout << "No data" << endl;

    return 0;
}

