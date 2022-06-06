
/* Included on header file */

template <unsigned int BETA, unsigned int GAMMA1, unsigned int GAMMA2, unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int D, unsigned int OMEGA, unsigned int TAU>
bool tcc::verify(struct signature<K, L, N, Q, W> signature, byte * message, int message_size, struct public_key<K, L, N, Q, W> public_key) {
    // ExpandA returns A in NTT form (A hat)
    struct tcc::ntt_polynomial_matrix<K, L, N, Q, W> A_hat = tcc::expandA<K, L, N, Q, W>(public_key.p);
    // Applies modular reduction (reduce32) for sampled A_hat and for all coefficients of polynomials, add Q if coefficient is negative.
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < L; j++) {
            for (int k = 0; k < N; k++) {
                A_hat[i][j][k] = tcc::reduce32<Q>(A_hat[i][j][k]);
                A_hat[i][j][k] = add(A_hat[i][j][k], Q, Q);
            }
        }
    }

    byte pt1[SEED_P_SIZE + T1_PACKED_SIZE];
    memcpy(pt1, public_key.p, SEED_P_SIZE);
    pack_vector_t1<K, N, Q, W>(public_key.t1, &pt1[SEED_P_SIZE]);

    byte hpt1[SAMPLE_32_BYTES];
    tcc::sample_bytes(pt1, SEED_P_SIZE + T1_PACKED_SIZE, hpt1, SAMPLE_32_BYTES); // TODO create sampled_bytes_size

    byte hpt1M[SAMPLE_32_BYTES + message_size];
    memcpy(hpt1M, hpt1, SAMPLE_32_BYTES);
    memcpy(&hpt1M[SAMPLE_32_BYTES], message, message_size);

    byte mu[MESSAGE_MU_SIZE];
    tcc::sample_bytes(hpt1M, SAMPLE_32_BYTES + message_size, mu, MESSAGE_MU_SIZE);
    printf("BEGIN MU============\n");
    for (int i = 0; i < MESSAGE_MU_SIZE/4; i++) {
        printf("%d %d %d %d\n", mu[4*i + 0], mu[4*i + 1], mu[4*i + 2], mu[4*i + 3]);
    }
    printf("END MU============\n");

    polynomial<N, Q, W> c = tcc::sample_in_ball<N, Q, W>(signature.c_til, TAU);

    printf("BEGIN C===========\n");
    for (int i = 0; i < N/4; i++) {
        printf("%d %d %d %d\n", c[4*i + 0], c[4*i + 1], c[4*i + 2], c[4*i + 3]);
    }
    printf("END C===========\n");

    ntt_polynomial_vector<L, N, Q, W> z_hat = signature.z.foward_transform();
    ntt_polynomial_vector<K, N, Q, W> Az_hat = A_hat * z_hat;
    polynomial_vector<K, N, Q, W> Az = Az_hat.backward_transform();

    // printf("BEGIN Az========\n");
    // for (int i = 0; i < K; i++) {
    //     for (int j = 0; j < N/4; j++) {
    //         int32_t aux[4];
    //         aux[0] = Az[i][4*j + 0];
    //         aux[1] = Az[i][4*j + 1];
    //         aux[2] = Az[i][4*j + 2];
    //         aux[3] = Az[i][4*j + 3];
    //         printf("%d %d %d %d\n", aux[0], aux[1], aux[2], aux[3]);
    //     }
    // }
    // printf("END Az========\n");

    ntt_polynomial<N, Q, W> c_hat = c.foward_transform();
    ntt_polynomial_vector<K, N, Q, W> t1_hat = public_key.t1.foward_transform();
    ntt_polynomial_vector<K, N, Q, W> ct1_hat = c_hat * t1_hat;
    polynomial_vector<K, N, Q, W> ct1 = ct1_hat.backward_transform();
    polynomial_vector<K, N, Q, W> ct1_2d = ct1 * (1 << D);

    polynomial_vector<K, N, Q, W> w1_ = Az - ct1_2d;

    printf("BEGIN AZ - CT1========\n");
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N/4; j++) {
            int32_t aux[4];
            aux[0] = w1_[i][4*j + 0];
            aux[1] = w1_[i][4*j + 1];
            aux[2] = w1_[i][4*j + 2];
            aux[3] = w1_[i][4*j + 3];
            printf("%d %d %d %d\n", aux[0], aux[1], aux[2], aux[3]);
        }
    }
    printf("END AZ - CT1========\n");

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            w1_[i][j] = tcc::use_hint<Q>(signature.h[i][j], w1_[i][j], 2*GAMMA2);
        }
    }

    printf("BEGIN use hint========\n");
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N/4; j++) {
            int32_t aux[4];
            aux[0] = w1_[i][4*j + 0];
            aux[1] = w1_[i][4*j + 1];
            aux[2] = w1_[i][4*j + 2];
            aux[3] = w1_[i][4*j + 3];
            printf("%d %d %d %d\n", aux[0], aux[1], aux[2], aux[3]);
        }
    }
    printf("END use hint========\n");
    // WE JUST FINISHED CHECKING "use hint"

    byte muW1[MESSAGE_MU_SIZE + W1_PACKED_SIZE];
    memcpy(muW1, mu, MESSAGE_MU_SIZE);
    tcc::pack_vector_w1<K, N, Q, W>(w1_, &muW1[MESSAGE_MU_SIZE]);

    byte c_til_[SAMPLED_C_TIL_SIZE];
    tcc::sample_bytes(muW1, MESSAGE_MU_SIZE + W1_PACKED_SIZE, c_til_, SAMPLED_C_TIL_SIZE);

    printf("BEGIN C~=========\n");
    for (int i = 0; i < SAMPLED_C_TIL_SIZE/4; i++) {
        printf("%d %d %d %d\n", c_til_[4*i + 0], c_til_[4*i + 1], c_til_[4*i + 2], c_til_[4*i + 3]);
    }
    printf("END C~=========\n");

    bool c_til_are_equal = true;
    for (int i = 0; i < SAMPLED_C_TIL_SIZE && c_til_are_equal; i++) {
        c_til_are_equal = (c_til_[i] == signature.c_til[i]);
    }

    int32_t hints_equal_1_count = 0;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            hints_equal_1_count += signature.h[i][j];
        }
    }
    printf("BLABLABLA: %d %d %d", signature.z.norm_power_2() >= (GAMMA1 - BETA), c_til_are_equal, hints_equal_1_count <= OMEGA);
    return signature.z.norm_power_2() < (GAMMA1 - BETA) && c_til_are_equal && hints_equal_1_count <= OMEGA;
}