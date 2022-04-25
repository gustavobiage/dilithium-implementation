
bool little_endian() {
	volatile uint32_t i=0x01234567;
	// return 0 for big endian, 1 for little endian.
	return (*((uint8_t*)(&i))) == 0x67;
}

/*
 * offset: 0    offset: 2   offset: 4    offset: 6
 *  |------------||------------||-----------||-----------|
 *  |      10   -||      10    ||      10   ||      10   |
 *  |------------||------------||-----------||-----------|
 *
 *  |--------| |--------| |--------| |--------| |--------|
 *  |   8    | |   8    | |   8    | |   8    | |   8    |
 *  |--------| |--------| |--------| |--------| |--------|
 */

template <unsigned int K, unsigned int N, unsigned int Q, unsigned int W>
void tcc::pack_vector_t1(struct tcc::polynomial_vector<K, N, Q, W> t1, byte * output) {
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N/4; j++) {
			output[320*i + 5*j + 0] = (t1[i][4*j + 0] >> 0);
			output[320*i + 5*j + 1] = (t1[i][4*j + 0] >> 8) | (t1[i][4*j + 1] << 2);
			output[320*i + 5*j + 2] = (t1[i][4*j + 1] >> 6) | (t1[i][4*j + 2] << 4);
			output[320*i + 5*j + 3] = (t1[i][4*j + 2] >> 4) | (t1[i][4*j + 3] << 6);
			output[320*i + 5*j + 4] = (t1[i][4*j + 3] >> 2);
		}
	}
}

template <unsigned int K, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<K, N, Q, W> tcc::unpack_vector_t1(byte * input) {
	struct tcc::polynomial_vector<K, N, Q, W> t1;
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N/4; j++) {
			t1[i][j*4+0] = (((uint32_t) input[i*320 + j*5 + 0] >> 0) | ((uint32_t) input[i*320 + j*5 + 1] << 8)) & 0x3FF;
			t1[i][j*4+1] = (((uint32_t) input[i*320 + j*5 + 1] >> 2) | ((uint32_t) input[i*320 + j*5 + 2] << 6)) & 0x3FF;
			t1[i][j*4+2] = (((uint32_t) input[i*320 + j*5 + 2] >> 4) | ((uint32_t) input[i*320 + j*5 + 3] << 4)) & 0x3FF;
			t1[i][j*4+3] = (((uint32_t) input[i*320 + j*5 + 3] >> 6) | ((uint32_t) input[i*320 + j*5 + 4] << 2)) & 0x3FF;
		}
	}
	return t1;
}

template <unsigned int K, unsigned int N, unsigned int Q, unsigned int W, unsigned int D>
void tcc::pack_vector_t0(struct tcc::polynomial_vector<K, N, Q, W> t0, byte * output) {
	int32_t aux[8];
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < N/8; j++) {
			aux[0] = (1 << (D-1)) - t0[i][8*j + 0];
			aux[1] = (1 << (D-1)) - t0[i][8*j + 1];
			aux[2] = (1 << (D-1)) - t0[i][8*j + 2];
			aux[3] = (1 << (D-1)) - t0[i][8*j + 3];
			aux[4] = (1 << (D-1)) - t0[i][8*j + 4];
			aux[5] = (1 << (D-1)) - t0[i][8*j + 5];
			aux[6] = (1 << (D-1)) - t0[i][8*j + 6];
			aux[7] = (1 << (D-1)) - t0[i][8*j + 7];

			output[416*i + 13*j+ 0]   =  aux[0];
			output[416*i + 13*j+ 1]   =  aux[0] >>  8;
			output[416*i + 13*j+ 1]  |=  aux[1] <<  5;
			output[416*i + 13*j+ 2]   =  aux[1] >>  3;
			output[416*i + 13*j+ 3]   =  aux[1] >> 11;
			output[416*i + 13*j+ 3]  |=  aux[2] <<  2;
			output[416*i + 13*j+ 4]   =  aux[2] >>  6;
			output[416*i + 13*j+ 4]  |=  aux[3] <<  7;
			output[416*i + 13*j+ 5]   =  aux[3] >>  1;
			output[416*i + 13*j+ 6]   =  aux[3] >>  9;
			output[416*i + 13*j+ 6]  |=  aux[4] <<  4;
			output[416*i + 13*j+ 7]   =  aux[4] >>  4;
			output[416*i + 13*j+ 8]   =  aux[4] >> 12;
			output[416*i + 13*j+ 8]  |=  aux[5] <<  1;
			output[416*i + 13*j+ 9]   =  aux[5] >>  7;
			output[416*i + 13*j+ 9]  |=  aux[6] <<  6;
			output[416*i + 13*j+ 10]  =  aux[6] >>  2;
			output[416*i + 13*j+ 11]  =  aux[6] >> 10;
			output[416*i + 13*j+ 11] |=  aux[7] <<  3;
			output[416*i + 13*j+ 12]  =  aux[7] >>  5;
		}
	}
}

