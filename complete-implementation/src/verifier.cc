
/* Included on header file */

template <unsigned int BETA, unsigned int GAMMA1, unsigned int GAMMA2, unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
bool tcc::verify(struct signature<L, N, Q, W> signature, byte message[], int message_size, struct public_key<K, L, N, Q, W> public_key) {

    // struct polynomial_vector<K, N, Q> z2 = (public_key.A * signature.z) - (signature.c * public_key.t);
    // struct polynomial_vector<K, N, Q> w1_;
    // for (int i = 0; i < K; i++) {
    //     for (int j = 0; j < N; j++) {
    //         w1_[i][j] = high_order_bits<Q>(z2[i][j], 2*GAMMA2);
    //     }
    // }

    // assert((K * N * 4) % 8  == 0);
    // assert(message_size == 48); // Simple implementation require a 48 byte message

    // int w1_packed_size =  (K * N * 4)/8;
    // byte w1_packed[w1_packed_size];
    // bit_packing<K, N, Q>(w1_, w1_packed);

    // // Accepts if:
    // //     1. all the coefficients of z are less than γ1−β
    // for (int i = 0; i < L; i++) {
    //     for (int j = 0; j < N; j++) {
    //         if (signature.z[i][j] >= GAMMA1 - BETA) {
    //             #ifdef OUTPUT
    //                 display_status_result("ERROR!"); new_line();
    //             #endif
    //             return false;
    //         }
    //     }
    // }

    // //     2. c is the hash of the message and w1'
    // polynomial<N, Q> c_ = sample_in_ball<N, Q>(message, w1_packed, w1_packed_size, 60);
    // for (int i = 0; i < N; i++) {
    //     if (signature.c[i] != c_[i]) {
    //         return false;
    //     }
    // }

    return true;
}