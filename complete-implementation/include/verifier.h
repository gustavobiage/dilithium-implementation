#include <common/signature.h>
#include <common/key_generation.h>
#include <signature_scheme_utils.h>

namespace tcc {
	
	template <unsigned int BETA, unsigned int GAMMA1, unsigned int GAMMA2, unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int D, unsigned int TAU>
	bool verify(struct signature<K, L, N, Q, W>, byte *, int, struct public_key<K, L, N, Q, W>);

}


#include <verifier.cc>
