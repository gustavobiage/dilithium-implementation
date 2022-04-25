#ifndef __common_dilithium_
#define __common_dilithium_

#include <common/polynomial.h>

typedef unsigned char byte;

namespace tcc {
	const unsigned int Q_ = 58728449;

	// Seed sizes
	const unsigned int SEED_P_SIZE = 32;
	const unsigned int SEED_P_LINE_SIZE = 64;
	const unsigned int SEED_K_SIZE = 32;
	const unsigned int SEED_TR_SIZE = 32;
	const unsigned int SEED_ZETA_SIZE = 32;

	extern const unsigned int K;
	extern const unsigned int L;

	// Packed sizes
	const unsigned int S1_PACKED_SIZE = L*96;
	const unsigned int S2_PACKED_SIZE = K*96;
	const unsigned int T0_PACKED_SIZE = K*416;
	const unsigned int T1_PACKED_SIZE = K*320;
	const unsigned int PUBLIC_KEY_SIZE = SEED_P_SIZE + T1_PACKED_SIZE;
	const unsigned int SECRET_KEY_SIZE = SEED_P_SIZE+SEED_K_SIZE+SEED_TR_SIZE+S1_PACKED_SIZE+S2_PACKED_SIZE+T0_PACKED_SIZE;

	template <unsigned int LENGTH, unsigned int N, unsigned int Q, unsigned int W>
	void pack_vector_s(struct polynomial_vector<LENGTH, N, Q, W> s, byte * output);

	template <unsigned int LENGTH, unsigned int N, unsigned int Q, unsigned int W>
	struct polynomial_vector<LENGTH, N, Q, W> unpack_vector_s(byte * input, int input_size);

}

#endif