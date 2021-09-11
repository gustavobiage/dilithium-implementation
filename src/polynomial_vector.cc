#include <stdlib.h>
#include <polynomial.h>

template<unsigned int M, unsigned int N, unsigned int Q>
struct polynomial<N, Q> & polynomial_vector<M, N, Q>::operator[](int index) {
    return vector[index];
}

// Referencing returns matrix
template<unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<M, 1, N, Q> & polynomial_vector<M, N, Q>::operator&() {
    struct polynomial_matrix<M, 1, N, Q> _a;
    struct polynomial_matrix<M, 1, N, Q> & a = *this;
    for (int i = 0; i < M; i++) {
        _a[i][0] = a[i];
    }
    return _a;
}
