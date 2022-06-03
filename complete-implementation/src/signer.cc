/* Included on header file */

#define ABORT() k++; goto RETRY;

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
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < N/4; j++) {
            add(y[i][j], Q, Q);
        }
    }

    ntt_polynomial_vector<L, N, Q, W> y_hat = y.foward_transform();

    ntt_polynomial_vector<K, N, Q, W> w_hat = (A_hat * y_hat);
    polynomial_vector<K, N, Q, W> w = w_hat.backward_transform();
    polynomial_vector<K, N, Q, W> w1;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            w1[i][j] = high_order_bits<Q>(w[i][j], 2*GAMMA2);
        }
    }
    for (int i = 0; i < L; i++) {
        for (int j = 0; j  < N/4; j++) {
            printf("%d %d %d %d\n", w[i][4*j + 0], w[i][4*j + 1], w[i][4*j + 2], w[i][4*j + 3]);
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
    //==================================================== UNCOMENT IF NECESSERY
    // // For all coefficients of polynomials, add Q if coefficient is negative.
    // for (int i = 0; i < L; i++) {
    //     for (int  j = 0; j < N; j++) {
    //         s1[i][j] = add(s1[i][j], Q, Q);
    //     }
    // }
    // // For all coefficients of polynomials, add Q if coefficient is negative.
    for (int i = 0; i < K; i++) {
        for (int  j = 0; j < N/4; j++) {
            s2[i][j] = add(s2[i][j], Q, Q);
        }
    }
    for (int i = 0; i < N; i++) { // Check if some cases has mistakes
        c[i] = add(c[i], Q, Q);
    }
    //=========================================================================

    ntt_polynomial<N, Q, W> c_hat = c.foward_transform();
    ntt_polynomial_vector<L, N, Q, W> s1_hat = s1.foward_transform();
    ntt_polynomial_vector<L, N, Q, W> cs1_hat = (c_hat * s1_hat);
    polynomial_vector<L, N, Q, W> cs1 = cs1_hat.backward_transform();
    // for (int i = 0; i < K; i++) {
    //     for (int j = 0; j < N/4; j++) {
    //         printf("%d %d %d %d\n", cs1[i][4*j + 0], cs1[i][4*j + 1], cs1[i][4*j + 2], cs1[i][4*j + 3]);
    //     }
    // }

    z = y + cs1;
    // for (int i = 0; i < K; i++) {
    //     for (int j = 0; j < N/4; j++) {
    //         printf("%d %d %d %d\n", z[i][4*j + 0], z[i][4*j + 1], z[i][4*j + 2], z[i][4*j + 3]);
    //     }
    // }

    ntt_polynomial_vector<K, N, Q, W> s2_hat = s2.foward_transform();
    ntt_polynomial_vector<K, N, Q, W> cs2_hat = (c_hat * s2_hat);
    polynomial_vector<K, N, Q, W> cs2 = cs2_hat.backward_transform();
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N/4; j++) {
            printf("%d %d %d %d\n", cs2[i][4*j + 0], cs2[i][4*j + 1], cs2[i][4*j + 2], cs2[i][4*j + 3]);
        }
    }
    // for (int i = 0; i < L; i++) {
    //     for (int j = 0; j  < N/4; j++) {
    //         printf("%d %d %d %d\n", y[i][4*j + 0], y[i][4*j + 1], y[i][4*j + 2], y[i][4*j + 3]);
    //     }
    // }
    polynomial_vector<K, N, Q, W> w_minus_cs2 = w - cs2;
    polynomial_vector<K, N, Q, W> r0;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            r0[i][j] = low_order_bits<Q>(w_minus_cs2[i][j], 2*GAMMA2);
        }
    }

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N/4; j++) {
            int32_t aux[4];
            aux[0] = r0[i][4*j + 0];
            aux[1] = r0[i][4*j + 1];
            aux[2] = r0[i][4*j + 2];
            aux[3] = r0[i][4*j + 3];
            for (int k = 0; k < 4; k++) {
                if (aux[k] < 0) {
                    aux[k] = aux[k] + Q;
                }
            }
            printf("%d %d %d %d\n", aux[0], aux[1], aux[2], aux[3]);
        }
    }
    // =========================== TEST UNTIL HERE =============
    struct tcc::signature<K, L, N, Q, W> signature;
    return signature;


    if (z.norm() >= GAMMA1 - BETA || r0.norm() >= GAMMA2 - BETA) {
        ABORT();
    }


    ntt_polynomial_vector<K, N, Q, W> t0_hat = secret_key.t0.foward_transform();
    ntt_polynomial_vector<K, N, Q, W> ct0_hat = (c_hat * t0_hat);
    polynomial_vector<K, N, Q, W> ct0 = ct0_hat.backward_transform();

    // TODO: Make hint 
    int hints_equal_1_count = 0;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            hints_equal_1_count += (h[i][j] == 1);
        }
    }

    if (ct0.norm() >= GAMMA2 || hints_equal_1_count > OMEGA) {
        ABORT();
    }

    // struct tcc::signature<K, L, N, Q, W> signature;
    memcpy(signature.c_til, c_til, SAMPLED_C_TIL_SIZE);
    signature.z = z;
    signature.h = h;
    return signature;

    // struct ntt_polynomial_vector<L, N, Q> s1_hat = sk.s1.fowardTransformation();
    // struct ntt_polynomial_vector<K, N, Q> s2_hat = sk.s2.fowardTransformation();

    // polynomial_vector<K, N, Q> Ay = sk.A * y;

    // TO BE CONTINUED

    // polynomial_vector<K, N, Q> w1;
    // for (int i = 0; i < K; i++) {
    //     for (int j = 0; j < N; j++) {
    //         w1[i][j] = high_order_bits<Q>(Ay[i][j], 2*GAMMA2);
    //     }
    // }

    // assert((K * N * 4) % 8 == 0);

    // // message to be signed concatenated with w1 (high order polynomial vector).
    // unsigned int w1_packed_size = (K * N * 4)/8;
    // assert(message_size == 48); // Simple implementation require a 48 byte message
    
    // byte w1_packed[w1_packed_size];
    // bit_packing<K, N, Q>(w1, w1_packed);

    // polynomial<N, Q> c = sample_in_ball<N, Q>(message, w1_packed, w1_packed_size, 60);

    // polynomial_vector<L, N, Q> z = y + (c * sk.s1);
    // polynomial_vector<K, N, Q> cs2 = c * sk.s2;

    // for (int i = 0; i < L; i++) {
    //     for (int j = 0; j < N; j++) {
    //         if (z[i][j] > GAMMA1 - BETA) {
    //             goto REJECT;
    //         }
    //     }
    // }

    // polynomial_vector<K, N, Q> z2 = Ay - cs2;

    // for (int i = 0; i < K; i++) {
    //     for (int j = 0; j < N; j++) {
    //         int lower_bits = low_order_bits<Q>(z2[i][j], 2*GAMMA2);
    //         if (lower_bits < 0) {
    //             lower_bits = -lower_bits;
    //         }
    //         if (lower_bits >= (int)(GAMMA2 - BETA)) {
    //             goto REJECT;
    //         }
    //     }
    // }

    // struct tcc::signature<L, N, Q, W> signature;
    // signature.z = z;
    // signature.c = c;
    // return signature;
}