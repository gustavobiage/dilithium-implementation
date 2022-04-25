#include <stdlib.h>
#include <dilithium_ii.h>
#include <polynomial.h>
#include <ntt_polynomial.h>
#include <result.h>

const unsigned int Q2 = 10;
const unsigned int N2 = 5;
const unsigned int M2 = 3;
const unsigned int N_TESTS = 1000;

const unsigned int K = tcc::K;
const unsigned int L = tcc::L;
const unsigned int N = tcc::N;
const unsigned int Q = tcc::Q;
const unsigned int W = tcc::W;
const unsigned int GAMMA1 = tcc::GAMMA1;

int test_random_polynome_generation() {
    struct tcc::polynomial<N, Q, W> p = tcc::polynomial<N, Q, W>::generate_random_polynomial();
    for (int i = 0; i < N; i++) {
        if (p[i] < 0 || p[i] >= Q) {
            return -1;
        }
    }

    struct tcc::polynomial_vector<K, N, GAMMA1, W> p2;
    for (int i = 0; i < K; i++) {
        p2[i] = tcc::polynomial<N, GAMMA1, W>::generate_random_polynomial();
        for (int j = 0; j < N; j++) {
            if (p2[i][j] < 0 || p2[i][j] >= GAMMA1) {
                return -1;
            }
        }
    }

    return 0;
}

int test_polynome_asignment() {
    struct tcc::polynomial<N, Q, W> p1 = tcc::polynomial<N, Q, W>::generate_random_polynomial();
    struct tcc::polynomial<N, Q, W> p2 = p1;

    for (int i = 0; i < N; i++) {
        if (p1[i] != p2[i]) {
            return -1;
        }
    }

    return 0;
}

int test_polynome_sum() {
    const int c1[N2] = {3, 9, 5, 1, 3};
    const int c2[N2] = {6, 9, 2, 3, 8};
    const int c3[N2] = {9, 8, 7, 4, 1};
    struct tcc::polynomial<N2, Q2, 0> a1(c1);
    struct tcc::polynomial<N2, Q2, 0> a2(c2);
    struct tcc::polynomial<N2, Q2, 0> a3 = a1 + a2;
    for (int i = 0; i < N2; i++) {
        if (a3[i] != c3[i]) {
            return -1;
        }
    }

    for (int i = 0; i < N_TESTS; i++) {
        struct tcc::polynomial<N, Q, W> p1 = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        struct tcc::ntt_polynomial<N, Q, W> p1_hat = p1.foward_transform();

        struct tcc::polynomial<N, Q, W> p2 = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        struct tcc::ntt_polynomial<N, Q, W> p2_hat = p2.foward_transform();

        struct tcc::polynomial<N, Q, W> p3 = p1 + p2;
        struct tcc::ntt_polynomial<N, Q, W> p3_hat = p1_hat + p2_hat;
        struct tcc::polynomial<N, Q, W> p3_ = p3_hat.backward_transform();

        for (int i = 0; i < N; i++) {
            if (p3[i] != p3_[i]) {
                return -1;
            }
        }
    }
    return 0;
}

int test_polynome_subtraction() {
    const int c1[N2] = {3, 9, 5, 1, 3};
    const int c2[N2] = {6, 9, 2, 3, 8};
    const int c3[N2] = {7, 0, 3, 8, 5};
    struct tcc::polynomial<N2, Q2, 0> p1(c1);
    struct tcc::polynomial<N2, Q2, 0> p2(c2);
    struct tcc::polynomial<N2, Q2, 0> p3 = p1 - p2;
    for (int i = 0; i < N2; i++) {
        if (p3[i] != c3[i]) {
            return -1;
        }
    }

    for (int i = 0; i < N_TESTS; i++) {
        struct tcc::polynomial<N, Q, W> p1 = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        struct tcc::ntt_polynomial<N, Q, W> p1_hat = p1.foward_transform();

        struct tcc::polynomial<N, Q, W> p2 = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        struct tcc::ntt_polynomial<N, Q, W> p2_hat = p2.foward_transform();

        struct tcc::polynomial<N, Q, W> p3 = p1 - p2;
        struct tcc::ntt_polynomial<N, Q, W> p3_hat = p1_hat - p2_hat;
        struct tcc::polynomial<N, Q, W> p3_ = p3_hat.backward_transform();

        for (int i = 0; i < N; i++) {
            if (p3[i] != p3_[i]) {
                return -1;
            }
        }
    }
    return 0;
}

