#include <dilithium_ii.h>
#include <stdlib.h>
#include <polynomial.h>
#include <ntt_polynomial.h>
#include <result.h>
#include <string.h>

const unsigned int M2 = 5;
const unsigned int N2 = 3;
const unsigned int Q2 = 10;
const unsigned int N_TESTS = 1000;

const unsigned int K = tcc::K;
const unsigned int L = tcc::L;
const unsigned int N = tcc::N;
const unsigned int Q = tcc::Q;
const unsigned int W = tcc::W;

template <unsigned int M, unsigned int N, unsigned int Q>
struct tcc::polynomial_vector<M, N, Q, 0> create_vector(int polynomials[M][N]) {
    struct tcc::polynomial_vector<M, N, Q, 0> v;
    for (int i = 0; i < M; i++) {
        struct tcc::polynomial<N, Q, 0> p(polynomials[i]);
        v[i] = p;
    }
    return v;
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct tcc::polynomial_matrix<P, M, N, Q, 0> create_matrix(int polynomials[P][M][N]) {
    struct tcc::polynomial_matrix<P, M, N, Q, 0> m;
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < M; j++) {
            struct tcc::polynomial<N, Q, 0> p(polynomials[i][j]);
            m[i][j] = p;
        }
    }
    return m;
}

// m1 = Mat([[Mod(8*x^0+ 9*x^1+ 7*x^2, x^3 + 1)], [Mod(2*x^0+ 0*x^1+ 4*x^2, x^3 + 1)], [Mod(7*x^0+ 3*x^1+ 2*x^2, x^3 + 1)], [Mod(4*x^0+ 3*x^1+ 4*x^2, x^3 + 1)], [Mod(4*x^0+ 2*x^1+ 2*x^2, x^3 + 1)]]~)
// v1 = Mat([[Mod(5*x^0+ 7*x^1+ 3*x^2, x^3 + 1)], [Mod(7*x^0+ 5*x^1+ 6*x^2, x^3 + 1)], [Mod(1*x^0+ 9*x^1+ 2*x^2, x^3 + 1)], [Mod(5*x^0+ 0*x^1+ 0*x^2, x^3 + 1)], [Mod(6*x^0+ 8*x^1+ 3*x^2, x^3 + 1)]]~)
// [Mod(10*x^2 + 16*x + 13, x^3 + 1)]
// [  Mod(10*x^2 + 5*x + 9, x^3 + 1)]
// [  Mod(4*x^2 + 12*x + 8, x^3 + 1)]
// [   Mod(4*x^2 + 3*x + 9, x^3 + 1)]
// [ Mod(5*x^2 + 10*x + 10, x^3 + 1)]
int test_matrix_sum_with_vector() {
    int a1[M2][1][N2] = {{{8, 9, 7}},
                         {{2, 0, 4}},
                         {{7, 3, 2}},
                         {{4, 3, 4}},
                         {{4, 2, 2}}};
    struct tcc::polynomial_matrix<M2, 1, N2, Q2, 0> b1 = create_matrix<M2, 1, N2, Q2>(a1);
    int a2[M2][N2] = {{5, 7, 3}, {7, 5, 6}, {1, 9, 2}, {5, 0, 0}, {6, 8, 3}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> b2 = create_vector<M2, N2, Q2>(a2);
    int a3[M2][1][N2] = {{{3, 6, 0}},
                         {{9, 5, 0}},
                         {{8, 2, 4}},
                         {{9, 3, 4}},
                         {{0, 0, 5}}};
    struct tcc::polynomial_matrix<M2, 1, N2, Q2, 0> b3 = b1 + b2;
    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < N2; j++) {
            if (b3[i][0][j] != a3[i][0][j]) {
                return -1;
            }
        }
    }
    return 0;
}

