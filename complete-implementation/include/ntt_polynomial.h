#ifndef __ntt_polynomial_
#define __ntt_polynomial_

#include <common/polynomial.h>
#include <stdlib.h>
#include <stdint.h>
#include <polynomial.h>
#include <math.h>
#include <stdexcept>

namespace tcc {

	/*
	 * Polynomial in Ring Rq = Zq[X] / (X^n + 1) on NTT format
	 * Multiplication of polynomial can done with the pointwise multiplication operation.
	 */
	template <unsigned int N, unsigned int Q, unsigned int W>
	struct ntt_polynomial {
	public:
		ntt_polynomial() {}

		ntt_polynomial(const int [N]);

		/* Constructor allows static cast between polynomial and ntt polynomial*/
		ntt_polynomial(const struct polynomial<N, Q, W> &);

		template <unsigned int Q2>
		/* Constructor allows static cast between ntt_polynomial of different modulus */
		ntt_polynomial(const struct ntt_polynomial<N, Q2, W> &);

		struct ntt_polynomial<N, Q, W> operator=(const int32_t);

		struct ntt_polynomial<N, Q, W> operator+(const struct ntt_polynomial<N, Q, W> &) const;

		struct ntt_polynomial<N, Q, W> operator-(const struct ntt_polynomial<N, Q, W> &) const;

		struct ntt_polynomial<N, Q, W> operator*(const struct ntt_polynomial<N, Q, W> &);

		template <unsigned int M>
		struct ntt_polynomial_vector<M, N, Q, W> operator*(const struct ntt_polynomial_vector<M, N, Q, W> &);

		int32_t & operator[](int);

		int32_t operator[](int) const;

		struct polynomial<N, Q, W> backward_transform() const;

	private:
		static zeta_array<N, Q, W> zetas;

		int32_t coefficients[N];
	};


	template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
	struct ntt_polynomial_vector {
	public:
		ntt_polynomial_vector() {}

		ntt_polynomial_vector(const struct ntt_polynomial_vector<M, N, Q, W> &);
		
		/* Constructor allows static cast between polynomial of different modulus */
		ntt_polynomial_vector(const struct ntt_polynomial_matrix<M, 1, N, Q, W> &);

		struct ntt_polynomial<N, Q, W> & operator[](int);

		struct ntt_polynomial<N, Q, W> operator[](int) const;

		struct ntt_polynomial_vector<M, N, Q, W> operator+(const struct ntt_polynomial_vector<M, N, Q, W>&) const;

		struct ntt_polynomial_matrix<M, 1, N, Q, W> operator+(const struct ntt_polynomial_matrix<M, 1, N, Q, W>&) const;

		struct ntt_polynomial_vector<M, N, Q, W> operator-(const struct ntt_polynomial_vector<M, N, Q, W>&) const;

		struct ntt_polynomial_matrix<M, 1, N, Q, W> operator-(const struct ntt_polynomial_matrix<M, 1, N, Q, W>&) const;

		template <unsigned int M2>
		struct ntt_polynomial_matrix<M, M2, N, Q, W> operator*(const struct ntt_polynomial_matrix<1, M2, N, Q, W>&) const;

		// Referencing returns matrix
		struct ntt_polynomial_matrix<M, 1, N, Q, W> operator&() const;

		struct polynomial_vector<M, N, Q, W> backward_transform() const;

	private:
	    struct ntt_polynomial<N, Q, W> polynomials[M];
	};

	template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
	struct ntt_polynomial_matrix {
	public:
		ntt_polynomial_matrix() {}

		ntt_polynomial_matrix(const struct ntt_polynomial_vector<P, N, Q, W> &);

		ntt_polynomial_matrix(const struct ntt_polynomial_matrix<P, M, N, Q, W> &);

		struct ntt_polynomial_vector<M, N, Q, W> & operator[](int);

		struct ntt_polynomial_vector<M, N, Q, W> operator[](int) const;

		struct ntt_polynomial_matrix<P, 1, N, Q, W> operator+(const struct ntt_polynomial_vector<P, N, Q, W>&) const;

		struct ntt_polynomial_matrix<P, M, N, Q, W> operator+(const struct ntt_polynomial_matrix<P, M, N, Q, W>&) const;

		struct ntt_polynomial_matrix<P, 1, N, Q, W> operator-(const struct ntt_polynomial_vector<P, N, Q, W>&) const;

		struct ntt_polynomial_matrix<P, M, N, Q, W> operator-(const struct ntt_polynomial_matrix<P, M, N, Q, W>&) const;

		template <unsigned int M2>
		struct ntt_polynomial_matrix<P, M2, N, Q, W> operator*(const ntt_polynomial_matrix<M, M2, N, Q, W> &) const;

		struct ntt_polynomial_matrix<P, 1, N, Q, W> operator*(const ntt_polynomial_vector<M, N, Q, W> &) const;

		struct ntt_polynomial_vector<P, N, Q, W> operator*() const;

		struct polynomial_matrix<P, M, N, Q, W> backward_transform() const;
	private:
	    struct ntt_polynomial_vector<M, N, Q, W> vectors[P];
	};

}

#include <ntt_polynomial.cc>
#include <ntt_polynomial_vector.cc>
#include <ntt_polynomial_matrix.cc>

#endif