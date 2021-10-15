#include <stdlib.h>
#include <polynomial.h>
#include <result.h>
#include <string.h>

const unsigned int P = 5;
const unsigned int M = 5;
const unsigned int N = 1;
const unsigned int Q = 10;
struct polynomial<N, Q> * p[Q];

void init() {
    const int v[Q][N] = {{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}};
    for (int i = 0; i < Q; i++) {
        p[i] = new polynomial<N, Q>(v[i]);
    }
}

void destroy() {
    for (int i = 0; i < Q; i++) {
        delete p[i];
    }
}

template <unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_vector<M, N, Q> create_vector(int polynomes_indexes[M]) {
    struct polynomial_vector<M, N, Q> v;
    for (int i = 0; i < M; i++) {
        v[i] = *p[polynomes_indexes[i]];
    }
    return v;
}

template <unsigned int P, unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<P, M, N, Q> create_matrix(int polynomes_indexes[P][M]) {
    struct polynomial_matrix<P, M, N, Q> m;
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < M; j++) {
            m[i][j] = *p[polynomes_indexes[i][j]];
        }
    }
    return m;
}

template <unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<M, 1, N, Q> create_column_matrix(int polynomes_indexes[M]) {
    int indexes[M][1];
    memcpy(indexes, polynomes_indexes, sizeof(int) * M);
    return create_matrix<M, 1, N, Q>(indexes);
}

template <unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<1, M, N, Q> create_line_matrix(int polynomes_indexes[M]) {
    int indexes[1][M];
    memcpy(indexes, polynomes_indexes, sizeof(int) * M);
    return create_matrix<1, M, N, Q>(indexes);
}

int test_matrix_sum_with_vector() {
    int a1[M] = {1, 6, 3, 9, 6};
    struct polynomial_matrix<M, 1, N, Q> m1 = create_column_matrix<M, N, Q>(a1);
    int a2[M] = {7, 8, 9, 6, 2};
    struct polynomial_vector<M, N, Q> v1 = create_vector<M, N, Q>(a2);
    int a3[M] = {8, 4, 2, 5, 8};
    struct polynomial_matrix<M, 1, N, Q> m2 = m1 + v1;
    for (int i = 0; i < M; i++) {
        if (m2[i][0][0] != a3[i]) {
            return -1;
        }
    }
    return 0;
}

int test_matrix_sum_with_matrix() {
    int a1[P][M] = {{8, 4, 4, 7, 1},
                    {5, 8, 1, 1, 2},
                    {3, 9, 0, 5, 4},
                    {6, 2, 8, 9, 1},
                    {5, 4, 8, 7, 6}};
    struct polynomial_matrix<P, M, N, Q> m1 = create_matrix<P, M, N, Q>(a1);
    int a2[P][M] = {{6, 9, 1, 2, 7},
                    {4, 3, 5, 5, 8},
                    {3, 2, 5, 8, 1},
                    {5, 6, 1, 4, 9},
                    {9, 7, 8, 3, 6}};
    struct polynomial_matrix<P, M, N, Q> m2 = create_matrix<P, M, N, Q>(a2);
    int a3[P][M] = {{4, 3, 5, 9, 8},
                    {9, 1, 6, 6, 0},
                    {6, 1, 5, 3, 5},
                    {1, 8, 9, 3, 0},
                    {4, 1, 6, 0, 2}};
    struct polynomial_matrix<P, M, N, Q> m3 = m1 + m2;
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < M; j++) {
            if (m3[i][j][0] != a3[i][j]) {
                return -1;
            }
        }
    }
    return 0;
}

int test_matrix_subtraction_with_vector() {
    int a1[M] = {6, 3, 8, 7, 0};
    struct polynomial_matrix<M, 1, N, Q> m1 = create_column_matrix<M, N, Q>(a1);
    int a2[M] = {5, 7, 6, 9, 0};
    struct polynomial_vector<M, N, Q> v1 = create_vector<M, N, Q>(a2);
    int a3[M] = {1, 6, 2, 8, 0};
    struct polynomial_matrix<M, 1, N, Q> m2 = m1 - v1;
    for (int i = 0; i < M; i++) {
        if (m2[i][0][0] != a3[i]) {
            return -1;
        }
    }
    return 0;
}

int test_matrix_subtraction_with_matrix() {
    int a1[P][M] = {{9, 2, 5, 7, 6},
                    {5, 1, 5, 7, 3},
                    {3, 7, 5, 9, 8},
                    {0, 5, 0, 3, 6},
                    {1, 6, 5, 1, 2}};
    struct polynomial_matrix<P, M, N, Q> m1 = create_matrix<P, M, N, Q>(a1);
    int a2[P][M] = {{2, 4, 4, 1, 8},
                    {4, 7, 6, 4, 0},
                    {1, 5, 4, 7, 6},
                    {1, 5, 4, 8, 7},
                    {1, 6, 2, 4, 9}};
    struct polynomial_matrix<P, M, N, Q> m2 = create_matrix<P, M, N, Q>(a2);
    int a3[P][M] = {{7, 8, 1, 6, 8},
                    {1, 4, 9, 3, 3},
                    {2, 2, 1, 2, 2},
                    {9, 0, 6, 5, 9},
                    {0, 0, 3, 7, 3}};
    struct polynomial_matrix<P, M, N, Q> m3 = m1 - m2;
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < M; j++) {
            if (m3[i][j][0] != a3[i][j]) {
                return -1;
            }
        }
    }
    return 0;
}

int test_matrix_multiplication_with_vector() {
    return -1;
}

int test_matrix_multiplication_with_matrix() {
    return -1;
}

int test_matrix_conversion_to_vector() {
    int a[M] = {4, 7, 2, 1, 2};
    struct polynomial_matrix<M, 1, N, Q> m = create_column_matrix<M, N, Q>(a);
    struct polynomial_vector<M, N, Q> v = *m;
    for (int i = 0; i < M; i++) {
        if (v[i][0] != a[i]) {
            return -1;
        }
    }
    return 0;
}

int main() {
    init();
    display_box("Testing polynomial matrix operations");
    assert_value("Test matrix sum with vector", test_matrix_sum_with_vector());
    assert_value("Test matrix sum with matrix", test_matrix_sum_with_matrix());
    assert_value("Test matrix subtraction with vector", test_matrix_subtraction_with_vector());
    assert_value("Test matrix subtraction with matrix", test_matrix_subtraction_with_matrix());
    // ... (multiplication)
    assert_value("Test matrix conversion to vector", test_matrix_conversion_to_vector());
    destroy();
    return 0;
}
