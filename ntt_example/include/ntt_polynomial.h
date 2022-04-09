#ifndef __ntt_polynomial_
#define __ntt_polynomial_

#include <common_polynomial.h>
#include <stdlib.h>
#include <stdint.h>
#include <polynomial.h>
#include <math.h>
#include <stdexcept>

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

	struct ntt_polynomial<N, Q, W> operator*(const struct ntt_polynomial<N, Q, W> &);

	int32_t & operator[](int);

	int32_t operator[](int) const;

	struct polynomial<N, Q, W> backward_transform() const;

private:
	static zeta_array<N, Q, W> zetas;

	int32_t coefficients[N];
};

#include <ntt_polynomial.cc>

#endif