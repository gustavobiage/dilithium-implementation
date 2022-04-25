#include <signer.h>
#include <key_generation.h>
#include <signature_scheme_utils.h>

namespace tcc {
	
	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
	bool verify(struct signature<L, N, Q, W>, byte[], int, struct public_key<K, L, N, Q, W>);

}


#include <verifier.cc>
