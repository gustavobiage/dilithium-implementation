#include <signer.h>
#include <key_generation.h>
#include <signature_scheme_utils.h>

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q>
bool verify(struct signature<L, N, Q>, byte[], int, struct public_key<K, L, N, Q>);

#include <verifier.cc>
