/* Included on header file */

template <unsigned int Q>
int32_t high_order_bits(int32_t w, int32_t gamma) {
    std::pair<int32_t, int32_t> pair = decompose<Q>(w, gamma);
    return pair.first;
}

template <unsigned int Q>
int32_t low_order_bits(int32_t w, int32_t gamma) {
    std::pair<int32_t, int32_t> pair = decompose<Q>(w, gamma);
    return pair.second;
}

template <unsigned int Q>
int32_t cmod(int32_t r, int32_t alpha) {
    if (r < 0 || r > Q) {
        char message[100];
        sprintf(message, "centralized reduction expects r (as %d) in interval [0, %d]", r, Q);
        throw std::domain_error(message);
    } else {
        /* https://d-nb.info/1204223297/34; Reference Implementation p254
         *
         * Instead we use the following well-known trick to compute the centralized remainder 
         * r′= r mod ± α where 0 ≤ r ≤ 3α/2. Subtracting α/2 + 1 from r yields a negative result
         * if and only if r ≤ α/2. Therefore, shifting this result arithmetically to the right by
         * 31 bits gives −1, i.e. the integer with all bits equal to 1, if r ≤ α/2 and 0 otherwise.
         * Then the logical AND of the shifted value and α is added to r and α/2 − 1 subtracted.
         * This results in r − α if r > α/2 and r if r ≤ α/2, i.e. the centralized remainder.
         */
        r = r - (alpha/2 + 1);
        int shift = (r >> 31);
        r = r + (shift & alpha);
        r = r - (alpha/2 - 1);
        return r;
    }
}

template <unsigned int Q>
std::pair<int32_t, int32_t> decompose(int32_t w, int32_t alpha) {
    w = w % Q;
    int32_t w0, w1;
    w0 = cmod<Q>(w, alpha);
    if (w - w0 == Q - 1) {
        w1 = 0;
        w0 = w0 - 1;
    } else {
        w1 = (w - w0) / alpha;
    }
    return std::make_pair(w1, w0);
}

template <unsigned int M, unsigned int N, unsigned int Q>
void bit_packing(polynomial_vector<M, N, Q> w1, byte * buffer) {
    int pointer = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            int32_t coefficient = w1[i][j];
            if (pointer % 2) {
                buffer[pointer/2] |= coefficient;
            } else {
                buffer[pointer/2] = (coefficient << 4);
            }
            pointer++;
        }
    }
}


const int SHAKE256_BLOCK_SIZE = 136;

/* Let Bh denote the set o elements of R that have a h coefficients that are either -1 or 1 and the rest are 0 */
template <unsigned int N, unsigned int Q>
polynomial<N, Q> sample_in_ball(byte * mu, byte * w1, int w1_packed_size, int h) {
    polynomial<N, Q> c;
    for (int i = 0; i < N; i++) {
        c[i] = 0;
    }

    const int message_size = 48;
    unsigned int concat_size = w1_packed_size + message_size;
    byte concat[concat_size];

    memcpy(concat, mu, message_size);
    memcpy(&concat[message_size], w1, w1_packed_size);

    /* https://pq-crystals.org/dilithium/data/dilithium-specification.pdf#cite.fisher-yates pg 15
     *
     * In each iteration of the for loop it uses rejection sampling on elements from {0,...,255}
     * until it gets a j ∈ {0,...,i}. An element in {0,...,255} is obtained by interpreting the
     * next byte of the random stream from SHAKE-256 as a number in this set.
     */
    CryptoPP::SHAKE256 shake(SHAKE256_BLOCK_SIZE);
    shake.Update((const byte *) &concat[0], concat_size);

    byte stream[SHAKE256_BLOCK_SIZE];
    shake.Final(stream);
 
    byte s[8];
    for (int i = 0; i < 8; i++) {
        s[i] = stream[i];
    }

    int unused_numbers[N];
    for (int i = 0; i < N; i++) {
        unused_numbers[i] = i;
    }

    int mask[] = {128, 64, 32, 16, 8, 4, 2, 1};

    int choosen, replace;
    int j, pos;
    pos = 8;

    /* Fisher–Yates shuffle - with SHAKE for generating coefficient indexes*/
    for (int i = 0; i < h; i++) {
        replace = N - 1 - i;
        do {
            if (pos >= SHAKE256_BLOCK_SIZE) {
                shake.Final(stream);
                pos = 0;
            }
            j = stream[pos++];
        } while (j > replace);

        choosen = unused_numbers[j];
        unused_numbers[j] = unused_numbers[replace];
        unused_numbers[replace] = choosen;
        if (s[i/8] & mask[i % 8]) {
            c[choosen] = -1;
        } else {
            c[choosen] = 1;
        }
    }
    return c;
}