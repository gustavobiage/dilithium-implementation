#ifndef __polynomial_
#define __polynomial_

#include <stdlib.h>
#include <stdint.h>
#include <math_utils.h>
#include <uniform_distribution.h>

template <unsigned int N, unsigned int Q> struct polynomial;
template <unsigned int M, unsigned int N, unsigned int Q> struct polynomial_vector;
template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q> struct polynomial_matrix;

/*
 * Polynomial in Ring Rq = Zq[X] / (X^n + 1)
 * Multiplication of polynomial can be represented as convolution operation in Zq^n lattice.
 */
template <unsigned int N, unsigned int Q>
struct polynomial {
	int32_t coefficients[Q];

	polynomial() {}

	template <unsigned int Q2>
	/* Constructor allows static cast between polynomial of different modulus */
	polynomial(const struct polynomial<Q2, N> &);

	struct polynomial<N, Q>& operator=(const struct polynomial<N, Q> &);

	struct polynomial<N, Q>& operator+(const struct polynomial<N, Q> &);

	struct polynomial<N, Q>& operator*(const struct polynomial<N, Q> &);

	int32_t & operator[](int);

	static struct uniform_distribution<Q> uniform_distribution;

	static struct polynomial<N, Q> generate_random_polynomial();
};

template <unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_vector {
    struct polynomial<N, Q> vector[M];

	struct polynomial<N, Q> & operator[](int);

	struct polynomial<N, Q> & operator[](int) const;
	// Referencing returns matrix
	struct polynomial_matrix<M, 1, N, Q>& operator&();
};

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix {
    struct polynomial_vector<M, N, Q> matrix[P];

	struct polynomial_vector<M, N, Q> & operator[](int);

	struct polynomial_matrix<P, M, N, Q> & operator+(const struct polynomial_matrix&);

	template <unsigned int P2, unsigned int M2>
	struct polynomial_matrix<P, M2, N, Q> operator*(const polynomial_matrix<P2, M2, N, Q> & b);

	struct polynomial_vector<P, N, Q> operator*();
};

#include <polynomial.cc>
#include <polynomial_vector.cc>
#include <polynomial_matrix.cc>

#endif