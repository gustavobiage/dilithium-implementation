#include <polynomial.h>

template <unsigned int N, unsigned int Q>
template <unsigned int Q2>
polynomial<N, Q>::polynomial(const struct polynomial<Q2, N> & b) {
    polynomial<N, Q> & a = *this;
    for (int i = 0; i < N; i++) {
        a[i] = b[i] % Q;
    }
}

template <unsigned int N, unsigned int Q>
struct polynomial<N, Q> & polynomial<N, Q>::operator+(const struct polynomial<N, Q> & b) {
    const struct polynomial<N, Q> & a = *this;
    struct polynomial<N, Q> c;
    for (int i = 0; i < N; i++) {
        c[i] = (a[i] + b[i]) % Q;
    }
    return c;
}

template <unsigned int N, unsigned int Q>
struct polynomial<N, Q> & polynomial<N, Q>::operator*(const struct polynomial<N, Q> & b) {
    struct polynomial<N, Q> & a = *this;
    struct polynomial<N, Q> c;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            c[(i+j) % N] = (a[j] * b[(i - j + N) % N]) % Q;
        }
    }
    return c;
}

template <unsigned int N, unsigned int Q>
int32_t & polynomial<N, Q>::operator[](int index) {
    return coefficients[index];
}

template <unsigned int N, unsigned int Q>
struct polynomial<N, Q> polynomial<N, Q>::generate_random_polynomial() {
    struct polynomial<N, Q> p;
    for (int i = 0; i < N; i++) {
        p[i] = polynomial<N, Q>::uniform_distribution.generate();
    }
    return p;
}