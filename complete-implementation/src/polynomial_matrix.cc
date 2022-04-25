/* Included on header file */

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
tcc::polynomial_matrix<P, M, N, Q, W>::polynomial_matrix(const struct tcc::polynomial_vector<P, N, Q, W> & b) {
	if (M == 1) {
		*this = &b;
	} else {
		throw std::domain_error("Vector to Matrix conversion requirer column matrix (constructor)");
	}
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
tcc::polynomial_matrix<P, M, N, Q, W>::polynomial_matrix(const struct tcc::polynomial_matrix<P, M, N, Q, W> & b) {
	struct polynomial_matrix<P, M, N, Q, W> & a = *this;
	for (int i = 0; i < P; i++) {
		a[i] = b[i];
	}
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<M, N, Q, W> & tcc::polynomial_matrix<P, M, N, Q, W>::operator[](int index) {
    return vectors[index];
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<M, N, Q, W> tcc::polynomial_matrix<P, M, N, Q, W>::operator[](int index) const {
	return vectors[index];
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_matrix<P, M, N, Q, W> tcc::polynomial_matrix<P, M, N, Q, W>::operator+(const tcc::polynomial_matrix<P, M, N, Q, W> & b) const {
	struct tcc::polynomial_matrix<P, M, N, Q, W> c;
    const struct tcc::polynomial_matrix<P, M, N, Q, W> & a = *this;
    for (int i = 0; i < P; i++) {
        c[i] = a[i] + b[i];
    }
    return c;
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_matrix<P, M, N, Q, W> tcc::polynomial_matrix<P, M, N, Q, W>::operator+(const struct tcc::polynomial_vector<P, N, Q, W>& b_) const {
	if (M == 1) {
		const struct tcc::polynomial_matrix<P, 1, N, Q, W> b = &b_;
		const struct tcc::polynomial_matrix<P, M, N, Q, W> & a = *this;
		return a + b;
	} else {
		throw std::domain_error("matrix-vector addition requirer matrix to be a column matrix (polynomial_matrix<P, M, N, Q, W>::operator+)");
	}
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_matrix<P, M, N, Q, W> tcc::polynomial_matrix<P, M, N, Q, W>::operator-(const tcc::polynomial_matrix<P, M, N, Q, W>& b) const {
    const struct tcc::polynomial_matrix<P, M, N, Q, W> & a = *this;
    struct tcc::polynomial_matrix<P, M, N, Q, W> c;
    for (int i = 0; i < P; i++) {
        c[i] = a[i] - b[i];
    }
    return c;
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_matrix<P, M, N, Q, W> tcc::polynomial_matrix<P, M, N, Q, W>::operator-(const struct tcc::polynomial_vector<P, N, Q, W>& b_) const {
	if (M == 1) {
		const struct tcc::polynomial_matrix<P, 1, N, Q, W> b = &b_;
		const struct tcc::polynomial_matrix<P, M, N, Q, W> & a = *this;
		return a - b;
	} else {
		throw std::domain_error("Vector to Matrix conversion results in a column matrix (polynomial_matrix<P, M, N, Q, W>::operator-)");
	}
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
template <unsigned int M2>
struct tcc::polynomial_matrix<P, M2, N, Q, W> tcc::polynomial_matrix<P, M, N, Q, W>::operator*(const tcc::polynomial_matrix<M, M2, N, Q, W> & b) const {
	const struct tcc::polynomial_matrix<P, M, N, Q, W> & a = *this;
	struct tcc::polynomial_matrix<P, M2, N, Q, W> c;
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < M2; j++) {
			c[i][j] = 0;
			for (int k = 0; k < M; k++) {
				c[i][j] = c[i][j] + (a[i][k] * b[k][j]);
			}
		}
	}
	return c;
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_matrix<P, 1, N, Q, W> tcc::polynomial_matrix<P, M, N, Q, W>::operator*(const tcc::polynomial_vector<M, N, Q, W> & b_) const {
	struct tcc::polynomial_matrix<M, 1, N, Q, W> b = &b_;
	const struct tcc::polynomial_matrix<P, M, N, Q, W> & a = *this;
	return a * b;
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<P, N, Q, W> tcc::polynomial_matrix<P, M, N, Q, W>::operator*() {
	if (M == 1) {
		struct tcc::polynomial_vector<P, N, Q, W> b;
		const tcc::polynomial_matrix<P, M, N, Q, W> & a = *this;
		for (int i = 0; i < P; i++) {
			b[i] = a[i][0];
		}
		return b;
	} else {
		throw std::domain_error("Matrix to Vector conversion requirer column matrix (polynomial_matrix<P, M, N, Q, W>::operator*)");
	}
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial_matrix<P, M, N, Q, W> tcc::polynomial_matrix<P, M, N, Q, W>::foward_transform() const {
	const struct tcc::polynomial_matrix<P, M, N, Q, W> & a = *this;
	struct tcc::ntt_polynomial_matrix<P, M, N, Q, W> b;
	for (int i = 0; i < P; i++) {
		b[i] = a[i].foward_transform();
	}
	return b;
}
