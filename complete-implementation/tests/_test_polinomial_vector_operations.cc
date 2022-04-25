#include <stdlib.h>
#include <dilithium_ii.h>
#include <polynomial.h>
#include <ntt_polynomial.h>
#include <result.h>

const unsigned int Q2 = 10;
const unsigned int N2 = 3;
const unsigned int M2 = 5;
const unsigned int N_TESTS = 1000;

const unsigned int K = tcc::K;
const unsigned int L = tcc::L;
const unsigned int N = tcc::N;
const unsigned int Q = tcc::Q;
const unsigned int W = tcc::W;

template <unsigned int M, unsigned int N, unsigned int Q>
struct tcc::polynomial_vector<M, N, Q, 0> create_vector(int polynomes[M][N]) {
    struct tcc::polynomial_vector<M, N, Q, 0> v;
    for (int i = 0; i < M; i++) {
        struct tcc::polynomial<N, Q, 0> p(polynomes[i]);
        v[i] = p;
    }
    return v;
}

template <unsigned int M, unsigned int N, unsigned int Q>
struct tcc::polynomial_matrix<M, 1, N, Q, 0> create_column_matrix(int polynomes[M][N]) {
    struct tcc::polynomial_matrix<M, 1, N, Q, 0> m;
    for (int i = 0; i < M; i++) {
        struct tcc::polynomial<N, Q, 0> p(polynomes[i]);
        m[i][0] = p;
    }
    return m;
}

template <unsigned int M, unsigned int N, unsigned int Q>
struct tcc::polynomial_matrix<1, M, N, Q, 0> create_line_matrix(int polynomes[M][N]) {
    struct tcc::polynomial_matrix<1, M, N, Q, 0> m;
    for (int i = 0; i < M; i++) {
        struct tcc::polynomial<N, Q, 0> p(polynomes[i]);
        m[0][i] = p;
    }
    return m;
}

int test_vector_sum_with_vector() {
    int a1[M2][N2] = {{1, 3, 6}, {6, 9, 6}, {4, 7, 1}, {4, 5, 0}, {9, 2, 3}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> v1 = create_vector<M2, N2, Q2>(a1);

    int a2[M2][N2] = {{5, 3, 8}, {2, 8, 4}, {3, 6, 4}, {7, 4, 1}, {8, 9, 2}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> v2 = create_vector<M2, N2, Q2>(a2);

    int a3[M2][N2] = {{6, 6, 4}, {8, 7, 0}, {7, 3, 5}, {1, 9, 1}, {7, 1, 5}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> v3 = v1 + v2;

    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < N2; j++) {
            if (v3[i][j] != a3[i][j]) {
                return -1;
            }
        }
    }

    for (int i = 0; i < N_TESTS; i++) {
        struct tcc::polynomial_vector<K, N, Q, W> k1;
        for (int j = 0; j < K; j++) {
            k1[j] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        }
        struct tcc::ntt_polynomial_vector<K, N, Q, W> k1_hat = k1.foward_transform();

        struct tcc::polynomial_vector<K, N, Q, W> k2;
        for (int j = 0; j < K; j++) {
            k2[j] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        }
        struct tcc::ntt_polynomial_vector<K, N, Q, W> k2_hat = k2.foward_transform();
        
        struct tcc::polynomial_vector<K, N, Q, W> k3 = k1 + k2;
        struct tcc::ntt_polynomial_vector<K, N, Q, W> k3_hat = k1_hat + k2_hat;
        struct tcc::polynomial_vector<K, N, Q, W> k3_ = k3_hat.backward_transform();

        for (int j = 0; j < K; j++) {
            for (int k = 0; k < N; k++) {
                if (k3[j][k] != k3_[j][k]) {
                    return -1;
                }
            }
        }
    }
    return 0;
}

int test_vector_sum_with_matrix() {
    int a1[M2][N2] = {{1, 3, 6}, {6, 9, 6}, {4, 7, 1}, {4, 5, 0}, {9, 2, 3}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> v1 = create_vector<M2, N2, Q2>(a1);

    int a2[M2][N2] = {{5, 3, 8}, {2, 8, 4}, {3, 6, 4}, {7, 4, 1}, {8, 9, 2}};
    struct tcc::polynomial_matrix<M2, 1, N2, Q2, 0> v2 = create_column_matrix<M2, N2, Q2>(a2);

    int a3[M2][N2] = {{6, 6, 4}, {8, 7, 0}, {7, 3, 5}, {1, 9, 1}, {7, 1, 5}};
    struct tcc::polynomial_matrix<M2, 1, N2, Q2, 0> v3 = v1 + v2;

    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < N2; j++) {
            if (v3[i][0][j] != a3[i][j]) {
                return -1;
            }
        }
    }

    for (int i = 0; i < N_TESTS; i++) {
        struct tcc::polynomial_vector<K, N, Q, W> k1;
        for (int j = 0; j < K; j++) {
            k1[j] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        }
        struct tcc::ntt_polynomial_vector<K, N, Q, W> k1_hat = k1.foward_transform();

        struct tcc::polynomial_matrix<K, 1, N, Q, W> k2;
        for (int j = 0; j < K; j++) {
            k2[j][0] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        }
        struct tcc::ntt_polynomial_matrix<K, 1, N, Q, W> k2_hat = k2.foward_transform();
        
        struct tcc::polynomial_matrix<K, 1, N, Q, W> k3 = k1 + k2;
        struct tcc::ntt_polynomial_matrix<K, 1, N, Q, W> k3_hat = k1_hat + k2_hat;
        struct tcc::polynomial_matrix<K, 1, N, Q, W> k3_ = k3_hat.backward_transform();

        for (int j = 0; j < K; j++) {
            for (int k = 0; k < N; k++) {
                if (k3[j][0][k] != k3_[j][0][k]) {
                    return -1;
                }
            }
        }
    }
    return 0;
}

