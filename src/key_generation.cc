#include <key_generation.h>

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q>
struct key_pair<K, L, N, Q> generate_key_pair() {
	struct polynomial_matrix<K, L, N, Q> A;
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < L; j++) {
			A[i][j] = polynomial<N, Q>::generate_random_polynomial();
		}
	}

	struct polynomial_vector<L, N, Q> s1;
	for (int i = 0; i < L; i++) {
		s1[i] = polynomial<N, Q>::generate_random_polynomial();
	}

	struct polynomial_vector<K, N, Q> s2;
	for (int i = 0; i < K; i++) {
		s2[i] = polynomial<N, Q>::generate_random_polynomial();
	}

	struct polynomial_vector<K, N, Q> t = *((A * &s1) + &s2);

	struct public_key<K, L, N, Q> pk;
	pk.A = A;
	pk.t = t;

	struct secret_key<K, L, N, Q> sk;
	sk.A = A;
	sk.t = t;
	sk.s1 = s1;
	sk.s2 = s2;

	struct key_pair<K, L, N, Q> key_pair;
	key_pair.public_key = pk;
	key_pair.secret_key = sk;

	return key_pair;
}