int test_matrix_sum_with_matrix() {
    int a1[M2][M2][N2] = {{{7, 9, 7}, {6, 9, 3}, {0, 3, 2}, {2, 6, 2}, {6, 2, 6}},
                          {{8, 0, 1}, {4, 6, 5}, {1, 8, 5}, {8, 4, 2}, {9, 3, 5}},
                          {{1, 1, 6}, {8, 7, 5}, {3, 9, 8}, {5, 1, 7}, {9, 7, 1}},
                          {{7, 5, 1}, {8, 9, 9}, {4, 3, 9}, {1, 1, 5}, {3, 2, 9}},
                          {{0, 5, 2}, {6, 3, 9}, {3, 9, 9}, {4, 4, 2}, {1, 4, 0}}};
    struct tcc::polynomial_matrix<M2, M2, N2, Q2, 0> b1 = create_matrix<M2, M2, N2, Q2>(a1);
    int a2[M2][M2][N2] = {{{5, 2, 1}, {4, 4, 6}, {2, 1, 2}, {7, 4, 2}, {0, 1, 8}},
                          {{4, 2, 5}, {3, 6, 6}, {2, 2, 9}, {9, 8, 0}, {7, 3, 6}},
                          {{6, 0, 0}, {9, 4, 6}, {5, 8, 8}, {9, 8, 4}, {3, 8, 7}},
                          {{2, 4, 2}, {9, 7, 8}, {6, 1, 0}, {7, 0, 0}, {9, 0, 5}},
                          {{5, 8, 6}, {7, 7, 0}, {4, 5, 1}, {4, 4, 9}, {8, 0, 9}}};
    struct tcc::polynomial_matrix<M2, M2, N2, Q2, 0> b2 = create_matrix<M2, M2, N2, Q2>(a2);
    int a3[M2][M2][N2] = {{{2, 1, 8}, {0, 3, 9}, {2, 4, 4}, {9, 0, 4}, {6, 3, 4}},
                          {{2, 2, 6}, {7, 2, 1}, {3, 0, 4}, {7, 2, 2}, {6, 6, 1}},
                          {{7, 1, 6}, {7, 1, 1}, {8, 7, 6}, {4, 9, 1}, {2, 5, 8}},
                          {{9, 9, 3}, {7, 6, 7}, {0, 4, 9}, {8, 1, 5}, {2, 2, 4}},
                          {{5, 3, 8}, {3, 0, 9}, {7, 4, 0}, {8, 8, 1}, {9, 4, 9}}};
    struct tcc::polynomial_matrix<M2, M2, N2, Q2, 0> b3 = b1 + b2;
    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < M2; j++) {
            for (int k = 0; k < N2; k++) {
                if (b3[i][j][k] != a3[i][j][k]) {
                    return -1;
                }
            }
        }
    }
    return 0;
}

// [ Mod(4*x^2 + 2*x + 3, x^3 + 1)]
// [Mod(-2*x^2 - 5*x - 5, x^3 + 1)]
// [        Mod(-6*x + 6, x^3 + 1)]
// [ Mod(4*x^2 + 3*x - 1, x^3 + 1)]
// [  Mod(-x^2 - 6*x - 2, x^3 + 1)]
int test_matrix_subtraction_with_vector() {
    int a1[M2][1][N2] = {{{8, 9, 7}},
                         {{2, 0, 4}},
                         {{7, 3, 2}},
                         {{4, 3, 4}},
                         {{4, 2, 2}}};
    struct tcc::polynomial_matrix<M2, 1, N2, Q2, 0> b1 = create_matrix<M2, 1, N2, Q2>(a1);
    int a2[M2][N2] = {{5, 7, 3}, {7, 5, 6}, {1, 9, 2}, {5, 0, 0}, {6, 8, 3}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> b2 = create_vector<M2, N2, Q2>(a2);
    int a3[M2][1][N2] = {{{3, 2, 4}},
                         {{5, 5, 8}},
                         {{6, 4, 0}},
                         {{9, 3, 4}},
                         {{8, 4, 9}}};
    struct tcc::polynomial_matrix<M2, 1, N2, Q2, 0> b3 = b1 - b2;
    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < N2; j++) {
            if (b3[i][0][j] != a3[i][0][j]) {
                return -1;
            }
        }
    }
    return 0;
}

