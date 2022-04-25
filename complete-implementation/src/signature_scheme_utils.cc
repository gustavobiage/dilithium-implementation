/* Included on header file */

template <unsigned int Q>
int32_t tcc::high_order_bits(int32_t w, int32_t gamma) {
    std::pair<int32_t, int32_t> pair = decompose<Q>(w, gamma);
    return pair.first;
}

template <unsigned int Q>
int32_t tcc::low_order_bits(int32_t w, int32_t gamma) {
    std::pair<int32_t, int32_t> pair = decompose<Q>(w, gamma);
    return pair.second;
}

template <unsigned int Q>
int32_t tcc::cmod(int32_t r, int32_t alpha) {
    if (r < 0 || r >= Q) {
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
        r = r % alpha;
        r = r - (alpha/2 + 1);
        int shift = (r >> 31);
        r = r + (shift & alpha);
        r = r - (alpha/2 - 1);
        return r;
    }
}

template <unsigned int Q, unsigned int D>
std::pair<int32_t, int32_t> tcc::power_2_round(int32_t r) {
    r = r % Q;
    int32_t r0, r1;
    r0 = cmod<Q>(r, (1 << D));
    return std::make_pair( ((r - r0) >> D), r0);
}

/*
 * For element w, compute high and low bits w0, w1 such
 * that w = w1* alpha + w0 with -alpha/2 < w0 <= alpha/2 except
 * if w = Q-1 where w1 = 0 and w0 = -1. Assumes a to be standard
 * representative.
 */
template <unsigned int Q>
std::pair<int32_t, int32_t> tcc::decompose(int32_t w, int32_t alpha) {
    w = w % Q;
    int32_t w0, w1;
    w0 = cmod<Q>(w, alpha);
    if (w == Q - 1) {
        w1 = 0;
        w0 = -1;
    } else {
        w1 = (w - w0) / alpha;
    }
    return std::make_pair(w1, w0);
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
void tcc::bit_packing(polynomial_vector<M, N, Q, W> w1, byte * buffer) {
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
template <unsigned int N, unsigned int Q, unsigned int W>
tcc::polynomial<N, Q, W> tcc::sample_in_ball(byte * mu, byte * w1, int w1_packed_size, int h) {
    tcc::polynomial<N, Q, W> c;
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

const unsigned int NUMBER_OF_BLOCKS = 6;

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial<N, Q, W> tcc::sample_ntt_polynomial(byte shake_input[], unsigned int shake_input_size) {
    // Beware: sample in ball uses SHAKE256 and sample polynomial uses SHAKE128
    CryptoPP::SHAKE128 shake(NUMBER_OF_BLOCKS * SHAKE128_BLOCK_SIZE);
    shake.Update((const byte *) &shake_input[0], shake_input_size);
    byte stream[NUMBER_OF_BLOCKS * SHAKE128_BLOCK_SIZE];
    shake.Final(stream);
    struct tcc::ntt_polynomial<N, Q, W> poly;
    int i, j, value, pos;
    i = pos = 0;

    while (i < N) {
        do {
            j = 0;
            value = 0;
            while (j < 3) {
                if (pos >= NUMBER_OF_BLOCKS * SHAKE128_BLOCK_SIZE) {
                    // shake.Final(stream);
                    // pos = 0;
                    abort();
                }
                value = value | ((uint32_t) stream[pos++] << (j * 8));
                j++;    
            }
            value = value & 0x7FFFFF;
        } while (value >= Q);
        poly[i++] = value;
    }
    return poly;
}

template <unsigned int ETA>
bool tcc::accept_sampled_eta(int32_t value) {
    return (ETA == 2 && value < 15) || (ETA == 4 && value < 9);
}

template <unsigned int ETA>
int tcc::sampled_eta(int32_t value) {
    if (ETA == 2 && value < 15) {
        return value % 5;
    } else if (ETA == 4 && value < 9) {
        return value;
    }
    return -1;
}

/* rename latter */
template <unsigned int N, unsigned int Q, unsigned int W, unsigned int ETA>
struct tcc::polynomial<N, Q, W> tcc::sample_polynomial_eta(byte shake_input[], int shake_input_size) {
    // Beware: expandA sample vectors with SHAKE 128, expandS sample polynomials with SHAKE256
    CryptoPP::SHAKE256 shake(NUMBER_OF_BLOCKS * SHAKE256_BLOCK_SIZE);
    // CryptoPP::Keccak_256 shake;
    shake.Update((const byte *) &shake_input[0], shake_input_size);
    byte stream[NUMBER_OF_BLOCKS * SHAKE256_BLOCK_SIZE];
    shake.Final(stream);

    struct tcc::polynomial<N, Q, W> poly;
    int i, value, pos;
    i = pos = 0;
    while (i < N) {
        value = 0;
        if (pos >= NUMBER_OF_BLOCKS * SHAKE256_BLOCK_SIZE) {
            abort();
        }
        int32_t value = stream[pos] & 0xF;
        if (tcc::accept_sampled_eta<ETA>(value)) {
            poly[i++] = ETA - tcc::sampled_eta<ETA>(value);
        }
        value = (stream[pos++] >> 4) & 0xF;
        if (i < N && tcc::accept_sampled_eta<ETA>(value)) {
            poly[i++] = ETA - tcc::sampled_eta<ETA>(value);
        }
    }
    return poly;
}

void tcc::sample_random_bytes(byte * output, int output_size) {
    /* Generates random bytes using OS entropy */
    CryptoPP::OS_GenerateRandomBlock(false, (CryptoPP::byte *) output, output_size);
    FILE * f = fopen("/tmp/urandom", "wb");
    for (int i = 0; i < output_size; i++) {
        byte b = fputc(output[i], f);
        if (b == EOF) {
            abort();
        }
    }
    fclose(f);
}

void tcc::sample_bytes(byte * input, int input_size, byte * output, int output_size) {
    CryptoPP::SHAKE256 shake(output_size);
    shake.Update(input, input_size);
    shake.Final(output);
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial_matrix<K, L, N, Q, W> tcc::expandA(byte seed[]) {
    struct tcc::ntt_polynomial_matrix<K, L, N, Q, W> a;
    const unsigned int SHAKE_INPUT_SIZE = tcc::SEED_P_SIZE + 2;
    byte shake_input[SHAKE_INPUT_SIZE];
    memcpy(shake_input, seed, tcc::SEED_P_SIZE);
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < L; j++) {
            shake_input[tcc::SEED_P_SIZE] = j;
            shake_input[tcc::SEED_P_SIZE + 1] = i;
            a[i][j] = tcc::sample_ntt_polynomial<N, Q, W>(shake_input, SHAKE_INPUT_SIZE);
        }
    }
    return a;
}

template <unsigned int LENGTH, unsigned int N, unsigned int Q, unsigned int W, unsigned int ETA>
struct tcc::polynomial_vector<LENGTH, N, Q, W> tcc::expandS(byte seed[], int nounce_offset) {
    struct tcc::polynomial_vector<LENGTH, N, Q, W> s;
    const int SHAKE_INPUT_SIZE = tcc::SEED_P_LINE_SIZE + 2;
    byte shake_input[SHAKE_INPUT_SIZE];
    memcpy(shake_input, seed, tcc::SEED_P_LINE_SIZE);
    for (int i = 0; i < LENGTH; i++) {
        *((int16_t *) &shake_input[tcc::SEED_P_LINE_SIZE]) = nounce_offset + i;
        s[i] = tcc::sample_polynomial_eta<N, Q, W, ETA>(shake_input, SHAKE_INPUT_SIZE);
    }
    return s;
}