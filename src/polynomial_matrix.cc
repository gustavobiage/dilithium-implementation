#include <stdlib.h>
#include <polynomial.h>

template<unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_vector<N, Q> & polynomial_matrix<M, N, Q>::operator[](int index) {
    return matrix[index];
}

template<unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<M, N, Q> & polynomial_matrix<M, N, Q>::operator+(const polynomial_matrix<M, N, Q> & b) {
    struct polynomial_matrix<M, N, Q> c;
    struct polynomial_matrix<M, N, Q> & a = *this;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    return c;
}

template<unsigned int M1, unsigned int N1, unsigned int M2, unsigned int N2, unsigned int Q>
struct polynomial_matrix<M1, N2, Q> & polynomial_matrix<M1, N1, Q>::operator*(const polynomial_matrix<M2, N2, Q> & b) {
    struct polynomial_matrix<M1, N2, Q> c;
    struct polynomial_matrix<M1, N1, Q> & a = *this;
    pmatrix * res = new pmatrix(lines(), _m.columns());
    for (int i = 0; i < M1; i++) {
        for (int j = 0; j < N2; j++) {
            c[i][j] = 0;
            for (int k = 0; k < M2; k++) {
                c[i][j] += a[i][j] * b[k][j]; 
            }
        }
    }
    return c;
}
