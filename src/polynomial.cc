#include <polynomial.h>

template <unsigned int N, unsigned int Q>
struct polynomial<N, Q> & polynomial<N, Q>::operator=(const struct polynomial<N, Q> & a) {
    // TODO
}

template <unsigned int N, unsigned int Q>
struct polynomial<N, Q> & polynomial<N, Q>::operator+(const struct polynomial<N, Q> & b) {
    struct polynomial<N, Q> & a = *this;
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
struct polynomial<N, Q> generate_random_polynomial() {
    struct polynomial<N, Q> p;
    for (int i = 0; i < N; i++) {
        p[i] = polynomial<N, Q>::uniform_distribution.gen();
    }
    return p;
}