#ifndef __polynomial_
#define __polynomial_

#include <common_polynomial.h>
#include <stdlib.h>
#include <stdint.h>
#include <uniform_distribution.h>
#include <stdexcept>
#include <math.h>

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

	/* Constructor allows static cast between ntt_polynomial */
	polynomial(const struct ntt_polynomial<N, Q, W> &);

	struct polynomial<N, Q, W> operator=(const int32_t);

	struct polynomial<N, Q, W> operator+(const struct polynomial<N, Q, W> &);

	struct polynomial<N, Q, W> operator-(const struct polynomial<N, Q, W> &);

	struct polynomial<N, Q, W> operator*(const struct polynomial<N, Q, W> &);

	struct polynomial<N, Q, W> operator+=(const struct polynomial<N, Q, W> &);

	int32_t & operator[](int);

	int32_t operator[](int) const;

	static struct uniform_distribution<Q> uniform_distribution;

	static struct polynomial<N, Q, W> generate_random_polynomial();

	struct ntt_polynomial<N, Q, W> foward_transform() const;

	static zeta_array<N, Q, W> zetas;
private:
	int32_t coefficients[N];
};

#include <polynomial.cc>

#endif