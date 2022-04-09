/* Included on header file */

template <unsigned int BETA, unsigned int GAMMA1, unsigned int GAMMA2, unsigned int K, unsigned int L, unsigned int N, unsigned int Q>
struct signature<L, N, Q> sign(struct secret_key<K, L, N, Q> & sk, byte message[], int message_size) {
    #ifdef OUTPUT
        display_box("Generating signature with prameters:",
                "BETA", BETA,
                "GAMMA1", GAMMA1,
                "GAMMA2", GAMMA2,
                "K", K,
                "L", L,
                "N", N,
                "Q", Q);
    #endif

    REJECT:

    #ifdef OUTPUT
        display_status_header("Generating y vector");
    #endif

    polynomial_vector<L, N, Q> y;
    for (int i = 0; i < L; i++) {
        y[i] = static_cast<struct polynomial<N, Q>>(polynomial<N, GAMMA1>::generate_random_polynomial());
    }

    #ifdef OUTPUT
        display_status_result("OK!"); new_line();
    	display_status_header("Generating Ay vector");
    #endif

    polynomial_vector<K, N, Q> Ay = sk.A * y;

    #ifdef OUTPUT
        display_status_result("OK!"); new_line();
        display_status_header("Generating w1 vector");
    #endif

    polynomial_vector<K, N, Q> w1;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            w1[i][j] = high_order_bits<Q>(Ay[i][j], 2*GAMMA2);
        }
    }

    #ifdef OUTPUT
        display_status_result("OK!"); new_line();
        display_status_header("Generating c polynomial from hash");
    #endif

    assert((K * N * 4) % 8 == 0);

    // message to be signed concatenated with w1 (high order polynomial vector).
    unsigned int w1_packed_size = (K * N * 4)/8;
    assert(message_size == 48); // Simple implementation require a 48 byte message
    
    byte w1_packed[w1_packed_size];
    bit_packing<K, N, Q>(w1, w1_packed);

    polynomial<N, Q> c = sample_in_ball<N, Q>(message, w1_packed, w1_packed_size, 60);

    #ifdef OUTPUT
        display_status_result("OK!"); new_line();
        display_status_header("Generating z vector");
    #endif

    polynomial_vector<L, N, Q> z = y + (c * sk.s1);

    #ifdef OUTPUT
        display_status_result("OK!"); new_line();
        display_status_header("Constructing c * s2");
    #endif

    polynomial_vector<K, N, Q> cs2 = c * sk.s2;

    #ifdef OUTPUT
        bool correct_c2 = true;
        int coefficient;
        for (int i = 0; i < K && correct_c2; i++) {
            for (int j = 0; j < N && correct_c2; j++) {
                coefficient = (cs2[i][j] + BETA) % Q;
                // coefficient < BETA or coefficient + BETA < 2 * BETA
                if (coefficient >= (int) 2 * BETA) {
                    correct_c2 = false;
                }
            }
        }
        if (correct_c2) {
            display_status_result("OK!"); new_line();
        } else {
            display_status_result("ERROR!"); new_line();
        }
        display_status_header("testing for rejection");
    #endif

    for (int i = 0; i < L; i++) {
        for (int j = 0; j < N; j++) {
            if (z[i][j] > GAMMA1 - BETA) {
                #ifdef OUTPUT
                    display_status_result("ERROR!"); new_line();
                #endif
                goto REJECT;
            }
        }
    }

    polynomial_vector<K, N, Q> z2 = Ay - cs2;

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            int lower_bits = low_order_bits<Q>(z2[i][j], 2*GAMMA2);
            if (lower_bits < 0) {
                lower_bits = -lower_bits;
            }
            if (lower_bits >= (int)(GAMMA2 - BETA)) {
                #ifdef OUTPUT
                    display_status_result("ERROR!"); new_line();
                #endif
                goto REJECT;
            }
        }
    }

    #ifdef OUTPUT
        display_status_result("OK!"); new_line();
    #endif

    struct signature<L, N, Q> signature;
    signature.z = z;
    signature.c = c;
    return signature;
}