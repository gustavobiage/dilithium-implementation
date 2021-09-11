#ifndef __polynomial_
#define __polynomial_

#include <stdlib.h>
#include <stdint.h>
#include <math_utils.h>

template <unsigned int Q> struct polynomial;
template <unsigned int N, unsigned int Q> struct polynomial_vector;
template <unsigned int M, unsigned int N, unsigned int Q> struct polynomial_matrix;

// TODO: make clear that is a polynomial in polynomial ring q
template <unsigned int Q>
struct polynomial {
	int32_t coefficients[Q];

	struct polynomial& operator=(const struct polynomial&);

	struct polynomial& operator+(const struct polynomial&);

	struct polynomial& operator*(const struct polynomial&);

	struct polynomial& operator/(const struct polynomial&);
};

template<unsigned int N, unsigned int Q>
struct polynomial_vector {
    struct polynomial<Q> vector[N];

	struct polynomial<Q>& operator[](int);
	// Referencing returns matrix
	struct polynomial_matrix<N, 1, Q>& operator&();
};

template<unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix {
    struct polynomial_vector<N, Q> matrix[M];

	struct polynomial_vector<N, Q>& operator[](int);

	struct polynomial_matrix<M, N, Q>& operator+(const struct polynomial_matrix&);

	template <unsigned int M2, unsigned int N2>
	struct polynomial_matrix<M, N2, Q>& operator*(const struct polynomial_matrix<M2, N2, Q>&);
};

template<unsigned int Q>
struct polynomial<Q> generate_random_polynomial();
#endif