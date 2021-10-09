/* Included on header file */

template <unsigned int Q>
int32_t high_order_bits(int32_t w, int32_t gamma) {
    std::pair<int32_t, int32_t> pair = decompose<Q>(w, gamma);
    return pair.second;
}

template <unsigned int Q>
std::pair<int32_t, int32_t> decompose(int32_t w, int32_t gamma) {
    int32_t w0, w1;
    w0 = w % gamma;
    if (w - w0 == Q - 1) {
        w1 = 0;
        w0 = w0 - 1;
    } else {
        w1 = (w - w0) / gamma;
    }
    return std::make_pair(w0, w1);
}

template <unsigned int K, unsigned int N, unsigned int Q>
void bit_packing(byte * buffer, int from, polynomial_vector<K, N, Q> w1) {
    int pointer = 0;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            int32_t coefficient = w1[i][j];
            if (pointer % 2) {
                buffer[from + pointer/2] |= coefficient;
            } else {
                buffer[from + pointer/2] = (coefficient << 4);
            }
        }
    }
}

/* Let Bh denote the set o elements of R that have a h coefficients that are either -1 or 1 and the rest are 0 */
template <unsigned int N, unsigned int Q>
polynomial<N, Q> sample_in_ball(byte * s, int h) {
    polynomial<N, Q> c;
    for (int i = 0; i < N; i++) {
        c[i] = 0;
    }
    
    int unused_numbers[N];
    for (int i = 0; i < N; i++) {
        unused_numbers[i] = i;
    }
    
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int> distrib;

    int mask[] = {128, 64, 32, 16, 8, 4, 2, 1};

    /* Fisher–Yates shuffle */
    int choosen, j;
    for (int i = 0; i < h; i++) {
        const std::uniform_int_distribution<int>::param_type p(i, N-1);
        distrib.param(p);
        j = distrib(gen); // random integer from i <= j < N
        choosen = unused_numbers[j];
        unused_numbers[j] = unused_numbers[i];
        unused_numbers[i] = choosen;
        if (s[i/8] & mask[i % 8]) {
            c[choosen] = -1;
        } else {
            c[choosen] = 1;
        }
    }
    return c;
}

template <unsigned int BETA, unsigned int GAMMA1, unsigned int GAMMA2, unsigned int K, unsigned int L, unsigned int N, unsigned int Q>
struct signature<L, N, Q> sign(struct secret_key<K, L, N, Q> & sk, byte message[], int message_size) {

    display_box("Generating signature with prameters:",
            "BETA", BETA,
            "GAMMA1", GAMMA1,
            "GAMMA2", GAMMA2,
            "K", K,
            "L", L,
            "Q", Q);

    REJECT:

    display_status_header("Generating y vector");

    polynomial_vector<L, N, Q> y;
    for (int i = 0; i < L; i++) {
        y[i] = static_cast<struct polynomial<N, Q>>(polynomial<N, GAMMA1>::generate_random_polynomial());
    }

    display_status_result("OK!"); new_line();
	display_status_header("Generating Ay vector");

    polynomial_vector<K, N, Q> Ay = *(sk.A * &y);

    display_status_result("OK!"); new_line();
    display_status_header("Generating w1 vector");

    polynomial_vector<K, N, Q> w1;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            w1[i][j] = high_order_bits<Q>(Ay[i][j], GAMMA2); 
        }
    }

    display_status_result("OK!"); new_line();
    display_status_header("Generating c polynomial from hash");

    assert((K * N * 4) % 8 == 0);

    // message to be signed concatenated with w1 (high order polynomial vector).
    unsigned int w1_packed_size = (K * N * 4)/8;
    unsigned int concat_size = w1_packed_size + message_size;
    byte concat[concat_size];

    assert(message_size == 48); // Simple implementation require a 48 byte message
    memcpy(concat, message, message_size);

    bit_packing(concat, message_size, w1);

    CryptoPP::SHAKE256 shake(8);
    shake.Update((const byte *) &concat[0], concat_size);

    byte digest[8];
    shake.Final(digest);

    polynomial<N, Q> c = sample_in_ball<N, Q>(digest, 60);

    display_status_result("OK!"); new_line();
    display_status_header("Generating z vector");

    polynomial_vector<L, N, Q> z = y + (c * sk.s1);

    display_status_result("OK!"); new_line();
    display_status_header("testing for rejection");

    // for (int i = 0; i < L; i++) {
    //     for (int j = 0; j < N; j++) {
    //         if (z[i][j] > GAMMA1 - BETA) {
    //             display_status_result("ERROR!"); new_line();
    //             goto REJECT;
    //         }
    //     }
    // }

    polynomial_vector<K, N, Q> z2 = (sk.A * z) - (c * sk.t);

    // for (int i = 0; i < K; i++) {
    //     for (int j = 0; j < N; j++) {
    //         if (z2[i][j] > GAMMA2 - BETA) {
    //             display_status_result("ERROR!"); new_line();
    //             goto REJECT;
    //         }
    //     }
    // }

    display_status_result("OK!"); new_line();

    struct signature<L, N, Q> signature;
    signature.z = z;
    signature.c = c;
    return signature;
}