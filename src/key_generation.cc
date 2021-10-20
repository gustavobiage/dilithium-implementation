/* Included on header file */
#include <display.h>

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int ETA>
struct key_pair<K, L, N, Q> generate_key_pair() {
#ifdef OUTPUT
	display_box("Generating key pair with prameters:",
				"K", K,
				"L", L,
				"Q", Q,
				"ETA", ETA);
	display_status_header("Generating A matrix");
#endif

	struct polynomial_matrix<K, L, N, Q> A;
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < L; j++) {
			A[i][j] = polynomial<N, Q>::generate_random_polynomial();
		}
	}

#ifdef OUTPUT
	display_status_result("OK!"); new_line();
	display_status_header("Generating s1 vector");
#endif

	struct polynomial_vector<L, N, Q> s1;
	for (int i = 0; i < L; i++) {
		/* S1 belongs to the polynomial ring Rq, although its random values are gerated in such a way that its coeficcients are smaller then ETA*/
		s1[i] = static_cast<struct polynomial<N, Q>>(polynomial<N, ETA>::generate_random_polynomial());
	}

#ifdef OUTPUT
	display_status_result("OK!"); new_line();
	display_status_header("Generating s2 vector");
#endif

	struct polynomial_vector<K, N, Q> s2;
	for (int i = 0; i < K; i++) {
		/* S1 belongs to the polynomial ring Rq, although its random values are gerated in such a way that its coeficcients are smaller then ETA*/
		s2[i] = static_cast<struct polynomial<N, Q>>(polynomial<N, ETA>::generate_random_polynomial());;
	}

#ifdef OUTPUT
	display_status_result("OK!"); new_line();
	display_status_header("Generating t vector");
#endif

	struct polynomial_vector<K, N, Q> t = (A * s1) + s2;;

#ifdef OUTPUT
	display_status_result("OK!"); new_line();
#endif

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

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q>
struct secret_key<K, L, N, Q> & secret_key<K, L, N, Q>::operator=(const struct secret_key<K, L, N, Q> & b) {
	struct secret_key<K, L, N, Q> & a = *this;
	a.A = b.A;
	a.t = b.t;
	a.s1 = b.s1;
	a.s2 = b.s2;
	return a;
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q>
struct public_key<K, L, N, Q> & public_key<K, L, N, Q>::operator=(const struct public_key<K, L, N, Q> & b) {
	struct public_key<K, L, N, Q> & a = *this;
	a.A = b.A;
	a.t = b.t;
	return a;
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q>
struct key_pair<K, L, N, Q> & key_pair<K, L, N, Q>::operator=(const struct key_pair<K, L, N, Q> & b) {
	struct key_pair<K, L, N, Q> & a = *this;
	a.public_key = b.public_key;
	a.secret_key = b.secret_key;
	return a;
}