int test_matrix_subtraction_with_matrix() {
    int a1[M2][M2][N2] = {{{7, 9, 7}, {6, 9, 3}, {0, 3, 2}, {2, 6, 2}, {6, 2, 6}},
                          {{8, 0, 1}, {4, 6, 5}, {1, 8, 5}, {8, 4, 2}, {9, 3, 5}},
                          {{1, 1, 6}, {8, 7, 5}, {3, 9, 8}, {5, 1, 7}, {9, 7, 1}},
                          {{7, 5, 1}, {8, 9, 9}, {4, 3, 9}, {1, 1, 5}, {3, 2, 9}},
                          {{0, 5, 2}, {6, 3, 9}, {3, 9, 9}, {4, 4, 2}, {1, 4, 0}}};
    struct tcc::polynomial_matrix<M2, M2, N2, Q2, 0> b1 = create_matrix<M2, M2, N2, Q2>(a1);
    int a2[M2][M2][N2] = {{{5, 2, 1}, {4, 4, 6}, {2, 1, 2}, {7, 4, 2}, {0, 1, 8}},
                          {{4, 2, 5}, {3, 6, 6}, {2, 2, 9}, {9, 8, 0}, {7, 3, 6}},
                          {{6, 0, 0}, {9, 4, 6}, {5, 8, 8}, {9, 8, 4}, {3, 8, 7}},
                          {{2, 4, 2}, {9, 7, 8}, {6, 1, 0}, {7, 0, 0}, {9, 0, 5}},
                          {{5, 8, 6}, {7, 7, 0}, {4, 5, 1}, {4, 4, 9}, {8, 0, 9}}};
    struct tcc::polynomial_matrix<M2, M2, N2, Q2, 0> b2 = create_matrix<M2, M2, N2, Q2>(a2);
    int a3[M2][M2][N2] = {{{2, 7, 6}, {2, 5, 7}, {8, 2, 0}, {5, 2, 0}, {6, 1, 8}},
                          {{4, 8, 6}, {1, 0, 9}, {9, 6, 6}, {9, 6, 2}, {2, 0, 9}},
                          {{5, 1, 6}, {9, 3, 9}, {8, 1, 0}, {6, 3, 3}, {6, 9, 4}},
                          {{5, 1, 9}, {9, 2, 1}, {8, 2, 9}, {4, 1, 5}, {4, 2, 4}},
                          {{5, 7, 6}, {9, 6, 9}, {9, 4, 8}, {0, 0, 3}, {3, 4, 1}}};
    struct tcc::polynomial_matrix<M2, M2, N2, Q2, 0> b3 = b1 - b2;
    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < M2; j++) {
            for (int k = 0; k < N2; k++) {
                if (b3[i][j][k] != a3[i][j][k]) {
                    return -1;
                }
            }
        }
    }
    return 0;
}


