/* Included on header file */
#include <assert.h>

template<unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_vector<M, N, Q> & polynomial_matrix<P, M, N, Q>::operator[](int index) {
    return matrix[index];
}

template<unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_vector<M, N, Q> polynomial_matrix<P, M, N, Q>::operator[](int index) const {
	return matrix[index];
}

template<unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<P, M, N, Q> polynomial_matrix<P, M, N, Q>::operator+(const polynomial_matrix<P, M, N, Q> & b) {
    struct polynomial_matrix<P, M, N, Q> c;
    struct polynomial_matrix<P, M, N, Q> & a = *this;
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < M; j++) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    return c;
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
template <unsigned int P2, unsigned int M2>
struct polynomial_matrix<P, M2, N, Q> polynomial_matrix<P, M, N, Q>::operator*(const polynomial_matrix<P2, M2, N, Q> & b) {
	struct polynomial_matrix<P, M2, N, Q> c;
	struct polynomial_matrix<P, M, N, Q> & a = *this;
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < M2; j++) {
			c[i][j] = 0;
			for (int k = 0; k < P2; k++) {
				c[i][j] += a[i][j] * b[k][j]; 
			}
		}
	}
	return c;
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_vector<P, N, Q> polynomial_matrix<P, M, N, Q>::operator*() {
	assert (M == 1);
	polynomial_vector<P, N, Q> b;
	polynomial_matrix<P, M, N, Q> & a = *this;
	for (int i = 0; i < P; i++) {
		b[i] = a[0][i];
	}
	return b;
}