/* Included on header file */

template <unsigned int BETA, unsigned int GAMMA1, unsigned int GAMMA2, unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::signature<L, N, Q, W> tcc::sign(struct secret_key<K, L, N, Q, W> & sk, byte message[], int message_size) {

    // REJECT:

    // polynomial_matrix<K, L, N, Q> A = expandA(sk.p);
    // byte trM[/*TR SIZE*/ + message_size];
    // memcopy(trM, tr, /*TR SIZE*/);
    // memcopy(&trM[/*TR SIZE*/], message, message_size);

    /*TODO 'mu' with compute hash*/

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

    struct tcc::signature<L, N, Q, W> signature;
    // signature.z = z;
    // signature.c = c;
    return signature;
}