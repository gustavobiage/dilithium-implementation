#include <polynomial.h>

template <unsigned int Q>
struct polynomial<Q> & polynomial<Q>::operator=(const struct polynomial & a) {
    // TODO
}

template <unsigned int Q>
struct polynomial<Q> & polynomial<Q>::operator+(const struct polynomial& b) {
    struct polynomial<Q> & a = *this;
    struct polynomial<Q> c;
    for (int i = 0; i < Q; i++) {
        c[i] = a[i] + b[i];
    }
    return c;
}

template <unsigned int Q>
struct polynomial<Q> & polynomial<Q>::operator*(const struct polynomial<Q> & b) {
    struct polynomial<Q> & a = *this;
    struct polynomial<Q> c;
    for (int i = 0; i < Q; i++) {
        for (int j = 0; j < Q; j++) {
            c[(i+j) % Q] = a[j] * b[(i - j + Q) % Q];
        }
    }
    return c;
}

template <unsigned int Q>
struct polynomial<Q> & polynomial<Q>::operator/(const struct polynomial& a) {
    // TODO
}

template <unsigned int Q>
struct polynomial<Q> generate_random_polynomial() {
    // TODO
}