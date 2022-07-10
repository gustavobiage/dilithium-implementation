/* Included on header file */

template <unsigned int N, unsigned int Q, unsigned int W>
polynomial<N, Q, W>::polynomial(const int b[N]) {
    polynomial<N, Q, W> & a = *this;
    for (int i = 0; i < N; i++) {
        a[i] = b[i];
    }
}

template <unsigned int N, unsigned int Q, unsigned int W>
template <unsigned int Q2>
polynomial<N, Q, W>::polynomial(const struct polynomial<N, Q2, W> & b) {
    polynomial<N, Q, W> & a = *this;
    for (int i = 0; i < N; i++) {
        a[i] = b[i] % Q;
    }
}

template <unsigned int N, unsigned int Q, unsigned int W>
polynomial<N, Q, W>::polynomial(const struct ntt_polynomial<N, Q, W> & b) {
    *this = b.backward_transform();
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct polynomial<N, Q, W> polynomial<N, Q, W>::operator=(int32_t c) {
    struct polynomial<N, Q, W> & p = *this;
    for (int i = 0; i < N; i++) {
        p[i] = (c + Q) % Q;
    }
    return p;
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct polynomial<N, Q, W> polynomial<N, Q, W>::operator+(const struct polynomial<N, Q, W> & b) {
    const struct polynomial<N, Q, W> & a = *this;
    struct polynomial<N, Q, W> c;
    for (int i = 0; i < N; i++) {
        c[i] = (a[i] + b[i] + Q) % Q;
    }
    return c;
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct polynomial<N, Q, W> polynomial<N, Q, W>::operator-(const struct polynomial<N, Q, W> & b) {
    const struct polynomial<N, Q, W> & a = *this;
    struct polynomial<N, Q, W> c;
    for (int i = 0; i < N; i++) {
        c[i] = (a[i] - b[i] + Q) % Q;
    }
    return c;
}

template <unsigned int N, unsigned int Q, unsigned W>
struct polynomial<N, Q, W> polynomial<N, Q, W>::operator*(const struct polynomial<N, Q, W> & b) {
    struct polynomial<N, Q, W> & a = *this;
    struct polynomial<N, Q, W> c;
    int32_t r[2*N] = {0};
    // multiply polynomials
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            r[i+j] = (r[i+j] + (int64_t) a[i] * b[j]) % Q;
        }
    }
    // remainder of division by (x^n + 1)
    for (int i = N; i < 2*N; i++) {
        r[i-N] = ((int64_t) r[i-N] - r[i] + Q) % Q;
    }
    // copy result to polynomial c (c = a * b)
    for(int i = 0; i < N; i++) {
        c[i] = r[i];
    }
    return c;
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct polynomial<N, Q, W> polynomial<N, Q, W>::operator+=(const struct polynomial<N, Q, W> & b) {
    struct polynomial<N, Q, W> & a = *this;
    return a + b;
}

template <unsigned int N, unsigned int Q, unsigned int W>
int32_t & polynomial<N, Q, W>::operator[](int index) {
    return coefficients[index];
}

template <unsigned int N, unsigned int Q, unsigned int W>
int32_t polynomial<N, Q, W>::operator[](int index) const {
    return coefficients[index];
}

template <unsigned int N, unsigned int Q, unsigned int W>
struct uniform_distribution<Q> polynomial<N, Q, W>::uniform_distribution;

template <unsigned int N, unsigned int Q, unsigned int W>
struct polynomial<N, Q, W> polynomial<N, Q, W>::generate_random_polynomial() {
    struct polynomial<N, Q, W> p;
    for (int i = 0; i < N; i++) {
        p[i] = polynomial<N, Q, W>::uniform_distribution.generate();
    }
    return p;
}

//  For non discrete FTT you use complex numbers as W
//      long double real = cos(-2 * M_PI * j * (long double) k/N);
//      long double img = sin(-2 * M_PI * j * (long double) k/N);

// template <unsigned int N, unsigned int Q, unsigned int W>
// struct ntt_polynomial<N, Q, W> polynomial<N, Q, W>::__foward_transform() const {
//     struct ntt_polynomial<N, Q, W> b;
//     const struct polynomial<N, Q, W> & a = *this;
//     for (int k = 0; k < N; k++) {
//         int32_t sum = 0;
//         for (int j = 0; j < N; j++) {
//             int64_t aux = a[j];
//             aux = aux * fast_power(W, k*j, Q);
//             aux = aux % Q;
//             sum = (sum + aux) % Q;
//         }
//         b[k] = sum;
//     }
//     return b;
// }

template <unsigned int N, unsigned int Q, unsigned int W>
zeta_array<N, Q, W> polynomial<N, Q, W>::zetas;

//////////////////////////////////////////////////////////////////////////
// Montgomery reduction
//////////////////////////////////////////////////////////////////////////
// template <unsigned int N, unsigned int Q, unsigned int W>
// struct ntt_polynomial<N, Q, W> polynomial<N, Q, W>::foward_transform() const {
//     int32_t new_degree, n_polynomials;
//     struct ntt_polynomial<N, Q, W> b;
//     const struct polynomial<N, Q, W> & a = *this;
//     // Copy coefficients of a to b
//     for (int i = 0; i < N; i++) {
//         b[i] = a[i];
//     }
//     // 1. Recursively split the polynomials
//     // 2. Every time we split all polynomials, we double the polynomial count
//     // 3. When a polynomial splits, we generate two new polynomials with half the degree of the old one
//     n_polynomials = 1;
//     int k = 0;
//     for (int32_t degree = N; degree > 1; degree = new_degree) {
//         new_degree = degree / 2;
//         for (int32_t poly = 0; poly < n_polynomials; poly++) {
//             int32_t zeta = polynomial<N, Q, W>::zetas[++k];
//             zeta = montgomery_reduction<Q>(zeta);
//             for (int32_t i = 0; i < new_degree; i++) {
//                 int32_t j = (poly*degree) + i;
//                 int32_t t = multiply_montgomery<Q>(b[j + new_degree], zeta, Q);
//                 b[j + new_degree] = subtract(b[j], t, Q);
//                 b[j] = add(b[j], t, Q);
//             }
//         }
//         n_polynomials *= 2;
//     }
//     return b;
// }
//////////////////////////////////////////////////////////////////////////


template <unsigned int N, unsigned int Q, unsigned int W>
struct ntt_polynomial<N, Q, W> polynomial<N, Q, W>::foward_transform() const {
    int32_t new_degree, n_polynomials;
    struct ntt_polynomial<N, Q, W> b;
    const struct polynomial<N, Q, W> & a = *this;
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
            zeta = montgomery_reduction<Q>(zeta);
            for (int32_t i = 0; i < new_degree; i++) {
                int32_t j = (poly*degree) + i;
                int32_t t = multiply_montgomery<Q>(b[j + new_degree], zeta, Q);
                b[j + new_degree] = subtract(b[j], t, Q);
                b[j] = add(b[j], t, Q);
            }
        }
        n_polynomials *= 2;
    }
    return b;
}