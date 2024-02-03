#include <iostream>
#include <chrono>

extern "C"
    double* addVectorsGPU(double* a, double* b, int n) {
    double* z;

    z = (double*) malloc(n * sizeof(double));

    return z;
}
