#ifndef __key_generation_
#define __key_generation_

#include <signature_scheme_utils.h>
#include <common/packing.h>
#include <common/key_generation.h>
#include <polynomial.h>

namespace tcc {

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int ETA, unsigned int D>
	struct key_pair<K, L, N, Q, W> generate_key_pair();

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int ETA, unsigned int D>
	struct key_pair<K, L, N, Q, W> generate_key_pair(byte zeta[SEED_ZETA_SIZE]);

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int ETA, unsigned int D>
	void generate_key_pair(byte * output_public_key, byte * output_secret_key);

}

#include <key_generation.cc>

#endif