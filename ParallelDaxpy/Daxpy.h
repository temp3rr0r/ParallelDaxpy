#pragma once
#include <stddef.h>

using namespace std;

void daxpy(size_t from, size_t to, double a, const double* x, double* y);
void daxpy_parallel(size_t n, double a, const double* x, double* y, size_t nt);
void daxpy_parallel_exact_work(size_t n, double a, const double* x, double* y, size_t nt);

void daxpy_parallel_pthreads(size_t n, double a, const double* x, double* y, size_t nt);
void daxpy_parallel_exact_work_pthreads(size_t n, double a, const double* x, double* y, size_t nt);
void *daxpy_pthreads(void* data_in);