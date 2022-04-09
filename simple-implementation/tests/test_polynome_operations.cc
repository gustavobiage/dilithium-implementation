#include <stdlib.h>
#include <polynomial.h>
#include <result.h>

const unsigned int Q = 10;
const unsigned int N = 5;
const unsigned int M = 3;
const unsigned int GAMMA = 5;

int test_random_polynome_generation() {
    struct polynomial<N, Q> p = polynomial<N, Q>::generate_random_polynomial();
    for (int i = 0; i < N; i++) {
        if (p[i] < 0 || p[i] >= Q) {
            return -1;
        }
    }
    struct polynomial_vector<M, N, Q> p2;
    for (int i = 0; i < M; i++) {
        p2[i] = polynomial<N, GAMMA>::generate_random_polynomial();
        for (int j = 0; j < N; j++) {
            if (p2[i][j] < 0 || p2[i][j] >= GAMMA) {
                return -1;
            }
        }
    }
    return 0;
}

int test_polynome_asignment() {
    const int c1[N] = {4, 9, 1, 5, 0};
    struct polynomial<N, Q> p1(c1);
    struct polynomial<N, Q> p2 = p1;
    for (int i = 0; i < N; i++) {
        if (p2[i] != c1[i]) {
            return -1;
        }
    }
    return 0;
}

int test_polynome_sum() {
    const int c1[N] = {3, 9, 5, 1, 3};
    const int c2[N] = {6, 9, 2, 3, 8};
    const int c3[N] = {9, 8, 7, 4, 1};
    struct polynomial<N, Q> p1(c1);
    struct polynomial<N, Q> p2(c2);
    struct polynomial<N, Q> p3 = p1 + p2;
    for (int i = 0; i < N; i++) {
        if (p3[i] != c3[i]) {
            return -1;
        }
    }
    return 0;
}

int test_polynome_subtraction() {
    const int c1[N] = {3, 9, 5, 1, 3};
    const int c2[N] = {6, 9, 2, 3, 8};
    const int c3[N] = {7, 0, 3, 8, 5};
    struct polynomial<N, Q> p1(c1);
    struct polynomial<N, Q> p2(c2);
    struct polynomial<N, Q> p3 = p1 - p2;
    for (int i = 0; i < N; i++) {
        if (p3[i] != c3[i]) {
            return -1;
        }
    }
    return 0;
}

int test_polynome_multiplication() {
    const int c1[N] = {2, 7, 4, 9, 0};
    const int c2[N] = {6, 6, 2, 8, 5};
    const int c3[N] = {7, 2, 5, 8, 8};
    struct polynomial<N, Q> p1(c1);
    struct polynomial<N, Q> p2(c2);
    struct polynomial<N, Q> p3 = p1 * p2;
    for (int i = 0; i < N; i++) {
        if (p3[i] != c3[i]) {
            return -1;
        }
    }
    return 0;
}

int test_polynome_multiplication_with_vector() {
    const int c1[N] = {4, 0, 1, 0, 4};
    struct polynomial<N, Q> p1(c1);
    const int c2[M][N] = {
        {4, 1, 6, 4, 3},
        {5, 5, 8, 4, 2},
        {1, 4, 3, 7, 8}
    };
    struct polynomial_vector<M, N, Q> v1;
    for (int i = 0; i < M; i++) {
        struct polynomial<N, Q> p(c2[i]);
        v1[i] = p;
    }
    const int c3[M][N] = {
        // 34*x^4 + 5*x^3 + 12*x^2 - 23*x + 8
        {8, 7, 2, 5, 4},
        // 36*x^4 + 13*x^3 + 21*x^2 - 14*x - 4
        {6, 6, 1, 3, 6},
        // 39*x^4 - 15*x^2 - 4*x - 19
        {1, 6, 5, 0, 9}
    };
    struct polynomial_vector<M, N, Q> v2 = p1 * v1;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (v2[i][j] != c3[i][j]) {
                return -1;
            }
        }
    }
    return 0;
}

int main() {
    display_box("Testing polynomial operations");
    assert_value("Test random polynome generation", test_random_polynome_generation());
    assert_value("Test polynome asignment", test_polynome_asignment());
    assert_value("Test polynome sum", test_polynome_sum());
    assert_value("Test polynome subtraction", test_polynome_subtraction());
    assert_value("Test polynome multiplication", test_polynome_multiplication());
    assert_value("Test polynome multiplication with vector", test_polynome_multiplication_with_vector());
    return 0;
}