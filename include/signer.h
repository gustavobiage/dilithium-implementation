#ifndef __signer_
#define __signer_

#include <key_generation.h>
#include <cryptopp/shake.h>
#include <polynomial.h>
#include <random>
#include <stdexcept>
#include <signature_scheme_utils.h>

template <unsigned int L, unsigned int N, unsigned int Q>
struct signature {
    polynomial_vector<L, N, Q> z;
    polynomial<N, Q> c;
};

template <unsigned int BETA, unsigned int GAMMA1, unsigned int GAMMA2, unsigned int K, unsigned int L, unsigned int N, unsigned int Q>
struct signature<L, N, Q> sign(struct secret_key<K, L, N, Q> &, byte *, int);

#include <signer.cc>

#endif