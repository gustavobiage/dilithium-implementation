/* Included on header file */
#include <display.h>

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int ETA, unsigned int D>
struct tcc::key_pair<K, L, N, Q, W> tcc::generate_key_pair() {
	byte zeta[SEED_ZETA_SIZE];
	tcc::sample_random_bytes(zeta, SEED_ZETA_SIZE);
	return tcc::generate_key_pair<K, L, N, Q, W, ETA, D>(zeta);
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int ETA, unsigned int D>
struct tcc::key_pair<K, L, N, Q, W> tcc::generate_key_pair(byte zeta[SEED_ZETA_SIZE]) {
	byte sampled_bytes[SEED_P_SIZE + SEED_P_LINE_SIZE + SEED_K_SIZE];
	tcc::sample_bytes(zeta, SEED_ZETA_SIZE, sampled_bytes, SEED_P_SIZE + SEED_P_LINE_SIZE + SEED_K_SIZE);

	byte p[SEED_P_SIZE];
	memcpy(p, &sampled_bytes[0], SEED_P_SIZE);
	byte p_[SEED_P_LINE_SIZE];
	memcpy(p_, &sampled_bytes[SEED_P_SIZE], SEED_P_LINE_SIZE);
	byte k[SEED_K_SIZE];
	memcpy(k, &sampled_bytes[SEED_P_SIZE + SEED_P_LINE_SIZE], SEED_K_SIZE);

	// ExpandA returns A in NTT form (A hat)
	struct tcc::ntt_polynomial_matrix<K, L, N, Q, W> A_hat = tcc::expandA<K, L, N, Q, W>(p);

	// Applies modular reduction (reduce32) for sampled A_hat and for all coefficients of polynomials, add Q if coefficient is negative.
	for (int i = 0; i < K; i++) {
        for (int j = 0; j < L; j++) {
            for (int k = 0; k < N; k++) {
            	A_hat[i][j][k] = tcc::reduce32<Q>(A_hat[i][j][k]);
            	A_hat[i][j][k] = add(A_hat[i][j][k], Q, Q);
            }
        }
    }

	struct tcc::polynomial_vector<L, N, Q, W> s1 = tcc::expandS<L, N, Q, W, ETA>(p_, 0);
	struct tcc::polynomial_vector<K, N, Q, W> s2 = tcc::expandS<K, N, Q, W, ETA>(p_, L);

	// For all coefficients of polynomials, add Q if coefficient is negative.
	for (int i = 0; i < L; i++) {
		for (int  j = 0; j < N/4; j++) {
			// printf("%d %d %d %d\n", s1[i][j*4+0], s1[i][j*4+1], s1[i][j*4+2], s1[i][j*4+3]);
			s1[i][j] = add(s1[i][j], Q, Q);
		}
	}

	// For all coefficients of polynomials, add Q if coefficient is negative.
	for (int i = 0; i < K; i++) {
		for (int  j = 0; j < N/4; j++) {
			s2[i][j] = add(s2[i][j], Q, Q);
		}
	}

	// Uses NTT on multiplication
	struct tcc::ntt_polynomial_vector<L, N, Q, W> s1_hat = s1.foward_transform();
	struct tcc::ntt_polynomial_vector<K, N, Q, W> As1_hat = (A_hat * s1_hat);
	struct tcc::polynomial_vector<K, N, Q, W> As1 = As1_hat.backward_transform();
	struct tcc::polynomial_vector<K, N, Q, W> t = As1 + s2;

	struct tcc::polynomial_vector<K, N, Q, W> t1, t0;
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N; j++) {
			std::pair<int32_t, int32_t> t_ = tcc::power_2_round<Q, D>(t[i][j]);
			t1[i][j] = t_.first;
			t0[i][j] = t_.second;
		}
	}

	byte pt1[SEED_P_SIZE + T1_PACKED_SIZE];
	memcpy(&pt1[0], p, SEED_P_SIZE);
	byte t1_packed[T1_PACKED_SIZE];
	tcc::pack_vector_t1<K, N, Q, W>(t1, &pt1[SEED_P_SIZE]);
	byte tr[SEED_TR_SIZE];
	tcc::sample_bytes(pt1, SEED_P_SIZE + T1_PACKED_SIZE, tr, SEED_TR_SIZE);

	// Convert coefficients higher than Q/2 of s1 to negative values, as they should be stored.
	for (int i = 0; i < L; i++)
		for (int  j = 0; j < N/4; j++) 
			if (s1[i][j] > Q/2)
				s1[i][j] = s1[i][j] - Q;

	// Convert coefficients higher than Q/2 of s2 to negative values, as they should be stored.
	for (int i = 0; i < K; i++)
		for (int  j = 0; j < N/4; j++) 
			if (s2[i][j] > Q/2)
				s2[i][j] = s2[i][j] - Q;

	struct tcc::public_key<K, L, N, Q, W> pk;
	memcpy(pk.p, p, SEED_P_SIZE);
	pk.t1 = t1;
	struct tcc::secret_key<K, L, N, Q, W> sk;
	memcpy(sk.p, p, SEED_P_SIZE);
	memcpy(sk.k, k, SEED_K_SIZE);
	memcpy(sk.tr, tr, SEED_TR_SIZE);
	sk.s1 = s1;
	sk.s2 = s2;
	sk.t0 = t0;

	struct tcc::key_pair<K, L, N, Q, W> key_pair;
	key_pair.public_key = pk;
	key_pair.secret_key = sk;

	return key_pair;
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int ETA, unsigned int D>
void tcc::generate_key_pair(byte * output_public_key, byte * output_secret_key) {
	struct key_pair<K, L, N, Q, W> key_pair = generate_key_pair<K, L, N, Q, W, ETA, D>();
	pack_public_key<K, L, N, Q, W>(key_pair.public_key, output_public_key);
	pack_secret_key<K, L, N, Q, W, D>(key_pair.secret_key, output_secret_key);
}
