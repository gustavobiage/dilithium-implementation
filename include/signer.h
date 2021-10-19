#ifndef __signer_
#define __signer_

#include <key_generation.h>
#include <cryptopp/shake.h>
#include <polynomial.h>
#include <random>
#include <utility>
#include <stdexcept>

typedef unsigned char byte;

template <unsigned int L, unsigned int N, unsigned int Q>
struct signature {
    polynomial_vector<L, N, Q> z;
    polynomial<N, Q> c;
};

template <unsigned int Q>
int32_t high_order_bits(int32_t, int32_t);

template <unsigned int Q>
std::pair<int32_t, int32_t> decompose(int32_t, int32_t);

template <unsigned int M, unsigned int N, unsigned int Q>
void bit_packing(byte *, int, polynomial_vector<M, N, Q>);

template <unsigned int N, unsigned int Q>
polynomial<N, Q> sample_in_ball(byte *, int);

template <unsigned int BETA, unsigned int GAMMA1, unsigned int GAMMA2, unsigned int K, unsigned int L, unsigned int N, unsigned int Q>
struct signature<L, N, Q> sign(struct secret_key<K, L, N, Q> &, byte *, int);

#include <signer.cc>

#endif