#include <stdlib.h>
#include <polynomial.h>
#include <result.h>

const unsigned int Q = 10;
const unsigned int N = 5;

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
    const int c3[N] = {0, 2, 4, 0, 8};
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

int main() {
    display_box("Testing polynomial operations");
    assert_value("Test polynome asignment", test_polynome_asignment());
    assert_value("Test polynome sum", test_polynome_sum());
    assert_value("Test polynome subtraction", test_polynome_subtraction());
    assert_value("Test polynome multiplication", test_polynome_multiplication());
    return 0;
}