int test_polynome_multiplication() {
    const int c1[N2] = {2, 7, 4, 9, 0};
    const int c2[N2] = {6, 6, 2, 8, 5};
    const int c3[N2] = {7, 2, 5, 8, 8};
    struct tcc::polynomial<N2, Q2, 0> p1(c1);
    struct tcc::polynomial<N2, Q2, 0> p2(c2);
    struct tcc::polynomial<N2, Q2, 0> p3 = p1 * p2;
    for (int i = 0; i < N2; i++) {
        if (p3[i] != c3[i]) {
            return -1;
        }
    }

    for (int i = 0; i < N_TESTS; i++) {
        struct tcc::polynomial<N, Q, W> p1 = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        struct tcc::ntt_polynomial<N, Q, W> p1_hat = p1.foward_transform();

        struct tcc::polynomial<N, Q, W> p2 = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        struct tcc::ntt_polynomial<N, Q, W> p2_hat = p2.foward_transform();

        struct tcc::polynomial<N, Q, W> p3 = p1 * p2;
        struct tcc::ntt_polynomial<N, Q, W> p3_hat = p1_hat * p2_hat;
        struct tcc::polynomial<N, Q, W> p3_ = p3_hat.backward_transform();

        for (int i = 0; i < N; i++) {
            if (p3[i] != p3_[i]) {
                return -1;
            }
        }
    }
    return 0;
}

int test_polynome_multiplication_with_vector() {
    const int c1[N2] = {4, 0, 1, 0, 4};
    struct tcc::polynomial<N2, Q2, 0> a1(c1);

    const int c2[M2][N2] = {{4, 1, 6, 4, 3},
                            {5, 5, 8, 4, 2},
                            {1, 4, 3, 7, 8}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> a2;
    for (int i = 0; i < M2; i++) {
        struct tcc::polynomial<N2, Q2, 0> p(c2[i]);
        a2[i] = p;
    }

    /* 
     * 34*x^4 + 5*x^3 + 12*x^2 - 23*x + 8
     * 36*x^4 + 13*x^3 + 21*x^2 - 14*x - 4
     * 39*x^4 - 15*x^2 - 4*x - 19
     */
    const int c3[M2][N2] = {{8, 7, 2, 5, 4},
                            {6, 6, 1, 3, 6},
                            {1, 6, 5, 0, 9}};
    struct tcc::polynomial_vector<M2, N2, Q2, 0> a3 = a1 * a2;
    for (int i = 0; i < M2; i++) {
        for (int j = 0; j < N2; j++) {
            if (a3[i][j] != c3[i][j]) {
                return -1;
            }
        }
    }

    for (int i = 0; i < N_TESTS; i++) {
        struct tcc::polynomial<N, Q, W> p1 = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        struct tcc::ntt_polynomial<N, Q, W> p1_hat = p1.foward_transform();

        struct tcc::polynomial_vector<K, N, Q, W> p2;
        for (int i = 0; i < K; i++) {
            p2[i] = tcc::polynomial<N, Q, W>::generate_random_polynomial();
        }
        struct tcc::ntt_polynomial_vector<K, N, Q, W> p2_hat = p2.foward_transform();

        struct tcc::polynomial_vector<K, N, Q, W> p3 = p1 * p2;
        struct tcc::ntt_polynomial_vector<K, N, Q, W> p3_hat = p1_hat * p2_hat;
        struct tcc::polynomial_vector<K, N, Q, W> p3_ = p3_hat.backward_transform();

        for (int i = 0; i < K; i++) {
            for (int j = 0; j < N; j++) {
                if (p3[i][j] != p3_[i][j]) {
                    return -1;
                }
            }
        }
    }
    return 0;
}

int main() {
    // display_box("Testing polynomial operations");
    // assert_value("Test random polynome generation", test_random_polynome_generation());
    // assert_value("Test polynome asignment", test_polynome_asignment());
    // assert_value("Test polynome sum", test_polynome_sum());
    // assert_value("Test polynome subtraction", test_polynome_subtraction());
    // assert_value("Test polynome multiplication", test_polynome_multiplication());
    // assert_value("Test polynome multiplication with vector", test_polynome_multiplication_with_vector());
    return 0;
}