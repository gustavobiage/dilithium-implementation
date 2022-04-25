/* Included on header file */

template <unsigned int N, unsigned int Q, unsigned int W>
tcc::polynomial<N, Q, W>::polynomial(const int b[N]) {
    struct tcc::polynomial<N, Q, W> & a = *this;
    for (int i = 0; i < N; i++) {
        a[i] = b[i];
    }
}

template <unsigned int N, unsigned int Q, unsigned int W>
template <unsigned int Q2>
tcc::polynomial<N, Q, W>::polynomial(const struct tcc::polynomial<N, Q2, W> & b) {
    struct tcc::polynomial<N, Q, W> & a = *this;
    for (int i = 0; i < N; i++) {
        a[i] = b[i] % Q;
    }
}

template <unsigned int N, unsigned int Q, unsigned int W>
tcc::polynomial<N, Q, W>::polynomial(const struct tcc::ntt_polynomial<N, Q, W> & b) {
    *this = b.backward_transform();
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial<N, Q, W> & tcc::polynomial<N, Q, W>::operator=(const struct tcc::polynomial<N, Q, W> & b) {
    struct tcc::polynomial<N, Q, W> & a = *this;
    for (int i = 0; i < N; i++) {
        a[i] = b[i];
    }
    return a;
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial<N, Q, W> tcc::polynomial<N, Q, W>::operator=(int32_t c) {
    struct tcc::polynomial<N, Q, W> & p = *this;
    for (int i = 0; i < N; i++) {
        p[i] = add(c, 0, Q);
    }
    return p;
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial<N, Q, W> tcc::polynomial<N, Q, W>::operator+(const struct tcc::polynomial<N, Q, W> & b) {
    const struct tcc::polynomial<N, Q, W> & a = *this;
    struct tcc::polynomial<N, Q, W> c;
    for (int i = 0; i < N; i++) {
        c[i] = add(a[i], b[i], Q);
    }
    return c;
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial<N, Q, W> tcc::polynomial<N, Q, W>::operator-(const struct tcc::polynomial<N, Q, W> & b) {
    const struct tcc::polynomial<N, Q, W> & a = *this;
    struct tcc::polynomial<N, Q, W> c;
    for (int i = 0; i < N; i++) {
        c[i] = subtract(a[i], b[i], Q);
    }
    return c;
}

template <unsigned int N, unsigned int Q, unsigned W>
struct tcc::polynomial<N, Q, W> tcc::polynomial<N, Q, W>::operator*(const struct tcc::polynomial<N, Q, W> & b) {
    struct tcc::polynomial<N, Q, W> & a = *this;
    struct tcc::polynomial<N, Q, W> c;
    int32_t r[2*N] = {0};
    // multiply polynomials
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            r[i+j] = add(r[i+j], multiply(a[i], b[j], Q), Q);
        }
    }
    // remainder of division by (x^n + 1)
    for (int i = N; i < 2*N; i++) {
        r[i-N] = subtract(r[i-N], r[i], Q);
    }
    // copy result to polynomial c (c = a * b)
    for(int i = 0; i < N; i++) {
        c[i] = r[i];
    }
    return c;
}

template <unsigned int N, unsigned int Q, unsigned int W>
template <unsigned int M>
struct tcc::polynomial_vector<M, N, Q, W> tcc::polynomial<N, Q, W>::operator*(const struct tcc::polynomial_vector<M, N, Q, W> & b) {
    struct tcc::polynomial<N, Q, W> a = *this;
    struct tcc::polynomial_vector<M, N, Q, W> c;
    for (int i = 0; i < M; i++) {
        c[i] = a * b[i];
    }
    return c;
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial<N, Q, W> tcc::polynomial<N, Q, W>::operator+=(const struct tcc::polynomial<N, Q, W> & b) {
    struct tcc::polynomial<N, Q, W> & a = *this;
    return a + b;
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<1, N, Q, W> tcc::polynomial<N, Q, W>::operator&() {
    struct tcc::polynomial_vector<1, N, Q, W> b;
    b[0] = *this;
    return b;
}

template <unsigned int N, unsigned int Q, unsigned int W>
int32_t & tcc::polynomial<N, Q, W>::operator[](int index) {
    return coefficients[index];
}

template <unsigned int N, unsigned int Q, unsigned int W>
int32_t tcc::polynomial<N, Q, W>::operator[](int index) const {
    return coefficients[index];
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::uniform_distribution<Q> tcc::polynomial<N, Q, W>::uniform_distribution;

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial<N, Q, W> tcc::polynomial<N, Q, W>::generate_random_polynomial() {
    struct tcc::polynomial<N, Q, W> p;
    for (int i = 0; i < N; i++) {
        p[i] = tcc::polynomial<N, Q, W>::uniform_distribution.generate();
    }
    return p;
}

template <unsigned int N, unsigned int Q, unsigned int W>
tcc::zeta_array<N, Q, W> tcc::polynomial<N, Q, W>::zetas;

template <unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial<N, Q, W> tcc::polynomial<N, Q, W>::foward_transform() const {
    int32_t new_degree, n_polynomials;
    struct tcc::ntt_polynomial<N, Q, W> b;
    const struct tcc::polynomial<N, Q, W> & a = *this;
    // Copy coefficients of a to b
    for (int i = 0; i < N; i++) {
        b[i] = a[i];
    }
    // 1. Recursively split the polynomials
    // 2. Every time we split all polynomials, we double the polynomial count
    // 3. When a polynomial splits, we generate two new polynomials with half the degree of the old one
    n_polynomials = 1;
    int k = 0;
    for (int32_t degree = N; degree > 1; degree = new_degree) {
        new_degree = degree / 2;
        for (int32_t poly = 0; poly < n_polynomials; poly++) {
            int32_t zeta = polynomial<N, Q, W>::zetas[++k];
            for (int32_t i = 0; i < new_degree; i++) {
                int32_t j = (poly*degree) + i;
                int32_t t = multiply(b[j + new_degree], zeta, Q);
                b[j + new_degree] = subtract(b[j], t, Q);
                b[j] = add(b[j], t, Q);
            }
        }
        n_polynomials *= 2;
    }
    return b;
}
