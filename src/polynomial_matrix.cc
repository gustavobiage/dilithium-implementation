/* Included on header file */
#include <assert.h>

template<unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
polynomial_matrix<P, M, N, Q>::polynomial_matrix(const struct polynomial_vector<P, N, Q> & b) {
	if (M == 1) {
		struct polynomial_matrix<P, 1, N, Q> & a = *this;
		for (int i = 0; i < P; i++) {
			a[i][0] = b[i];
		}
	} else {
		throw std::domain_error("Vector to Matrix conversion requirer column matrix (constructor)");
	}
}

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

template<unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<P, M, N, Q> polynomial_matrix<P, M, N, Q>::operator+(const struct polynomial_vector<P, N, Q>& b_) {
	if (M == 1) {
		const struct polynomial_matrix<P, 1, N, Q> b = &b_;
		struct polynomial_matrix<P, M, N, Q> & a = *this;
		struct polynomial_matrix<P, 1, N, Q> c = a + b;
		return c;
	} else {
		throw std::domain_error("Vector to Matrix conversion requirer column matrix (polynomial_matrix<P, M, N, Q>::operator+)");
	}
}

template<unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<P, M, N, Q> polynomial_matrix<P, M, N, Q>::operator-(const polynomial_matrix<P, M, N, Q>& b) {
    struct polynomial_matrix<P, M, N, Q> c;
    struct polynomial_matrix<P, M, N, Q> & a = *this;
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < M; j++) {
            c[i][j] = a[i][j] - b[i][j];
        }
    }
    return c;
}

template<unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<P, M, N, Q> polynomial_matrix<P, M, N, Q>::operator-(const struct polynomial_vector<P, N, Q>& b_) {
	if (M == 1) {
		const struct polynomial_matrix<P, 1, N, Q> b = &b_;
		struct polynomial_matrix<P, M, N, Q> & a = *this;
		struct polynomial_matrix<P, 1, N, Q> c = a - b;
		return c;
	} else {
		throw std::domain_error("Vector to Matrix conversion results in a column matrix (polynomial_matrix<P, M, N, Q>::operator-)");
	}
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
template <unsigned int M2>
struct polynomial_matrix<P, M2, N, Q> polynomial_matrix<P, M, N, Q>::operator*(const polynomial_matrix<M, M2, N, Q> & b) {
	struct polynomial_matrix<P, M2, N, Q> c;
	struct polynomial_matrix<P, M, N, Q> & a = *this;
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < M2; j++) {
			c[i][j] = 0;
			for (int k = 0; k < M; k++) {
				c[i][j] += a[i][j] * b[k][j]; 
			}
		}
	}
	return c;
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<P, 1, N, Q> polynomial_matrix<P, M, N, Q>::operator*(const polynomial_vector<M, N, Q> & b_) {
	struct polynomial_matrix<M, 1, N, Q> b = &b_;
	struct polynomial_matrix<P, M, N, Q> & a = *this;
	struct polynomial_matrix<P, 1, N, Q> c = a * b;
	return c;
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_vector<P, N, Q> polynomial_matrix<P, M, N, Q>::operator*() {
	if (M == 1) {
		polynomial_vector<P, N, Q> b;
		polynomial_matrix<P, M, N, Q> & a = *this;
		for (int i = 0; i < P; i++) {
			b[i] = a[0][i];
		}
		return b;
	} else {
		throw std::domain_error("Matrix to Vector conversion requirer column matrix (polynomial_matrix<P, M, N, Q>::operator*)");
	}
}