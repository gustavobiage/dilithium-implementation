/* Included on header file */

template <unsigned int N, unsigned int Q>
template <unsigned int Q2>
polynomial<N, Q>::polynomial(const struct polynomial<N, Q2> & b) {
    polynomial<N, Q> & a = *this;
    for (int i = 0; i < N; i++) {
        a[i] = b[i] % Q;
    }
}

template <unsigned int N, unsigned int Q>
struct polynomial<N, Q> polynomial<N, Q>::operator=(int32_t c) {
    struct polynomial<N, Q> p;
    for (int i = 0; i < N; i++) {
        p[i] = (c + Q) % Q;
    }
    return p;
}

template <unsigned int N, unsigned int Q>
struct polynomial<N, Q> polynomial<N, Q>::operator+(const struct polynomial<N, Q> & b) {
    const struct polynomial<N, Q> & a = *this;
    struct polynomial<N, Q> c;
    for (int i = 0; i < N; i++) {
        c[i] = (a[i] + b[i]) % Q;
    }
    return c;
}

template <unsigned int N, unsigned int Q>
struct polynomial<N, Q> polynomial<N, Q>::operator-(const struct polynomial<N, Q> & b) {
    const struct polynomial<N, Q> & a = *this;
    struct polynomial<N, Q> c;
    for (int i = 0; i < N; i++) {
        c[i] = (a[i] - b[i] + Q) % Q;
    }
    return c;
}

template <unsigned int N, unsigned int Q>
struct polynomial<N, Q> polynomial<N, Q>::operator*(const struct polynomial<N, Q> & b) {
    struct polynomial<N, Q> & a = *this;
    struct polynomial<N, Q> c;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            c[i] = (a[j] * b[(i - j + N) % N]) % Q;
        }
    }
    return c;
}

template <unsigned int N, unsigned int Q>
template <unsigned int M>
struct polynomial_vector<M, N, Q> polynomial<N, Q>::operator*(const struct polynomial_vector<M, N, Q> & b) {
    struct polynomial<N, Q> a = *this;
    struct polynomial_vector<M, N, Q> c;
    for (int i = 0; i < M; i++) {
        c[i] = b[i] * a;
    }
    return c;
}

template <unsigned int N, unsigned int Q>
struct polynomial<N, Q> polynomial<N, Q>::operator+=(const struct polynomial<N, Q> & b) {
    struct polynomial<N, Q> & a = *this;
    return a + b;
}

template <unsigned int N, unsigned int Q>
struct polynomial_vector<1, N, Q> polynomial<N, Q>::operator&() {
    struct polynomial_vector<1, N, Q> b;
    b[0] = *this;
    return b;
}

template <unsigned int N, unsigned int Q>
int32_t & polynomial<N, Q>::operator[](int index) {
    return coefficients[index];
}

template <unsigned int N, unsigned int Q>
int32_t polynomial<N, Q>::operator[](int index) const {
    return coefficients[index];
}

template <unsigned int N, unsigned int Q>
struct uniform_distribution<Q> polynomial<N, Q>::uniform_distribution;

template <unsigned int N, unsigned int Q>
struct polynomial<N, Q> polynomial<N, Q>::generate_random_polynomial() {
    struct polynomial<N, Q> p;
    for (int i = 0; i < N; i++) {
        p[i] = polynomial<N, Q>::uniform_distribution.generate();
    }
    return p;
}