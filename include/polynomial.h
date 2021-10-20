#ifndef __polynomial_
#define __polynomial_

#include <stdlib.h>
#include <stdint.h>
#include <math_utils.h>
#include <uniform_distribution.h>
#include <stdexcept>

template <unsigned int N, unsigned int Q> struct polynomial;
template <unsigned int M, unsigned int N, unsigned int Q> struct polynomial_vector;
template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q> struct polynomial_matrix;

/*
 * Polynomial in Ring Rq = Zq[X] / (X^n + 1)
 * Multiplication of polynomial can be represented as convolution operation in Zq^n lattice.
 */
template <unsigned int N, unsigned int Q>
struct polynomial {
	int32_t coefficients[N];

	polynomial() {}

	polynomial(const int [N]);

	template <unsigned int Q2>
	/* Constructor allows static cast between polynomial of different modulus */
	polynomial(const struct polynomial<N, Q2> &);

	struct polynomial<N, Q> operator=(const int32_t);

	struct polynomial<N, Q> operator+(const struct polynomial<N, Q> &);

	struct polynomial<N, Q> operator-(const struct polynomial<N, Q> &);

	struct polynomial<N, Q> operator*(const struct polynomial<N, Q> &);

	template <unsigned int M>
	struct polynomial_vector<M, N, Q> operator*(const struct polynomial_vector<M, N, Q> &);

	struct polynomial<N, Q> operator+=(const struct polynomial<N, Q> &);

	// Referencing returns vector
	struct polynomial_vector<1, N, Q> operator&();

	int32_t & operator[](int);

	int32_t operator[](int) const;

	static struct uniform_distribution<Q> uniform_distribution;

	static struct polynomial<N, Q> generate_random_polynomial();
};

template <unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_vector {
    struct polynomial<N, Q> vector[M];

	polynomial_vector() {}

	/* Constructor allows static cast between polynomial of different modulus */
	polynomial_vector(const struct polynomial_matrix<M, 1, N, Q> &);

	struct polynomial<N, Q> & operator[](int);

	struct polynomial<N, Q> operator[](int) const;

	struct polynomial_matrix<M, 1, N, Q> operator+(const struct polynomial_matrix<M, 1, N, Q>&);

	struct polynomial_matrix<M, 1, N, Q> operator-(const struct polynomial_matrix<M, 1, N, Q>&);

	template <unsigned int M2>
	struct polynomial_matrix<M, M2, N, Q> operator*(const struct polynomial_matrix<1, M2, N, Q>&);

	struct polynomial_vector<M, N, Q> & operator=(const struct polynomial_vector<M, N, Q> &);

	// Referencing returns matrix
	struct polynomial_matrix<M, 1, N, Q> operator&();
	// Referencing returns matrix
	struct polynomial_matrix<M, 1, N, Q> operator&() const;
};

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix {
    struct polynomial_vector<M, N, Q> matrix[P];

	polynomial_matrix() {}

	polynomial_matrix(const struct polynomial_vector<P, N, Q> &);

	struct polynomial_vector<M, N, Q> & operator[](int);

	struct polynomial_vector<M, N, Q> operator[](int) const;

	struct polynomial_matrix<P, M, N, Q> operator+(const struct polynomial_matrix<P, M, N, Q>&);

	struct polynomial_matrix<P, M, N, Q> operator+(const struct polynomial_vector<P, N, Q>&);

	struct polynomial_matrix<P, M, N, Q> operator-(const struct polynomial_matrix<P, M, N, Q>&);

	struct polynomial_matrix<P, M, N, Q> operator-(const struct polynomial_vector<P, N, Q>&);

	struct polynomial_matrix<P, M, N, Q> & operator=(const struct polynomial_matrix<P, M, N, Q>&);

	template <unsigned int M2>
	struct polynomial_matrix<P, M2, N, Q> operator*(const polynomial_matrix<M, M2, N, Q> &);

	struct polynomial_matrix<P, 1, N, Q> operator*(const polynomial_vector<M, N, Q> &);

	struct polynomial_vector<P, N, Q> operator*();
};

#include <polynomial.cc>
#include <polynomial_vector.cc>
#include <polynomial_matrix.cc>

#endif