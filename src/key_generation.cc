/* Included on header file */
#include <display.h>

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int ETA>
struct key_pair<K, L, N, Q> generate_key_pair() {
	display_box("Generating key pair with prameters:",
				"K", K,
				"L", L,
				"Q", Q,
				"ETA", ETA);

	display_status_header("Generating A matrix");

	struct polynomial_matrix<K, L, N, Q> A;
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < L; j++) {
			A[i][j] = polynomial<N, Q>::generate_random_polynomial();
		}
	}

	display_status_result("OK!"); new_line();
	display_status_header("Generating s1 vector");

	struct polynomial_vector<L, N, Q> s1;
	for (int i = 0; i < L; i++) {
		/* S1 belongs to the polynomial ring Rq, although its random values are gerated in such a way that its coeficcients are smaller then ETA*/
		s1[i] = static_cast<struct polynomial<N, Q>>(polynomial<N, ETA>::generate_random_polynomial());
	}

	display_status_result("OK!"); new_line();
	display_status_header("Generating s2 vector");

	struct polynomial_vector<K, N, Q> s2;
	for (int i = 0; i < K; i++) {
		/* S1 belongs to the polynomial ring Rq, although its random values are gerated in such a way that its coeficcients are smaller then ETA*/
		s2[i] = static_cast<struct polynomial<N, Q>>(polynomial<N, ETA>::generate_random_polynomial());;
	}

	display_status_result("OK!"); new_line();
	display_status_header("Generating t vector");

	struct polynomial_vector<K, N, Q> t = (A * s1) + s2;;

	display_status_result("OK!"); new_line();

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
