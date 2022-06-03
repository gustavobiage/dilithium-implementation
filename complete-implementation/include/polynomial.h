#ifndef __polynomial_
#define __polynomial_

#include <common/polynomial.h>
#include <uniform_distribution.h>
#include <stdlib.h>
#include <stdexcept>

namespace tcc {

	/*
	 * Polynomial in Ring Rq = Zq[X] / (X^n + 1)
	 * Multiplication of polynomial can be represented as convolution operation in Zq^n lattice.
	 */
	template <unsigned int N, unsigned int Q, unsigned int W>
	struct polynomial {
	public:
		polynomial() {}

		polynomial(const int [N]);

		template <unsigned int Q2>
		/* Constructor allows static cast between polynomial of different modulus */
		polynomial(const struct polynomial<N, Q2, W> &);

		polynomial(const struct ntt_polynomial<N, Q, W> &);

		struct polynomial<N, Q, W> operator=(const int32_t);

		struct polynomial<N, Q, W> & operator=(const struct polynomial<N, Q, W> & b);

		struct polynomial<N, Q, W> operator+(const struct polynomial<N, Q, W> &);

		struct polynomial<N, Q, W> operator-(const struct polynomial<N, Q, W> &);

		struct polynomial<N, Q, W> operator*(const struct polynomial<N, Q, W> &);

		template <unsigned int M>
		struct polynomial_vector<M, N, Q, W> operator*(const struct polynomial_vector<M, N, Q, W> &);

		struct polynomial<N, Q, W> operator+=(const struct polynomial<N, Q, W> &);

		// Referencing returns vector
		struct polynomial_vector<1, N, Q, W> operator&();

		int32_t & operator[](int);

		int32_t operator[](int) const;

		static struct uniform_distribution<Q> uniform_distribution;

		static struct polynomial<N, Q, W> generate_random_polynomial();

		struct ntt_polynomial<N, Q, W> foward_transform() const;

		int32_t norm_power_2();

		static zeta_array<N, Q, W> zetas;
	private:
		int32_t coefficients[N];
	};

	template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
	struct polynomial_vector {
	public:
		polynomial_vector() {}

		polynomial_vector(const struct polynomial_vector<M, N, Q, W> &);

		/* Constructor allows static cast between polynomial of different modulus */
		polynomial_vector(const struct polynomial_matrix<M, 1, N, Q, W> &);

		struct polynomial<N, Q, W> & operator[](int);

		struct polynomial<N, Q, W> operator[](int) const;

		struct polynomial_vector<M, N, Q, W> & operator=(const struct polynomial_vector<M, N, Q, W> &);

		struct polynomial_matrix<M, 1, N, Q, W> operator+(const struct polynomial_matrix<M, 1, N, Q, W>&);

		struct polynomial_vector<M, N, Q, W> operator+(const struct polynomial_vector<M, N, Q, W>& b);

		struct polynomial_matrix<M, 1, N, Q, W> operator-(const struct polynomial_matrix<M, 1, N, Q, W>&) const;

		struct polynomial_vector<M, N, Q, W> operator-(const struct polynomial_vector<M, N, Q, W>& b) const;

		template <unsigned int M2>
		struct polynomial_matrix<M, M2, N, Q, W> operator*(const struct polynomial_matrix<1, M2, N, Q, W>&);

		// Referencing returns matrix
		struct polynomial_matrix<M, 1, N, Q, W> operator&();
		// Referencing returns matrix
		struct polynomial_matrix<M, 1, N, Q, W> operator&() const;

		struct ntt_polynomial_vector<M, N, Q, W> foward_transform() const;

		int32_t norm_power_2();
	private:
		struct polynomial<N, Q, W> vector[M];
	};

	template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
	struct polynomial_matrix {
	public:

		polynomial_matrix() {}

		polynomial_matrix(const struct polynomial_vector<P, N, Q, W> &);

		polynomial_matrix(const struct polynomial_matrix<P, M, N, Q, W>&);

		struct polynomial_vector<M, N, Q, W> & operator[](int);

		struct polynomial_vector<M, N, Q, W> operator[](int) const;

		struct polynomial_matrix<P, M, N, Q, W> operator+(const struct polynomial_matrix<P, M, N, Q, W>&) const;

		struct polynomial_matrix<P, M, N, Q, W> operator+(const struct polynomial_vector<P, N, Q, W>&) const;

		struct polynomial_matrix<P, M, N, Q, W> operator-(const struct polynomial_matrix<P, M, N, Q, W>&) const;

		struct polynomial_matrix<P, M, N, Q, W> operator-(const struct polynomial_vector<P, N, Q, W>&) const;

		template <unsigned int M2>
		struct polynomial_matrix<P, M2, N, Q, W> operator*(const polynomial_matrix<M, M2, N, Q, W> &) const;

		struct polynomial_matrix<P, 1, N, Q, W> operator*(const polynomial_vector<M, N, Q, W> &) const;

		struct polynomial_vector<P, N, Q, W> operator*();

		struct ntt_polynomial_matrix<P, M, N, Q, W> foward_transform() const;
	private:
	    struct polynomial_vector<M, N, Q, W> vectors[P];
	};

}

#include <polynomial.cc>
#include <polynomial_vector.cc>
#include <polynomial_matrix.cc>

#endif