// [Mod(330*x^2 + 219*x - 100, x^3 + 1)]
// [ Mod(288*x^2 + 187*x - 15, x^3 + 1)]
// [ Mod(377*x^2 + 189*x - 88, x^3 + 1)]
// [Mod(365*x^2 + 155*x - 168, x^3 + 1)]
// [ Mod(300*x^2 + 86*x - 132, x^3 + 1)]
int test_matrix_multiplication_with_vector() {
    int a1[M2][M2][N2] = {{{7, 9, 7}, {6, 9, 3}, {0, 3, 2}, {2, 6, 2}, {6, 2, 6}},
                          {{8, 0, 1}, {4, 6, 5}, {1, 8, 5}, {8, 4, 2}, {9, 3, 5}},
                          {{1, 1, 6}, {8, 7, 5}, {3, 9, 8}, {5, 1, 7}, {9, 7, 1}},
                          {{7, 5, 1}, {8, 9, 9}, {4, 3, 9}, {1, 1, 5}, {3, 2, 9}},
                          {{0, 5, 2}, {6, 3, 9}, {3, 9, 9}, {4, 4, 2}, {1, 4, 0}}};
    struct tcc::polynomial_matrix<M2, M2, N2, Q2, 0> m1 = create_matrix<M2, M2, N2, Q2>(a1);
    int a2[M2][N2] = {{5, 7, 3}, {7, 5, 6}, {1, 9, 2}, {5, 0, 0}, {6, 8, 3}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> v1 = create_vector<M2, N2, Q2>(a2);
    int a3[M2][N2] = {{0, 9, 0},
                      {5, 7, 8},
                      {2, 9, 7},
                      {2, 5, 5},
                      {8, 6, 0}};
    struct tcc::polynomial_matrix<M2, 1, N2, Q2, 0> m2 = m1 * v1;
    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < N2; j++) {
            if (m2[i][0][j] != a3[i][j]) {
                return -1;
            }
        }
    }
    return 0;
}
// m2 = Mat([[Mod(7*x^0+ 9*x^1+ 7*x^2, x^3 + 1), Mod(6*x^0+ 9*x^1+ 3*x^2, x^3 + 1), Mod(0*x^0+ 3*x^1+ 2*x^2, x^3 + 1), Mod(2*x^0+ 6*x^1+ 2*x^2, x^3 + 1), Mod(6*x^0+ 2*x^1+ 6*x^2, x^3 + 1)], [Mod(8*x^0+ 0*x^1+ 1*x^2, x^3 + 1), Mod(4*x^0+ 6*x^1+ 5*x^2, x^3 + 1), Mod(1*x^0+ 8*x^1+ 5*x^2, x^3 + 1), Mod(8*x^0+ 4*x^1+ 2*x^2, x^3 + 1), Mod(9*x^0+ 3*x^1+ 5*x^2, x^3 + 1)], [Mod(1*x^0+ 1*x^1+ 6*x^2, x^3 + 1), Mod(8*x^0+ 7*x^1+ 5*x^2, x^3 + 1), Mod(3*x^0+ 9*x^1+ 8*x^2, x^3 + 1), Mod(5*x^0+ 1*x^1+ 7*x^2, x^3 + 1), Mod(9*x^0+ 7*x^1+ 1*x^2, x^3 + 1)], [Mod(7*x^0+ 5*x^1+ 1*x^2, x^3 + 1), Mod(8*x^0+ 9*x^1+ 9*x^2, x^3 + 1), Mod(4*x^0+ 3*x^1+ 9*x^2, x^3 + 1), Mod(1*x^0+ 1*x^1+ 5*x^2, x^3 + 1), Mod(3*x^0+ 2*x^1+ 9*x^2, x^3 + 1)], [Mod(0*x^0+ 5*x^1+ 2*x^2, x^3 + 1), Mod(6*x^0+ 3*x^1+ 9*x^2, x^3 + 1), Mod(3*x^0+ 9*x^1+ 9*x^2, x^3 + 1), Mod(4*x^0+ 4*x^1+ 2*x^2, x^3 + 1), Mod(1*x^0+ 4*x^1+ 0*x^2, x^3 + 1)]]~)
// m3 = Mat([[Mod(5*x^0+ 2*x^1+ 1*x^2, x^3 + 1), Mod(4*x^0+ 4*x^1+ 6*x^2, x^3 + 1), Mod(2*x^0+ 1*x^1+ 2*x^2, x^3 + 1), Mod(7*x^0+ 4*x^1+ 2*x^2, x^3 + 1), Mod(0*x^0+ 1*x^1+ 8*x^2, x^3 + 1)], [Mod(4*x^0+ 2*x^1+ 5*x^2, x^3 + 1), Mod(3*x^0+ 6*x^1+ 6*x^2, x^3 + 1), Mod(2*x^0+ 2*x^1+ 9*x^2, x^3 + 1), Mod(9*x^0+ 8*x^1+ 0*x^2, x^3 + 1), Mod(7*x^0+ 3*x^1+ 6*x^2, x^3 + 1)], [Mod(6*x^0+ 0*x^1+ 0*x^2, x^3 + 1), Mod(9*x^0+ 4*x^1+ 6*x^2, x^3 + 1), Mod(5*x^0+ 8*x^1+ 8*x^2, x^3 + 1), Mod(9*x^0+ 8*x^1+ 4*x^2, x^3 + 1), Mod(3*x^0+ 8*x^1+ 7*x^2, x^3 + 1)], [Mod(2*x^0+ 4*x^1+ 2*x^2, x^3 + 1), Mod(9*x^0+ 7*x^1+ 8*x^2, x^3 + 1), Mod(6*x^0+ 1*x^1+ 0*x^2, x^3 + 1), Mod(7*x^0+ 0*x^1+ 0*x^2, x^3 + 1), Mod(9*x^0+ 0*x^1+ 5*x^2, x^3 + 1)], [Mod(5*x^0+ 8*x^1+ 6*x^2, x^3 + 1), Mod(7*x^0+ 7*x^1+ 0*x^2, x^3 + 1), Mod(4*x^0+ 5*x^1+ 1*x^2, x^3 + 1), Mod(4*x^0+ 4*x^1+ 9*x^2, x^3 + 1), Mod(8*x^0+ 0*x^1+ 9*x^2, x^3 + 1)]]~)
// [Mod(246*x^2 + 141*x - 61, x^3 + 1) Mod(367*x^2 + 190*x - 178, x^3 + 1) Mod(207*x^2 + 83*x - 124, x^3 + 1) Mod(340*x^2 + 245*x + 1, x^3 + 1) Mod(309*x^2 + 15*x - 119, x^3 + 1)]
// [Mod(234*x^2 + 163*x + 7, x^3 + 1) Mod(376*x^2 + 244*x - 31, x^3 + 1) Mod(233*x^2 + 73*x - 86, x^3 + 1) Mod(356*x^2 + 207*x + 30, x^3 + 1) Mod(406*x^2 + 26*x - 20, x^3 + 1)]
// [Mod(298*x^2 + 183*x - 28, x^3 + 1) Mod(431*x^2 + 156*x - 89, x^3 + 1) Mod(338*x^2 + 64*x - 137, x^3 + 1) Mod(467*x^2 + 261*x - 56, x^3 + 1) Mod(407*x^2 + 12*x - 84, x^3 + 1)]
// [Mod(265*x^2 + 39*x - 68, x^3 + 1) Mod(449*x^2 + 63*x - 184, x^3 + 1) Mod(310*x^2 - 36*x - 190, x^3 + 1) Mod(421*x^2 + 175*x - 48, x^3 + 1) Mod(427*x^2 - 71*x - 137, x^3 + 1)]
// [Mod(212*x^2 + 104*x - 27, x^3 + 1) Mod(350*x^2 + 121*x - 158, x^3 + 1) Mod(265*x^2 - 11*x - 152, x^3 + 1) Mod(343*x^2 + 223*x - 121, x^3 + 1) Mod(280*x^2 + 15*x - 183, x^3 + 1)]
int test_matrix_multiplication_with_matrix() {
    int a1[M2][M2][N2] = {{{7, 9, 7}, {6, 9, 3}, {0, 3, 2}, {2, 6, 2}, {6, 2, 6}},
                          {{8, 0, 1}, {4, 6, 5}, {1, 8, 5}, {8, 4, 2}, {9, 3, 5}},
                          {{1, 1, 6}, {8, 7, 5}, {3, 9, 8}, {5, 1, 7}, {9, 7, 1}},
                          {{7, 5, 1}, {8, 9, 9}, {4, 3, 9}, {1, 1, 5}, {3, 2, 9}},
                          {{0, 5, 2}, {6, 3, 9}, {3, 9, 9}, {4, 4, 2}, {1, 4, 0}}};
    struct tcc::polynomial_matrix<M2, M2, N2, Q2, 0> b1 = create_matrix<M2, M2, N2, Q2>(a1);
    int a2[M2][M2][N2] = {{{5, 2, 1}, {4, 4, 6}, {2, 1, 2}, {7, 4, 2}, {0, 1, 8}},
                          {{4, 2, 5}, {3, 6, 6}, {2, 2, 9}, {9, 8, 0}, {7, 3, 6}},
                          {{6, 0, 0}, {9, 4, 6}, {5, 8, 8}, {9, 8, 4}, {3, 8, 7}},
                          {{2, 4, 2}, {9, 7, 8}, {6, 1, 0}, {7, 0, 0}, {9, 0, 5}},
                          {{5, 8, 6}, {7, 7, 0}, {4, 5, 1}, {4, 4, 9}, {8, 0, 9}}};
    struct tcc::polynomial_matrix<M2, M2, N2, Q2, 0> b2 = create_matrix<M2, M2, N2, Q2>(a2);
    int a3[M2][M2][N2] = {{{9, 1, 6}, {2, 0, 7}, {6, 3, 7}, {1, 5, 0}, {1, 5, 9}},
                          {{7, 3, 4}, {9, 4, 6}, {4, 3, 3}, {0, 7, 6}, {0, 6, 6}},
                          {{2, 3, 8}, {1, 6, 1}, {3, 4, 8}, {4, 1, 7}, {6, 2, 7}},
                          {{2, 9, 5}, {6, 3, 9}, {0, 4, 0}, {2, 5, 1}, {3, 9, 7}},
                          {{3, 4, 2}, {2, 1, 0}, {8, 9, 5}, {9, 3, 3}, {7, 5, 0}}};
    struct tcc::polynomial_matrix<M2, M2, N2, Q2, 0> b3 = b1 * b2;
    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < M2; j++) {
            for (int k = 0; k < N2; k++) {
                if (b3[i][j][k] != a3[i][j][k]) {
                    return -1;
                }
            }
        }
    }
    return 0;
}

