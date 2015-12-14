#include <thread>
#include <vector>
#include <pthread.h>
#include "Settings.h"
#include "Daxpy.h"

using namespace std;

// Serial Daxpy on a specific range, y[n] <- a * x[n] + y[n]
void daxpy(size_t from, size_t to, double a, const double* x, double* y) {
	for (size_t i = from; i < to; i++)
		y[i] = a * x[i] + y[i];
}

// Runs daxpy using the given number of threads. Splits the 
// work equally. Remarks: Leaves left over work. Leftover work size = n % nt
void daxpy_parallel_exact_work(size_t n, double a, const double* x, double* y, size_t nt) {	
	size_t work_size = n / nt; // Calculate the work size for each thread
	
	vector<thread> threads(nt); // Allocate a vector of threads
	// Assign work to each thread and Start them
	for (size_t i = 0; i < nt; i++) {
		size_t from = i * work_size; // Starting work index
		size_t to = from + work_size; // Ending work index
		threads[i] = thread(daxpy, from, to, a, x, y); // Run the thread
	}

	for (thread& current_thread : threads) // Wait for all threads to finish
		current_thread.join();
}

// DAXPY constant * a vector plus a vector.
// nt: number of threads, n: size of the vectors
void daxpy_parallel(size_t n, double a, const double* x, double* y, size_t nt) {
	
	if (n <= 0) // No elements on the vector arrays, exit
		return;
	
	if (n == 1 || nt == 1) // Only one element on the vector array or 1 thread requested, just use the main thread
		daxpy(0, n, a, x, y);
	else if (n > 1 && n < nt) // Number of elements less than the total element count		
		daxpy_parallel_exact_work(n, a, x, y, n); // number of threads <- vector size
	else {				

		// Split and run equal work on all threads
		daxpy_parallel_exact_work(n, a, x, y, nt);

		// The "leftover" work will be run from the main thread.
		size_t remaining_work_size = n % nt;
		
		// Run remaining work on the main thread
		if (remaining_work_size != 0) {
			size_t work_size = n / nt;
			size_t from = nt * work_size;
			size_t to = n;
			daxpy(from, to, a, x, y);
		}
	}	
}

struct thread_data {
	size_t from;
	size_t to;
	double a;
	const double* x;
	double* y;
};

// Serial Daxpy for Pthreads on a specific range, y[n] <- a * x[n] + y[n]
void *daxpy_pthreads(void* data_in) {

	thread_data *data;
	data = static_cast<thread_data*>(data_in);
	size_t in_from = data->from;
	size_t in_to = data->to;

	double in_a = data->a;
	const double* in_x = data->x;
	double* in_y = data->y;

	for (size_t i = in_from; i < in_to; i++)
		in_y[i] = in_a * in_x[i] + in_y[i];
	
	return nullptr;
}

void daxpy_parallel_exact_work_pthreads(size_t n, double a, const double* x, double* y, size_t nt) {
	
	int rc;
	void* status;
	size_t work_size = n / nt; // Calculate the work size for each thread

	pthread_t p_threads[DEFAULT_NUMBER_OF_THREADS];
	pthread_attr_t attribute;

	// Initialize and set thread detached attribute
	pthread_attr_init(&attribute);
	pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_JOINABLE);

	for (size_t i = 0; i < nt; i++) {
		struct thread_data td;
		td.a = a;
		td.x = x;
		td.y = y;
		size_t from = i * work_size; // Starting work index
		size_t to = from + work_size; // Ending work index
		td.from = from;
		td.to = to;
		rc = pthread_create(&p_threads[i], &attribute, daxpy_pthreads, &td); // Run the thread
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
				exit(-1);
		}
	}

	// Free attribute and wait for the other threads
	pthread_attr_destroy(&attribute);
	for (size_t i = 0; i < nt; i++) {
		rc = pthread_join(p_threads[i], &status);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
				exit(-1);
		}
	}
	
	//pthread_exit(NULL);
}

void daxpy_parallel_pthreads(size_t n, double a, const double* x, double* y, size_t nt) {

	if (n <= 0) // No elements on the vector arrays, exit
		return;

	if (n == 1 || nt == 1) // Only one element on the vector array or 1 thread requested, just use the main thread
		daxpy(0, n, a, x, y);
	else if (n > 1 && n < nt) // Number of elements less than the total element count	
		daxpy_parallel_exact_work_pthreads(n, a, x, y, n); // number of threads <- vector size
	else {

		// Split and run equal work on all threads
		daxpy_parallel_exact_work_pthreads(n, a, x, y, nt);

		// The "leftover" work will be run from the main thread.
		size_t remaining_work_size = n % nt;

		// Run remaining work on the main thread
		if (remaining_work_size != 0) {
			size_t work_size = n / nt;
			size_t from = nt * work_size;
			size_t to = n;
			daxpy(from, to, a, x, y);
		}
	}
}