/* Included on header file polynomial.h */

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
tcc::ntt_polynomial_vector<M, N, Q, W>::ntt_polynomial_vector(const struct tcc::ntt_polynomial_vector<M, N, Q, W> & b) {
    struct ntt_polynomial_vector<M, N, Q, W> & a = *this;
    for (int i = 0; i < M; i++) {
        a[i] = b[i];
    }
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
tcc::ntt_polynomial_vector<M, N, Q, W>::ntt_polynomial_vector(const struct tcc::ntt_polynomial_matrix<M, 1, N, Q, W> & b) {
	*this = *b;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial<N, Q, W> & tcc::ntt_polynomial_vector<M, N, Q, W>::operator[](int index) {
	return polynomials[index];
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial<N, Q, W> tcc::ntt_polynomial_vector<M, N, Q, W>::operator[](int index) const {
	return polynomials[index];
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial_vector<M, N, Q, W> tcc::ntt_polynomial_vector<M, N, Q, W>::operator+(const struct tcc::ntt_polynomial_vector<M, N, Q, W>& b) const {
	const struct tcc::ntt_polynomial_vector<M, N, Q, W> & a = *this;
	struct tcc::ntt_polynomial_vector<M, N, Q, W> c;
	for (int i = 0; i < M; i++) {
		c[i] = a[i] + b[i];
	}
	return c;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial_matrix<M, 1, N, Q, W> tcc::ntt_polynomial_vector<M, N, Q, W>::operator+(const struct tcc::ntt_polynomial_matrix<M, 1, N, Q, W>& b) const {
	const struct tcc::ntt_polynomial_matrix<M, 1, N, Q, W> a = &(*this);
	struct tcc::ntt_polynomial_matrix<M, 1, N, Q, W> c;
	for (int i = 0; i < M; i++) {
		c[i] = a[i] + b[i];
	}
	return c;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial_vector<M, N, Q, W> tcc::ntt_polynomial_vector<M, N, Q, W>::operator-(const struct tcc::ntt_polynomial_vector<M, N, Q, W>& b) const {
	const struct tcc::ntt_polynomial_vector<M, N, Q, W> & a = *this;
	struct tcc::ntt_polynomial_vector<M, N, Q, W> c;
	for (int i = 0; i < M; i++) {
		c[i] = a[i] - b[i];
	}
	return c;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial_matrix<M, 1, N, Q, W> tcc::ntt_polynomial_vector<M, N, Q, W>::operator-(const struct tcc::ntt_polynomial_matrix<M, 1, N, Q, W>& b) const {
	const struct tcc::ntt_polynomial_matrix<M, 1, N, Q, W> a = &(*this);
	struct tcc::ntt_polynomial_matrix<M, 1, N, Q, W> c;
	for (int i = 0; i < M; i++) {
		c[i] = a[i] - b[i];
	}
	return c;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
template <unsigned int M2>
struct tcc::ntt_polynomial_matrix<M, M2, N, Q, W> tcc::ntt_polynomial_vector<M, N, Q, W>::operator*(const struct tcc::ntt_polynomial_matrix<1, M2, N, Q, W>& b) const {
	const struct tcc::ntt_polynomial_matrix<M, 1, N, Q, W> a = &(*this);
	struct tcc::ntt_polynomial_matrix<M, M2, N, Q, W> c = a * b;
	return c;
}

// Referencing returns matrix
template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial_matrix<M, 1, N, Q, W> tcc::ntt_polynomial_vector<M, N, Q, W>::operator&() const {
    struct tcc::ntt_polynomial_matrix<M, 1, N, Q, W> b;
    const struct tcc::ntt_polynomial_vector<M, N, Q, W> & a = *this;
    for (int i = 0; i < M; i++) {
        b[i][0] = a[i];
    }
    return b;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<M, N, Q, W> tcc::ntt_polynomial_vector<M, N, Q, W>::backward_transform() const {
	const struct tcc::ntt_polynomial_vector<M, N, Q, W> & a = *this;
	struct tcc::polynomial_vector<M, N, Q, W> b;
	for (int i = 0; i < M; i++) {
		b[i] = a[i].backward_transform();
	}
	return b;
}