template <unsigned int K, unsigned int N, unsigned int Q, unsigned int W, unsigned int D>
struct tcc::polynomial_vector<K, N, Q, W> tcc::unpack_vector_t0(byte * input) {
	struct tcc::polynomial_vector<K, N, Q, W> t0;
	for (int i = 0; i < K; i++) {
		for(int j = 0; j < N/8; j++) {
			t0[i][8*j + 0]  = input[416*i + 13*j + 0];
			t0[i][8*j + 0] |= (uint32_t) input[416*i + 13*j + 1] << 8;
			t0[i][8*j + 0] &= 0x1FFF;

			t0[i][8*j + 1]  = input[416*i + 13*j + 1] >> 5;
			t0[i][8*j + 1] |= (uint32_t) input[416*i + 13*j + 2] << 3;
			t0[i][8*j + 1] |= (uint32_t) input[416*i + 13*j + 3] << 11;
			t0[i][8*j + 1] &= 0x1FFF;

			t0[i][8*j + 2]  = input[416*i + 13*j + 3] >> 2;
			t0[i][8*j + 2] |= (uint32_t) input[416*i + 13*j + 4] << 6;
			t0[i][8*j + 2] &= 0x1FFF;

			t0[i][8*j + 3]  = input[416*i + 13*j + 4] >> 7;
			t0[i][8*j + 3] |= (uint32_t) input[416*i + 13*j + 5] << 1;
			t0[i][8*j + 3] |= (uint32_t) input[416*i + 13*j + 6] << 9;
			t0[i][8*j + 3] &= 0x1FFF;

			t0[i][8*j + 4]  = input[416*i + 13*j + 6] >> 4;
			t0[i][8*j + 4] |= (uint32_t) input[416*i + 13*j + 7] << 4;
			t0[i][8*j + 4] |= (uint32_t) input[416*i + 13*j + 8] << 12;
			t0[i][8*j + 4] &= 0x1FFF;

			t0[i][8*j + 5]  = input[416*i + 13*j + 8] >> 1;
			t0[i][8*j + 5] |= (uint32_t) input[416*i + 13*j + 9] << 7;
			t0[i][8*j + 5] &= 0x1FFF;

			t0[i][8*j + 6]  = input[416*i + 13*j + 9] >> 6;
			t0[i][8*j + 6] |= (uint32_t) input[416*i + 13*j + 10] << 2;
			t0[i][8*j + 6] |= (uint32_t) input[416*i + 13*j + 11] << 10;
			t0[i][8*j + 6] &= 0x1FFF;

			t0[i][8*j + 7]  = input[416*i + 13*j + 11] >> 3;
			t0[i][8*j + 7] |= (uint32_t) input[416*i + 13*j + 12]<< 5;
			t0[i][8*j + 7] &= 0x1FFF;

			t0[i][8*j + 0] = (1 << (D-1)) - t0[i][8*j + 0];
			t0[i][8*j + 1] = (1 << (D-1)) - t0[i][8*j + 1];
			t0[i][8*j + 2] = (1 << (D-1)) - t0[i][8*j + 2];
			t0[i][8*j + 3] = (1 << (D-1)) - t0[i][8*j + 3];
			t0[i][8*j + 4] = (1 << (D-1)) - t0[i][8*j + 4];
			t0[i][8*j + 5] = (1 << (D-1)) - t0[i][8*j + 5];
			t0[i][8*j + 6] = (1 << (D-1)) - t0[i][8*j + 6];
			t0[i][8*j + 7] = (1 << (D-1)) - t0[i][8*j + 7];
		}
	}
	return t0;
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
void tcc::pack_public_key(struct tcc::public_key<K, L, N, Q, W> public_key, byte * output) {
	/*
	 * 1. SEED_P
	 * 2. T1_PACKED
	 */
	unsigned int offset = 0;
	memcpy(&output[offset], public_key.p, SEED_P_SIZE);
	offset += SEED_P_SIZE;
	tcc::pack_vector_t1<K, N, Q, W>(public_key.t1, &output[offset]);
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int ETA>
struct tcc::public_key<K, L, N, Q, W> tcc::unpack_public_key(byte * input) {
	/*
	 * 1. SEED_P
	 * 2. T1_PACKED
	 */
	struct tcc::public_key<K, L, N, Q, W> public_key;
	unsigned int offset = 0;
	memcpy(public_key.p, &input[offset], SEED_P_SIZE);
	offset += SEED_P_SIZE;
	public_key.t1 = tcc::unpack_vector_t1<K, N, Q, W>(&input[offset]);
	return public_key;
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int D>
void tcc::pack_secret_key(struct tcc::secret_key<K, L, N, Q, W> secret_key, byte * output) {
	/*
	 * 1. SEED_P
	 * 2. SEED_K
	 * 3. SEED_TR
	 * 4. S1_PACKED
	 * 5. S2_PACKED
	 * 6. T0_PACKED
	 */
	unsigned int offset = 0;
	memcpy(&output[offset], secret_key.p, SEED_P_SIZE);
	offset += SEED_P_SIZE;
	memcpy(&output[offset], secret_key.k, SEED_K_SIZE);
	offset += SEED_K_SIZE;
	memcpy(&output[offset], secret_key.tr, SEED_TR_SIZE);
	offset += SEED_TR_SIZE;
	tcc::pack_vector_s<L, N, Q, W>(secret_key.s1, &output[offset]);
	offset += S1_PACKED_SIZE;
	tcc::pack_vector_s<K, N, Q, W>(secret_key.s2, &output[offset]);
	offset += S2_PACKED_SIZE;
	tcc::pack_vector_t0<K, N, Q, W, D>(secret_key.t0, &output[offset]);
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W, unsigned int D>
struct tcc::secret_key<K, L, N, Q, W> tcc::unpack_secret_key(byte * input) {
	/*
	 * 1. SEED_P
	 * 2. SEED_K
	 * 3. SEED_TR
	 * 4. S1_PACKED
	 * 5. S2_PACKED
	 * 6. T0_PACKED
	 */
	struct tcc::secret_key<K, L, N, Q, W> secret_key;
	unsigned int offset = 0;
	memcpy(secret_key.p, &input[offset], SEED_P_SIZE);
	offset += SEED_P_SIZE;
	memcpy(secret_key.k, &input[offset], SEED_K_SIZE);
	offset += SEED_K_SIZE;
	memcpy(secret_key.tr, &input[offset], SEED_TR_SIZE);
	offset += SEED_TR_SIZE;
	secret_key.s1 = tcc::unpack_vector_s<L, N, Q, W>(&input[offset], tcc::S1_PACKED_SIZE);
	offset += S1_PACKED_SIZE;
	secret_key.s2 = tcc::unpack_vector_s<K, N, Q, W>(&input[offset], tcc::S2_PACKED_SIZE);
	offset += S2_PACKED_SIZE;
	secret_key.t0 = tcc::unpack_vector_t0<K, N, Q, W, D>(&input[offset]);
	return secret_key;
}