int test_vector_subtraction_with_vector() {
    int a1[M2][N2] = {{1, 3, 6}, {6, 9, 6}, {4, 7, 1}, {4, 5, 0}, {9, 2, 3}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> v1 = create_vector<M2, N2, Q2>(a1);

    int a2[M2][N2] = {{5, 3, 8}, {2, 8, 4}, {3, 6, 4}, {7, 4, 1}, {8, 9, 2}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> v2 = create_vector<M2, N2, Q2>(a2);

    int a3[M2][N2] = {{6, 0, 8}, {4, 1, 2}, {1, 1, 7}, {7, 1, 9}, {1, 3, 1}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> v3 = v1 - v2;

    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < N2; j++) {
            if (v3[i][j] != a3[i][j]) {
                return -1;
            }
        }
    }

    for (int i = 0; i < N_TESTS; i++) {
        struct tcc::polynomial_vector<K, N, Q, W> k1;
        for (int j = 0; j < K; j++) {
            k1[j] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        }
        struct tcc::ntt_polynomial_vector<K, N, Q, W> k1_hat = k1.foward_transform();

        struct tcc::polynomial_vector<K, N, Q, W> k2;
        for (int j = 0; j < K; j++) {
            k2[j] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        }
        struct tcc::ntt_polynomial_vector<K, N, Q, W> k2_hat = k2.foward_transform();
        
        struct tcc::polynomial_vector<K, N, Q, W> k3 = k1 - k2;
        struct tcc::ntt_polynomial_vector<K, N, Q, W> k3_hat = k1_hat - k2_hat;
        struct tcc::polynomial_vector<K, N, Q, W> k3_ = k3_hat.backward_transform();

        for (int j = 0; j < K; j++) {
            for (int k = 0; k < N; k++) {
                if (k3[j][k] != k3_[j][k]) {
                    return -1;
                }
            }
        }
    }
    return 0;
}

int test_vector_subtraction_with_matrix() {
    int a1[M2][N2] = {{1, 3, 6}, {6, 9, 6}, {4, 7, 1}, {4, 5, 0}, {9, 2, 3}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> v1 = create_vector<M2, N2, Q2>(a1);

    int a2[M2][N2] = {{5, 3, 8}, {2, 8, 4}, {3, 6, 4}, {7, 4, 1}, {8, 9, 2}};
    struct tcc::polynomial_matrix<M2, 1, N2, Q2, 0> v2 = create_column_matrix<M2, N2, Q2>(a2);

    int a3[M2][N2] = {{6, 0, 8}, {4, 1, 2}, {1, 1, 7}, {7, 1, 9}, {1, 3, 1}};
    struct tcc::polynomial_matrix<M2, 1, N2, Q2, 0> v3 = v1 - v2;

    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < N2; j++) {
            if (v3[i][0][j] != a3[i][j]) {
                return -1;
            }
        }
    }

    for (int i = 0; i < N_TESTS; i++) {
        struct tcc::polynomial_vector<K, N, Q, W> k1;
        for (int j = 0; j < K; j++) {
            k1[j] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        }
        struct tcc::ntt_polynomial_vector<K, N, Q, W> k1_hat = k1.foward_transform();

        struct tcc::polynomial_matrix<K, 1, N, Q, W> k2;
        for (int j = 0; j < K; j++) {
            k2[j][0] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        }
        struct tcc::ntt_polynomial_matrix<K, 1, N, Q, W> k2_hat = k2.foward_transform();
        
        struct tcc::polynomial_matrix<K, 1, N, Q, W> k3 = k1 - k2;
        struct tcc::ntt_polynomial_matrix<K, 1, N, Q, W> k3_hat = k1_hat - k2_hat;
        struct tcc::polynomial_matrix<K, 1, N, Q, W> k3_ = k3_hat.backward_transform();

        for (int j = 0; j < K; j++) {
            for (int k = 0; k < N; k++) {
                if (k3[j][0][k] != k3_[j][0][k]) {
                    return -1;
                }
            }
        }
    }
    return 0;
}