int test_random_matrix_with_matrix_operations() {
    for (int i = 0; i < 1; i++) {
        struct tcc::polynomial_matrix<K, K, N, Q, W> m1;
        for (int j = 0; j < K; j++) {
            for (int k = 0; k < K; k++) {
                m1[j][k] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
            }
        }
        struct tcc::ntt_polynomial_matrix<K, K, N, Q, W> m1_hat = m1.foward_transform();

        struct tcc::polynomial_matrix<K, K, N, Q, W> m2;
        for (int j = 0; j < K; j++) {
            for (int k = 0; k < K; k++) {
                m2[j][k] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
            }
        }
        struct tcc::ntt_polynomial_matrix<K, K, N, Q, W> m2_hat = m2.foward_transform();

        struct tcc::polynomial_matrix<K, K, N, Q, W> m3m = m1 * m2;
        struct tcc::ntt_polynomial_matrix<K, K, N, Q, W> m3m_hat = m1_hat * m2_hat;
        struct tcc::polynomial_matrix<K, K, N, Q, W> m3m_ = m3m_hat.backward_transform();
        
        struct tcc::polynomial_matrix<K, K, N, Q, W> m3a = m1 + m2;
        struct tcc::ntt_polynomial_matrix<K, K, N, Q, W> m3a_hat = m1_hat + m2_hat;
        struct tcc::polynomial_matrix<K, K, N, Q, W> m3a_ = m3a_hat.backward_transform();
        
        struct tcc::polynomial_matrix<K, K, N, Q, W> m3s = m1 - m2;
        struct tcc::ntt_polynomial_matrix<K, K, N, Q, W> m3s_hat = m1_hat - m2_hat;
        struct tcc::polynomial_matrix<K, K, N, Q, W> m3s_ = m3s_hat.backward_transform();

        for (int k = 0; k < K; k++) {
            for (int l = 0; l < K; l++) {
                for (int n = 0; n < N; n++) {
                    if (m3m[k][l][n] != m3m_[k][l][n] ||
                        m3a[k][l][n] != m3a_[k][l][n] ||
                        m3s[k][l][n] != m3s_[k][l][n]) {
                        return -1;
                    }
                }
            }
        }
    }
    return 0;
}

