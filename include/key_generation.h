#ifndef __key_generation_
#define __key_generation_

#include <polynomial.h>

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q>
struct secret_key {
	struct polynomial_matrix<K, L, N, Q> A;
	struct polynomial_vector<K, N, Q> t;
	struct polynomial_vector<L, N, Q> s1;
	struct polynomial_vector<K, N, Q> s2;
};

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q>
struct public_key {
	struct polynomial_matrix<K, L, N, Q> A;
	struct polynomial_vector<K, N, Q> t;
};

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q>
struct key_pair {
	struct public_key<K, L, N, Q> public_key;
	struct secret_key<K, L, N, Q> secret_key;
};

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int ETA>
struct key_pair<K, L, N, Q> generate_key_pair();

#include <key_generation.cc>

#endif