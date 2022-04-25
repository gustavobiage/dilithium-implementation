#ifndef __common_key_generation_
#define __common_key_generation_

namespace tcc {

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W> struct public_key;
	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W> struct secret_key;
	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W> struct key_pair;

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
	struct public_key {
		byte p[SEED_P_SIZE];
		struct polynomial_vector<K, N, Q, W> t1;

		public_key() { }

		public_key(const struct public_key<K, L, N, Q, W> &);

		public_key<K, L, N, Q, W> & operator=(const struct public_key<K, L, N, Q, W> &);
	};

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
	struct secret_key {
		byte p[SEED_P_SIZE];
		byte k[SEED_K_SIZE];
		byte tr[SEED_TR_SIZE];
		struct polynomial_vector<L, N, Q, W> s1;
		struct polynomial_vector<K, N, Q, W> s2;
		struct polynomial_vector<K, N, Q, W> t0;

		secret_key() { }

		secret_key(const struct secret_key<K, L, N, Q, W> &);

		struct secret_key<K, L, N, Q, W> & operator=(const struct secret_key<K, L, N, Q, W> &);
	};

	template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
	struct key_pair {
		struct public_key<K, L, N, Q, W> public_key;
		struct secret_key<K, L, N, Q, W> secret_key;

		key_pair() { }

		key_pair(const struct key_pair<K, L, N, Q, W> &);

		struct key_pair<K, L, N, Q, W> & operator=(const struct key_pair<K, L, N, Q, W> &);
	};

}

#include <common/key_generation.cc>

#endif