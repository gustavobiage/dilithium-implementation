#ifndef __common_dilithium_
#define __common_dilithium_

#include <common/polynomial.h>

typedef unsigned char byte;

namespace tcc {
	// extern const unsigned int OMEGA;

	const unsigned int Q_ = 58728449;


	// Seed sizes
	const unsigned int SAMPLE_32_BYTES = 32;
	const unsigned int SAMPLE_64_BYTES = 64;

	const unsigned int SEED_P_SIZE = SAMPLE_32_BYTES;
	const unsigned int SEED_P_LINE_SIZE = SAMPLE_64_BYTES;
	const unsigned int SEED_K_SIZE = SAMPLE_32_BYTES;
	const unsigned int SEED_TR_SIZE = SAMPLE_32_BYTES;
	const unsigned int SEED_ZETA_SIZE = SAMPLE_32_BYTES;
	const unsigned int MESSAGE_MU_SIZE = SAMPLE_64_BYTES;
	const unsigned int SAMPLED_C_TIL_SIZE = SAMPLE_32_BYTES;

	extern const unsigned int K;
	extern const unsigned int L;

	// Packed sizes
	const unsigned int S1_PACKED_SIZE = L*96;
	const unsigned int S2_PACKED_SIZE = K*96;
	const unsigned int T0_PACKED_SIZE = K*416;
	const unsigned int T1_PACKED_SIZE = K*320;
	const unsigned int W1_PACKED_SIZE = K*192; // This should be specific for Dilithium II
	const unsigned int Z_PACKED_SIZE  = L*576;
	const unsigned int PUBLIC_KEY_SIZE = SEED_P_SIZE + T1_PACKED_SIZE;
	const unsigned int SECRET_KEY_SIZE = SEED_P_SIZE+SEED_K_SIZE+SEED_TR_SIZE+S1_PACKED_SIZE+S2_PACKED_SIZE+T0_PACKED_SIZE;
	const unsigned int SIGNATURE_SIZE = SAMPLED_C_TIL_SIZE + Z_PACKED_SIZE + 80/*OMEGA*/ + K;

	template <unsigned int LENGTH, unsigned int N, unsigned int Q, unsigned int W>
	void pack_vector_s(struct polynomial_vector<LENGTH, N, Q, W> s, byte * output);

	template <unsigned int LENGTH, unsigned int N, unsigned int Q, unsigned int W>
	struct polynomial_vector<LENGTH, N, Q, W> unpack_vector_s(byte * input, int input_size);

	template <unsigned int K, unsigned int N, unsigned int Q, unsigned int W>
	void pack_vector_w1(struct tcc::polynomial_vector<K, N, Q, W> w0, byte * output);

	template <unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
	void pack_vector_z(struct tcc::polynomial_vector<L, N, Q, W> z, byte * output);
}

#endif