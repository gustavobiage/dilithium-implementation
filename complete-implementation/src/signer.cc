/* Included on header file */

#define ABORT() k += L; goto RETRY;

template <unsigned int BETA, unsigned int GAMMA1, unsigned int GAMMA2, unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int OMEGA, unsigned int TAU>
struct tcc::signature<K, L, N, Q, W> tcc::sign(struct secret_key<K, L, N, Q, W> & secret_key, byte message[], int message_size) {
    // ExpandA returns A in NTT form (A hat)
    struct tcc::ntt_polynomial_matrix<K, L, N, Q, W> A_hat = tcc::expandA<K, L, N, Q, W>(secret_key.p);
    // Applies modular reduction (reduce32) for sampled A_hat and for all coefficients of polynomials, add Q if coefficient is negative.
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < L; j++) {
            for (int k = 0; k < N; k++) {
                A_hat[i][j][k] = tcc::reduce32<Q>(A_hat[i][j][k]);
                A_hat[i][j][k] = add(A_hat[i][j][k], Q, Q);
            }
        }
    }

    byte trM[SEED_TR_SIZE + message_size];
    memcpy(trM, secret_key.tr, SEED_TR_SIZE);
    memcpy(&trM[SEED_TR_SIZE], message, message_size);

    byte mu[MESSAGE_MU_SIZE];
    sample_bytes(trM, SEED_TR_SIZE + message_size, mu, MESSAGE_MU_SIZE);

    uint16_t k = 0;
    polynomial_vector<L, N, Q, W> z;
    polynomial_vector<K, N, Q, W> h;

    byte kMu[SEED_K_SIZE + MESSAGE_MU_SIZE];
    memcpy(kMu, secret_key.k, SEED_K_SIZE);
    memcpy(&kMu[SEED_K_SIZE], mu, MESSAGE_MU_SIZE);

    byte p_[SEED_P_LINE_SIZE];
    sample_bytes(kMu, SEED_K_SIZE + MESSAGE_MU_SIZE, p_, SEED_P_LINE_SIZE);

RETRY:

    polynomial_vector<L, N, Q, W> y = tcc::expand_mask<L, N, Q, W, GAMMA1>(p_, k);

    ntt_polynomial_vector<L, N, Q, W> y_hat = y.foward_transform();
    ntt_polynomial_vector<K, N, Q, W> w_hat = (A_hat * y_hat);
    polynomial_vector<K, N, Q, W> w = w_hat.backward_transform();
    polynomial_vector<K, N, Q, W> w1;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            w1[i][j] = high_order_bits<Q>(w[i][j], 2*GAMMA2);
        }
    }

    byte muW1[MESSAGE_MU_SIZE + W1_PACKED_SIZE];
    memcpy(muW1, mu, MESSAGE_MU_SIZE);
    tcc::pack_vector_w1(w1, &muW1[MESSAGE_MU_SIZE]);

    byte c_til[SAMPLED_C_TIL_SIZE];
    tcc::sample_bytes(muW1, MESSAGE_MU_SIZE + W1_PACKED_SIZE, c_til, SAMPLED_C_TIL_SIZE);

    polynomial<N, Q, W> c = tcc::sample_in_ball<N, Q, W>(c_til, TAU);

    polynomial_vector<L, N, Q, W> s1 = secret_key.s1;
    polynomial_vector<K, N, Q, W> s2 = secret_key.s2;

    // For all coefficients of polynomials, add Q if coefficient is negative.
    for (int i = 0; i < K; i++) {
        for (int  j = 0; j < N/4; j++) {
            s2[i][j] = add(s2[i][j], Q, Q);
        }
    }
    for (int i = 0; i < N; i++) {
        c[i] = add(c[i], Q, Q);
    }

    ntt_polynomial<N, Q, W> c_hat = c.foward_transform();
    ntt_polynomial_vector<L, N, Q, W> s1_hat = s1.foward_transform();
    ntt_polynomial_vector<L, N, Q, W> cs1_hat = (c_hat * s1_hat);
    polynomial_vector<L, N, Q, W> cs1 = cs1_hat.backward_transform();

    z = y + cs1;

    ntt_polynomial_vector<K, N, Q, W> s2_hat = s2.foward_transform();
    ntt_polynomial_vector<K, N, Q, W> cs2_hat = (c_hat * s2_hat);
    polynomial_vector<K, N, Q, W> cs2 = cs2_hat.backward_transform();

    polynomial_vector<K, N, Q, W> w_minus_cs2 = w - cs2;
    polynomial_vector<K, N, Q, W> r0;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            r0[i][j] = low_order_bits<Q>(w_minus_cs2[i][j], 2*GAMMA2);
        }
    }

    if (z.norm_power_2() >=  (GAMMA1 - BETA) ||
        r0.norm_power_2() >= (GAMMA2 - BETA)) {
        ABORT();
    }

    // Convert coefficients higher than Q/2 of s1 to negative values, as they should be stored.
    for (int i = 0; i < L; i++) {
        for (int  j = 0; j < N; j++) {
            if (z[i][j] > Q/2) {
                z[i][j] = z[i][j] - Q;
            }
        } 
    }

    ntt_polynomial_vector<K, N, Q, W> t0_hat = secret_key.t0.foward_transform();
    ntt_polynomial_vector<K, N, Q, W> ct0_hat = (c_hat * t0_hat);
    polynomial_vector<K, N, Q, W> ct0 = ct0_hat.backward_transform();
    polynomial_vector<K, N, Q, W> _ct0 = ct0 * -1;
    polynomial_vector<K, N, Q, W> w_minus_cs2_plus_ct0 = w_minus_cs2 + ct0;

    int hints_equal_1_count = 0;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            h[i][j] = tcc::make_hint<Q>(_ct0[i][j], w_minus_cs2_plus_ct0[i][j], 2*GAMMA2);
            hints_equal_1_count += h[i][j];
        }
    }

    if (ct0.norm_power_2() >= (int64_t) GAMMA2 * GAMMA2 || hints_equal_1_count > OMEGA) {
        ABORT();
    }

    struct tcc::signature<K, L, N, Q, W> signature;
    memcpy(signature.c_til, c_til, SAMPLED_C_TIL_SIZE);
    signature.z = z;
    signature.h = h;
    return signature;
}