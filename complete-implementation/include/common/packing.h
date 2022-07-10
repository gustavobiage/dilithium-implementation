#ifndef __common_packing_
#define __common_packing_

#include <byteswap.h>
#include <polynomial.h>
#include <common/key_generation.h>
#include <common/signature.h>

namespace tcc {

	template <unsigned int K, unsigned int N, unsigned int Q, unsigned int W>
	void pack_vector_t1(struct polynomial_vector<K, N, Q, W> t1, byte * output);

	template <unsigned int K, unsigned int N, unsigned int Q, unsigned int W>
	struct polynomial_vector<K, N, Q, W> unpack_vector_t1(byte * input);

	template <unsigned int K, unsigned int N, unsigned int Q, unsigned int W, unsigned int D>
	void pack_vector_t0(struct polynomial_vector<K, N, Q, W> t0, byte * output);

	template <unsigned int K, unsigned int N, unsigned int Q, unsigned int W, unsigned int D>
	struct polynomial_vector<K, N, Q, W> unpack_vector_t0(byte * input);

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
	void pack_public_key(struct public_key<K, L, N, Q, W> public_key, byte * output);

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int ETA>
	struct public_key<K, L, N, Q, W> unpack_public_key(byte * input);

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int D>
	void pack_secret_key(struct secret_key<K, L, N, Q, W> secret_key, byte * output);

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int D>
	struct secret_key<K, L, N, Q, W> unpack_secret_key(byte * input);

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int OMEGA>
	void pack_signature(struct tcc::signature<K, L, N, Q, W> signature, byte * output);

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int OMEGA>
	struct signature<K, L, N, Q, W> unpack_signature(byte * input);
}

#include <common/packing.cc>

#endif