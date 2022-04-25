/* Included on header file */

template <unsigned int N, unsigned int Q, unsigned int W>
tcc::ntt_polynomial<N, Q, W>::ntt_polynomial(const int b[N]) {
    *this = b;
}

template <unsigned int N, unsigned int Q, unsigned int W>
tcc::ntt_polynomial<N, Q, W>::ntt_polynomial(const struct tcc::polynomial<N, Q, W> & b) {
    *this = b.foward_transform();
}

template <unsigned int N, unsigned int Q, unsigned int W>
template <unsigned int Q2>
tcc::ntt_polynomial<N, Q, W>::ntt_polynomial(const struct tcc::ntt_polynomial<N, Q2, W> & b) {
    struct tcc::ntt_polynomial<N, Q, W> & a = *this;
    for (int i = 0; i < N; i++) {
        a[i] = b[i];
    }
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial<N, Q, W> tcc::ntt_polynomial<N, Q, W>::operator=(const int32_t c) {
    struct tcc::ntt_polynomial<N, Q, W> & a = *this;
    for (int i = 0; i < N; i++) {
        a[i] = 0;
    }
    return a;
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial<N, Q, W> tcc::ntt_polynomial<N, Q, W>::operator+(const struct tcc::ntt_polynomial<N, Q, W> & b) const {
    struct tcc::ntt_polynomial<N, Q, W> c;
    const struct tcc::ntt_polynomial<N, Q, W> & a = *this;
    for (int i = 0; i < N; i++) {
        c[i] = add(a[i], b[i], Q);
    }
    return c;
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial<N, Q, W> tcc::ntt_polynomial<N, Q, W>::operator-(const struct tcc::ntt_polynomial<N, Q, W> & b) const {
    struct tcc::ntt_polynomial<N, Q, W> c;
    const struct tcc::ntt_polynomial<N, Q, W> & a = *this;
    for (int i = 0; i < N; i++) {
        c[i] = subtract(a[i], b[i], Q);
    }
    return c;
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial<N, Q, W> tcc::ntt_polynomial<N, Q, W>::operator*(const struct tcc::ntt_polynomial<N, Q, W> & b) {
    struct tcc::ntt_polynomial<N, Q, W> c;
    const struct tcc::ntt_polynomial<N, Q, W> & a = *this;
    for (int i = 0; i < N; i++) {
        c[i] = multiply(a[i], b[i], Q);
    }
    return c;
}

template <unsigned int N, unsigned int Q, unsigned int W>
template <unsigned int M>
struct tcc::ntt_polynomial_vector<M, N, Q, W> tcc::ntt_polynomial<N, Q, W>::operator*(const struct tcc::ntt_polynomial_vector<M, N, Q, W> & b) {
    struct tcc::ntt_polynomial<N, Q, W> a = *this;
    struct tcc::ntt_polynomial_vector<M, N, Q, W> c;
    for (int i = 0; i < M; i++) {
        c[i] = a * b[i];
    }
    return c;
}

template <unsigned int N, unsigned int Q, unsigned int W>
int32_t & tcc::ntt_polynomial<N, Q, W>::operator[](int index) {
    return coefficients[index];
}

template <unsigned int N, unsigned int Q, unsigned int W>
int32_t tcc::ntt_polynomial<N, Q, W>::operator[](int index) const {
    return coefficients[index];
}

template <unsigned int N, unsigned int Q, unsigned int W>
tcc::zeta_array<N, Q, W> tcc::ntt_polynomial<N, Q, W>::zetas;

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial<N, Q, W> tcc::ntt_polynomial<N, Q, W>::backward_transform() const {
    int32_t new_degree, n_polynomials;
    struct tcc::polynomial<N, Q, W> b;
    const struct ntt_polynomial<N, Q, W> & a = *this;
    // Copy coefficients of a to b
    for (int i = 0; i < N; i++) {
        b[i] = a[i];
    }
    int _2 = find_modular_inverse(2, Q);
    // 1. Recursively join the polynomials
    // 2. Every time we join all polynomials, we cut in half the polynomial count
    // 3. When a polynomial joins, it become 1 polynomial with double the degree
    n_polynomials = N;
    int k = N;
    for (int32_t degree = 1; degree < N; degree = new_degree) {
        new_degree = degree * 2;
        for (int32_t poly = n_polynomials-2; poly >= 0; poly -= 2) {
            int32_t zeta = ntt_polynomial<N, Q, W>::zetas[--k];
            zeta = find_modular_inverse(zeta, Q);
            for (int32_t i = degree-1; i >= 0; i--) {
                int32_t j = (poly*degree) + i;
                int32_t t = b[j];
                b[j] = add(t, b[j+degree], Q);
                b[j] = multiply(_2, b[j], Q);
                b[j + degree] = subtract(t, b[j+degree], Q);
                b[j + degree] = multiply(zeta, b[j + degree], Q);
                b[j + degree] = multiply(_2, b[j + degree], Q);
            }
        }
        n_polynomials /= 2;
    }
    return b;
}