/* Included on header file */

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
tcc::polynomial_vector<M, N, Q, W>::polynomial_vector(const struct tcc::polynomial_vector<M, N, Q, W> & b) {
    struct tcc::polynomial_vector<M, N, Q, W> & a = *this;
    for (int i = 0; i < M; i++) {
        a[i] = b[i];
    }
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
tcc::polynomial_vector<M, N, Q, W>::polynomial_vector(const struct tcc::polynomial_matrix<M, 1, N, Q, W> &b) {
    *this = *b;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial<N, Q, W> & tcc::polynomial_vector<M, N, Q, W>::operator[](int index) {
    return vector[index];
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial<N, Q, W> tcc::polynomial_vector<M, N, Q, W>::operator[](int index) const {
    return vector[index];
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<M, N, Q, W> & tcc::polynomial_vector<M, N, Q, W>::operator=(const int32_t c) {
    struct tcc::polynomial_vector<M, N, Q, W> & a = *this;
    for (int i = 0; i < M; i++) {
        a[i] = c;
    }
    return a;
}


template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<M, N, Q, W> & tcc::polynomial_vector<M, N, Q, W>::operator=(const struct tcc::polynomial_vector<M, N, Q, W> & b) {
    struct tcc::polynomial_vector<M, N, Q, W> & a = *this;
    for (int i = 0; i < M; i++) {
        a[i] = b[i];
    }
    return a;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_matrix<M, 1, N, Q, W> tcc::polynomial_vector<M, N, Q, W>::operator+(const struct tcc::polynomial_matrix<M, 1, N, Q, W>& b) {
    struct tcc::polynomial_matrix<M, 1, N, Q, W> a = &(*this);
    struct tcc::polynomial_matrix<M, 1, N, Q, W> c = a + b;
    return c;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<M, N, Q, W> tcc::polynomial_vector<M, N, Q, W>::operator+(const struct tcc::polynomial_vector<M, N, Q, W>& b) {
    const struct tcc::polynomial_vector<M, N, Q, W> & a = *this;
    struct tcc::polynomial_vector<M, N, Q, W> c;
    for (int i = 0; i < M; i++) {
        c[i] = a[i] + b[i];
    }
    return c;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_matrix<M, 1, N, Q, W> tcc::polynomial_vector<M, N, Q, W>::operator-(const struct tcc::polynomial_matrix<M, 1, N, Q, W>& b) const {
    const struct tcc::polynomial_matrix<M, 1, N, Q, W> a = &(*this);
    struct tcc::polynomial_matrix<M, 1, N, Q, W> c = a - b;
    return c;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<M, N, Q, W> tcc::polynomial_vector<M, N, Q, W>::operator-(const struct tcc::polynomial_vector<M, N, Q, W>& b) const {
    const struct tcc::polynomial_vector<M, N, Q, W> & a = *this;
    struct tcc::polynomial_vector<M, N, Q, W> c;
    for (int i = 0; i < M; i++) {
        c[i] = a[i] - b[i];
    }
    return c;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
template <unsigned int M2>
struct tcc::polynomial_matrix<M, M2, N, Q, W> tcc::polynomial_vector<M, N, Q, W>::operator*(const struct tcc::polynomial_matrix<1, M2, N, Q, W>& b) {
    const struct tcc::polynomial_matrix<M, 1, N, Q, W> a = &(*this);
    struct tcc::polynomial_matrix<M, M2, N, Q, W> c = a * b;
    return c;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<M, N, Q, W> tcc::polynomial_vector<M, N, Q, W>::operator*(int32_t c) const {
    const struct tcc::polynomial_vector<M, N, Q, W> & a = *this;
    struct tcc::polynomial_vector<M, N, Q, W> b;
    for (int i = 0; i < M; i++) {
        b[i] = a[i] * c;
    }
    return b;
}

// Referencing returns matrix
template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_matrix<M, 1, N, Q, W> tcc::polynomial_vector<M, N, Q, W>::operator&() {
    struct tcc::polynomial_matrix<M, 1, N, Q, W> b;
    const struct tcc::polynomial_vector<M, N, Q, W> & a = *this;
    for (int i = 0; i < M; i++) {
        b[i][0] = a[i];
    }
    return b;
}

// Referencing returns matrix
template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_matrix<M, 1, N, Q, W> tcc::polynomial_vector<M, N, Q, W>::operator&() const {
    struct tcc::polynomial_matrix<M, 1, N, Q, W> b;
    const struct tcc::polynomial_vector<M, N, Q, W> & a = *this;
    for (int i = 0; i < M; i++) {
        b[i][0] = a[i];
    }
    return b;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::ntt_polynomial_vector<M, N, Q, W> tcc::polynomial_vector<M, N, Q, W>::foward_transform() const {
    const struct tcc::polynomial_vector<M, N, Q, W> & a = *this;
    struct tcc::ntt_polynomial_vector<M, N, Q, W> b;
    for (int i = 0; i < M; i++) {
        b[i] = a[i].foward_transform();
    }
    return b;
}

template <unsigned int M, unsigned int N, unsigned int Q, unsigned int W>
int32_t tcc::polynomial_vector<M, N, Q, W>::norm_power_2() {
    const struct tcc::polynomial_vector<M, N, Q, W> & a = *this;
    int32_t max = -Q, aux;
    for (int i = 0; i < M; i++) {
        aux = a[i].norm_power_2();
        if (aux > max) {
            max = aux;
        }
    }
    return max;
}