int test_vector_conversion_to_matrix() {
    int a1[M2][N2] = {{1, 3, 6}, {6, 9, 6}, {4, 7, 1}, {4, 5, 0}, {9, 2, 3}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> v1 = create_vector<M2, N2, Q2>(a1);
    struct tcc::polynomial_matrix<M2, 1, N2, Q2, 0> m1 = &v1;
    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < N2; j++) {
            if (m1[i][0][j] != a1[i][j]) {
                return -1;
            }
        }
    }
    return 0;
}

// [47*x^2 - 30*x - 37; 40*x^2 - 10*x - 58; 40*x^2 - 9*x - 45; 55*x^2 + 19*x - 20; 77*x^2 + 21*x - 52]
// [105*x^2 + 15*x - 60; 108*x^2 + 42*x - 72; 96*x^2 + 39*x - 54; 84*x^2 + 81*x + 9; 141*x^2 + 114*x - 24]
// [58*x^2 + 39*x - 39; 74*x^2 + 42*x - 28; 61*x^2 + 41*x - 22; 39*x^2 + 64*x + 17; 79*x^2 + 90*x + 9]
// [47*x^2 + 37*x - 20; 56*x^2 + 42*x - 12; 46*x^2 + 39*x - 8; 24*x^2 + 51*x + 23; 53*x^2 + 76*x + 22]
// [93*x^2 + 13*x + 20; 58*x^2 + 64*x - 14; 57*x^2 + 48*x + 1; 38*x^2 + 47*x + 49; 60*x^2 + 91*x + 41]

int test_vector_multiplication_with_matrix() {
    int a1[M2][N2] = {{1, 3, 6}, {6, 9, 6}, {4, 7, 1}, {4, 5, 0}, {9, 2, 3}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> v1 = create_vector<M2, N2, Q2>(a1);

    int a2[M2][N2] = {{5, 3, 8}, {2, 8, 4}, {3, 6, 4}, {7, 4, 1}, {8, 9, 2}};
    struct tcc::polynomial_matrix<1, M2, N2, Q2, 0> v2 = create_line_matrix<M2, N2, Q2>(a2);

    int a3[M2][M2][N2] = {
                        {{3, 0, 7}, {2, 0, 0}, {5, 1, 0}, {0, 9, 5}, {8, 1, 7}},
                        {{0, 5, 5}, {8, 2, 8}, {6, 9, 6}, {9, 1, 4}, {6, 4, 1}},
                        {{1, 9, 8}, {2, 2, 4}, {8, 1, 1}, {7, 4, 9}, {9, 0, 9}},
                        {{0, 7, 7}, {8, 2, 6}, {2, 9, 6}, {3, 1, 4}, {2, 6, 3}},
                        {{0, 3, 3}, {6, 4, 8}, {1, 8, 7}, {9, 7, 8}, {1, 1, 0}}};
    struct tcc::polynomial_matrix<M2, M2, N2, Q2, 0> v3 = v1 * v2;

    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < M2; j++) {
            for (int k = 0; k < N2; k++) {
                if (v3[i][j][k] != a3[i][j][k]) {
                    return -1;
                }
            }
        }
    }
    return 0;
}

int main() {
    // display_box("Testing polynomial vector operations");
    // assert_value("Test vector sum with vector", test_vector_sum_with_vector());
    // assert_value("Test vector sum with matrix", test_vector_sum_with_matrix());
    // assert_value("Test vector subtraction with vector", test_vector_subtraction_with_vector());
    // assert_value("Test vector subtraction with matrix", test_vector_subtraction_with_matrix());
    // assert_value("Test vector conversion to matrix", test_vector_conversion_to_matrix());
    // assert_value("Test vector multiplication with matrix", test_vector_multiplication_with_matrix());
    return 0;
}
