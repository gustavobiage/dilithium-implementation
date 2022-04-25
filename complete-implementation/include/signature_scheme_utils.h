#ifndef __signature_scheme_utils_
#define __signature_scheme_utils_

// dilithium implementation
#include <common/dilithium.h>
#include <polynomial.h>
#include <ntt_polynomial.h>
// c standard library
#include <utility>
// crypto pp
#include <cryptopp/osrng.h>
#include <cryptopp/shake.h>
#include <cryptopp/keccak.h>

namespace tcc {

	const int SHAKE128_BLOCK_SIZE = 168;

	template <unsigned int Q>
	int32_t high_order_bits(int32_t, int32_t);

	template <unsigned int Q>
	int32_t low_order_bits(int32_t, int32_t);

	template <unsigned int Q>
	int32_t cmod(int32_t, int32_t);

	template <unsigned int Q, unsigned int D>
	std::pair<int32_t, int32_t> power_2_round(int32_t r);

	template <unsigned int Q>
	std::pair<int32_t, int32_t> decompose(int32_t, int32_t);

	template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
	void bit_packing(struct polynomial_vector<M, N, Q, W>, byte *);

	template <unsigned int N, unsigned int Q, unsigned int W>
	struct polynomial<N, Q, W> sample_in_ball(byte *, byte *, int, int);

	template <unsigned int N, unsigned int Q, unsigned int W>
	struct ntt_polynomial<N, Q, W> sample_ntt_polynomial(byte shake_input[], unsigned int shake_input_size);

	template <unsigned int ETA>
	bool accept_sampled_eta(int32_t value);

	template <unsigned int ETA>
	int sampled_eta(int32_t value);

	/* rename latter */
	template <unsigned int N, unsigned int Q, unsigned int W, unsigned int ETA>
	struct polynomial<N, Q, W> sample_polynomial_eta(byte shake_input[], int shake_input_size);

	void sample_random_bytes(byte * output, int output_size);

	void sample_bytes(byte * input, int input_size, byte * output, int output_size);

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
	struct ntt_polynomial_matrix<K, L, N, Q, W> expandA(byte seed[]);

	template <unsigned int LENGTH, unsigned int N, unsigned int Q, unsigned int W, unsigned int ETA>
	struct polynomial_vector<LENGTH, N, Q, W> expandS(byte seed[], int nounce_offset);
}


#include <signature_scheme_utils.cc>

#endif