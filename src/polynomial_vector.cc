/* Included on header file */

template <unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_vector<M, N, Q> polynomial_vector<M, N, Q>::operator=(const struct polynomial_vector<M, N, Q> & b) {
    struct polynomial_vector<M, N, Q> a;
    for (int i = 0; i < M; i++) {
        a[i] = b[i];
    }
    return a;
}

template <unsigned int M, unsigned int N, unsigned int Q>
polynomial_vector<M, N, Q>::polynomial_vector(const struct polynomial_matrix<M, 1, N, Q> &b) {
    struct polynomial_vector<M, N, Q> & a = *this;
    for (int i = 0; i < M; i++) {
        a[i] = b[i][1];
    }
}

template<unsigned int M, unsigned int N, unsigned int Q>
struct polynomial<N, Q> & polynomial_vector<M, N, Q>::operator[](int index) {
    return vector[index];
}

template<unsigned int M, unsigned int N, unsigned int Q>
struct polynomial<N, Q> polynomial_vector<M, N, Q>::operator[](int index) const {
    return vector[index];
}

template <unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<M, 1, N, Q> polynomial_vector<M, N, Q>::operator+(const struct polynomial_matrix<M, 1, N, Q>& b) {
    struct polynomial_matrix<M, 1, N, Q> a = &(*this);
    struct polynomial_matrix<M, 1, N, Q> c = a + b;
    return c;
}

template <unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<M, 1, N, Q> polynomial_vector<M, N, Q>::operator-(const struct polynomial_matrix<M, 1, N, Q>& b) {
    struct polynomial_matrix<M, 1, N, Q> a = &(*this);
    struct polynomial_matrix<M, 1, N, Q> c = a - b;
    return c;
}

template <unsigned int M, unsigned int N, unsigned int Q>
template <unsigned int M2>
struct polynomial_matrix<M, M2, N, Q> polynomial_vector<M, N, Q>::operator*(const struct polynomial_matrix<1, M2, N, Q>& b) {
    struct polynomial_matrix<M, 1, N, Q> a = &(*this);
    struct polynomial_matrix<M, M2, N, Q> c = a * b;
    return c;
}


// Referencing returns matrix
template<unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<M, 1, N, Q> polynomial_vector<M, N, Q>::operator&() {
    struct polynomial_matrix<M, 1, N, Q> b;
    const struct polynomial_vector<M, N, Q> & a = *this;
    for (int i = 0; i < M; i++) {
        b[i][0] = a[i];
    }
    return b;
}

// Referencing returns matrix
template<unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<M, 1, N, Q> polynomial_vector<M, N, Q>::operator&() const {
    struct polynomial_matrix<M, 1, N, Q> b;
    const struct polynomial_vector<M, N, Q> & a = *this;
    for (int i = 0; i < M; i++) {
        b[i][0] = a[i];
    }
    return b;
}