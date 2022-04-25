#ifndef __signer_
#define __signer_

#include <key_generation.h>
#include <cryptopp/shake.h>
#include <polynomial.h>
#include <random>
#include <stdexcept>
#include <signature_scheme_utils.h>

namespace tcc {

    template <unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
    struct signature {
        polynomial_vector<L, N, Q, W> z;
        polynomial<N, Q, W> c;
    };

    template <unsigned int BETA, unsigned int GAMMA1, unsigned int GAMMA2, unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
    struct signature<L, N, Q, W> sign(struct secret_key<K, L, N, Q, W> &, byte[], int);

}


#include <signer.cc>

#endif