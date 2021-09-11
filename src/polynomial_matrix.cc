#include <stdlib.h>
#include <polynomial.h>

template<unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_vector<M, N, Q> & polynomial_matrix<P, M, N, Q>::operator[](int index) {
    return matrix[index];
}

template<unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<P, M, N, Q> & polynomial_matrix<P, M, N, Q>::operator+(const polynomial_matrix<P, M, N, Q> & b) {
    struct polynomial_matrix<P, M, N, Q> c;
    struct polynomial_matrix<P, M, N, Q> & a = *this;
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < M; j++) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    return c;
}

// template <unsigned int P2, unsigned int M2, unsigned int N2, unsigned int Q2, unsigned int P3, unsigned int M3, unsigned int N3, unsigned int Q3>
// struct polynomial_matrix<M3, P3, N3, Q3>& polynomial_matrix<P, M, N, Q>::operator*(const struct polynomial_matrix<P2, M2, N2, Q2>&) {
//     struct polynomial_matrix<P3, M3, N3, Q3> c;
//     struct polynomial_matrix<P, M, N, Q> & a = *this;
//     for (int i = 0; i < P; i++) {
//         for (int j = 0; j < M2; j++) {
//             c[i][j] = 0;
//             for (int k = 0; k < P2; k++) {
//                 c[i][j] += a[i][j] * b[k][j]; 
//             }
//         }
//     }
//     return c;
// }

template <unsigned int P1, unsigned int M1, unsigned int P2, unsigned int M2, unsigned int N, unsigned int Q>
struct polynomial_matrix<P1, M2, N, Q> & polynomial_matrix<P1, M1, N, Q>::operator*(const polynomial_matrix<P2, M2, N, Q> & b) {
    struct polynomial_matrix<P1, M2, N, Q> c;
    struct polynomial_matrix<P1, M1, N, Q> & a = *this;
    for (int i = 0; i < P1; i++) {
        for (int j = 0; j < M2; j++) {
            c[i][j] = 0;
            for (int k = 0; k < P2; k++) {
                c[i][j] += a[i][j] * b[k][j]; 
            }
        }
    }
    return c;
}
