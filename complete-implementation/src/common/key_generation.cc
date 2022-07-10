/* included in head file "common/key_generation.h" */

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
tcc::public_key<K, L, N, Q, W>::public_key(const struct tcc::public_key<K, L, N, Q, W> & b) {
	struct tcc::public_key<K, L, N, Q, W> & a = *this;
	memcpy(a.p, b.p, SEED_P_SIZE);
	a.t1 = b.t1;
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::public_key<K, L, N, Q, W> & tcc::public_key<K, L, N, Q, W>::operator=(const struct tcc::public_key<K, L, N, Q, W> & b) {
	struct tcc::public_key<K, L, N, Q, W> & a = *this;
	memcpy(a.p, b.p, SEED_P_SIZE);
	a.t1 = b.t1;
	return a;
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
tcc::secret_key<K, L, N, Q, W>::secret_key(const struct secret_key<K, L, N, Q, W> & b) {
	struct tcc::secret_key<K, L, N, Q, W> & a = *this;
	memcpy(a.p, b.p, SEED_P_SIZE);
	memcpy(a.k, b.k, SEED_K_SIZE);
	memcpy(a.tr, b.tr, SEED_TR_SIZE);
	a.s1 = b.s1;
	a.s2 = b.s2;
	a.t0 = b.t0;
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::secret_key<K, L, N, Q, W> & tcc::secret_key<K, L, N, Q, W>::operator=(const struct tcc::secret_key<K, L, N, Q, W> & b) {
	struct tcc::secret_key<K, L, N, Q, W> & a = *this;
	memcpy(a.p, b.p, SEED_P_SIZE);
	memcpy(a.k, b.k, SEED_K_SIZE);
	memcpy(a.tr, b.tr, SEED_TR_SIZE);
	a.s1 = b.s1;
	a.s2 = b.s2;
	a.t0 = b.t0;
	return a;
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
tcc::key_pair<K, L, N, Q, W>::key_pair(const struct tcc::key_pair<K, L, N, Q, W> & b) {
	struct tcc::key_pair<K, L, N, Q, W> & a = *this;
	a.public_key = b.public_key;
	a.secret_key = b.secret_key;
}

template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::key_pair<K, L, N, Q, W> & tcc::key_pair<K, L, N, Q, W>::operator=(const struct tcc::key_pair<K, L, N, Q, W> & b) {
	struct tcc::key_pair<K, L, N, Q, W> & a = *this;
	a.public_key = b.public_key;
	a.secret_key = b.secret_key;
	return a;
}