int test_random_matrix_with_vector_operations() {
    for (int i = 0; i < 1; i++) {
        struct tcc::polynomial_matrix<K, 1, N, Q, W> m1;
        for (int j = 0; j < K; j++) {
                m1[i][0] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        }
        struct tcc::ntt_polynomial_matrix<K, 1, N, Q, W> m1_hat = m1.foward_transform();

        struct tcc::polynomial_matrix<K, K, N, Q, W> m2;
        for (int j = 0; j < K; j++) {
            for (int k = 0; k < K; k++) {
                m2[j][k] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
            }
        }
        struct tcc::ntt_polynomial_matrix<K, K, N, Q, W> m2_hat = m2.foward_transform();

        struct tcc::polynomial_vector<K, N, Q, W> m3;
        for (int j = 0; j < K; j++) {
            m3[j] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        }
        struct tcc::ntt_polynomial_vector<K, N, Q, W> m3_hat = m3.foward_transform();

        // Multiply
        struct tcc::polynomial_matrix<K, 1, N, Q, W> m4m = m2 * m3;
        struct tcc::ntt_polynomial_matrix<K, 1, N, Q, W> m4m_hat = m2_hat * m3_hat;
        struct tcc::polynomial_matrix<K, 1, N, Q, W> m4m_ = m4m_hat.backward_transform();

        // add
        struct tcc::polynomial_matrix<K, 1, N, Q, W> m4a = m1 + m3;
        struct tcc::ntt_polynomial_matrix<K, 1, N, Q, W> m4a_hat = m1_hat + m3_hat;
        struct tcc::polynomial_matrix<K, 1, N, Q, W> m4a_ = m4a_hat.backward_transform();

        // subtract
        struct tcc::polynomial_matrix<K, 1, N, Q, W> m4s = m1 + m3;
        struct tcc::ntt_polynomial_matrix<K, 1, N, Q, W> m4s_hat = m1_hat + m3_hat;
        struct tcc::polynomial_matrix<K, 1, N, Q, W> m4s_ = m4s_hat.backward_transform();

        for (int j = 0; j < K; j++) {
            for (int k = 0; k < N; k++) {       
                if (m4m[j][0][k] != m4m_[j][0][k] ||
                    m4a[j][0][k] != m4a_[j][0][k] ||
                    m4s[j][0][k] != m4s_[j][0][k]) {
                    return -1;
                }
            }
        }
    }
    return 0;
}

