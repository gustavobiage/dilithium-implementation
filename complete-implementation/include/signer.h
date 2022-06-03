#ifndef __signer_
#define __signer_

#include <common/key_generation.h>
#include <common/polynomial.h>
#include <common/signature.h>
#include <signature_scheme_utils.h>

#include <cryptopp/shake.h>
#include <random>
#include <stdexcept>

namespace tcc {
    template <unsigned int BETA, unsigned int GAMMA1, unsigned int GAMMA2, unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int OMEGA, unsigned int TAU>
    struct signature<K, L, N, Q, W> sign(struct secret_key<K, L, N, Q, W> &, byte[], int);
}


#include <signer.cc>

#endif