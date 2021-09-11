#ifndef __key_generation_
#define __key_generation_

#include <polynomial.h>

template <unsigned int K, unsigned int L, unsigned int Q>
struct secret_key {
	struct polynomial_matrix<K, L, Q> A;
	struct polynomial_vector<K, Q> t;
	struct polynomial_vector<L, Q> s1;
	struct polynomial_vector<K, Q> s2;
};

template <unsigned int K, unsigned int L, unsigned int Q>
struct public_key {
	struct polynomial_matrix<K, L, Q> A;
	struct polynomial_vector<K, Q> t;
};

template <unsigned int K, unsigned int L, unsigned int Q>
struct key_pair {
	struct public_key<K, L, Q> public_key;
	struct secret_key<K, L, Q> secret_key;
};

template <unsigned int K, unsigned int L, unsigned int Q>
key_pair<K, L, Q> generate_key_pair();

template <unsigned int Q>
struct polynomial<Q> generate_random_polynomial();

#endif