int test_matrix_conversion_to_vector() {
    struct tcc::polynomial_matrix<K, 1, N, Q, W> m;
    for (int i = 0; i < K; i++) {
        m[i][0] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
    }
    struct tcc::polynomial_vector<K, N, Q, W> v = *m;
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            if (v[i][j] != m[i][0][j]) {
                return -1;
            }
        }
    }
    return 0;
}

int main() {
    // display_box("Testing polynomial matrix operations");
    // assert_value("Test matrix sum with vector", test_matrix_sum_with_vector());
    // assert_value("Test matrix sum with matrix", test_matrix_sum_with_matrix());
    // assert_value("Test matrix subtraction with vector", test_matrix_subtraction_with_vector());
    // assert_value("Test matrix subtraction with matrix", test_matrix_subtraction_with_matrix());
    // assert_value("Test matrix multiplication with vector", test_matrix_multiplication_with_vector());
    // assert_value("Test matrix multiplication with matrix", test_matrix_multiplication_with_matrix());
    // assert_value("Test random matrix with matrix operations", test_random_matrix_with_matrix_operations());
    // assert_value("Test random matrix with vector operations", test_random_matrix_with_vector_operations());
    // assert_value("Test matrix conversion to vector", test_matrix_conversion_to_vector());
    return 0;
}
