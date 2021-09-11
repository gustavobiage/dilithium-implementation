#include <stdlib.h>
#include <polynomial.h>

template<unsigned int N, unsigned int Q>
struct polynomial<Q> & polynomial_vector<N, Q>::operator[](int index) {
    return vector[index];
}

// Referencing returns matrix
template<unsigned int N, unsigned int Q>
struct polynomial_matrix<N, 1, Q> & polynomial_vector<N, Q>::operator&() {
    struct polynomial_matrix<N, 1, Q> _a;
    struct polynomial_matrix<N, 1, Q> & a = *this;
    for (int i = 0; i < N; i++) {
        _a[i][0] = a[i];
    }
    return _a;
}
