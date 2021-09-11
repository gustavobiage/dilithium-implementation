#ifndef __polynomial_
#define __polynomial_

#include <stdlib.h>
#include <stdint.h>
#include <math_utils.h>
#include <uniform_distribution.h>

// template <unsigned int N, unsigned int Q> struct polynomial;
template <unsigned int M, unsigned int N, unsigned int Q> struct polynomial_vector;
// template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q> struct polynomial_matrix;

// TODO: make clear that is a polynomial in polynomial ring q
template <unsigned int N, unsigned int Q>
struct polynomial {
	int32_t coefficients[Q];

	struct polynomial<N, Q>& operator=(const struct polynomial<N, Q> &);

	struct polynomial<N, Q>& operator+(const struct polynomial<N, Q> &);

	struct polynomial<N, Q>& operator*(const struct polynomial<N, Q> &);

	static struct uniform_distribution uniform_distribution;
};

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix {
    struct polynomial_vector<M, N, Q> matrix[P];

	struct polynomial_vector<M, N, Q> & operator[](int);

	struct polynomial_matrix<P, M, N, Q> & operator+(const struct polynomial_matrix&);

	template <unsigned int P2, unsigned int M2, unsigned int N2, unsigned int Q2, unsigned int P3, unsigned int M3, unsigned int N3, unsigned int Q3>
	struct polynomial_matrix<M3, P3, N3, Q3>& operator*(const struct polynomial_matrix<P2, M2, N2, Q2>&);
};

template <unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_vector {
    struct polynomial<N, Q> vector[M];

	struct polynomial<N, Q> & operator[](int);
	// Referencing returns matrix
	struct polynomial_matrix<M, 1, N, Q>& operator&();
};

template<unsigned int N, unsigned int Q>
struct polynomial<N, Q> generate_random_polynomial();
#endif