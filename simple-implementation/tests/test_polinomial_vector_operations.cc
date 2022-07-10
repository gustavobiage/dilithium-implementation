#include <stdlib.h>
#include <polynomial.h>
#include <result.h>

const unsigned int Q = 10;
const unsigned int N = 1;
const unsigned int M = 5;
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
        v[i] = *(p[polynomes_indexes[i]]);
    }
    return v;
}

template <unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<M, 1, N, Q> create_column_matrix(int polynomes_indexes[M]) {
    struct polynomial_matrix<M, 1, N, Q> m;
    for (int i = 0; i < M; i++) {
        m[i][0] = *p[polynomes_indexes[i]];
    }
    return m;
}

template <unsigned int M, unsigned int N, unsigned int Q>
struct polynomial_matrix<1, M, N, Q> create_line_matrix(int polynomes_indexes[M]) {
    struct polynomial_matrix<1, M, N, Q> m;
    for (int i = 0; i < M; i++) {
        m[0][i] = *p[polynomes_indexes[i]];
    }
    return m;
}

int test_vector_sum_with_vector() {
    int a1[M] = {1, 6, 4, 4, 9};
    struct polynomial_vector<M, N, Q> v1 = create_vector<M, N, Q>(a1);
    int a2[M] = {5, 2, 3, 7, 8};
    struct polynomial_vector<M, N, Q> v2 = create_vector<M, N, Q>(a2);
    int a3[M] = {6, 8, 7, 1, 7};
    struct polynomial_vector<M, N, Q> v3 = v1 + v2;
    for (int i = 0; i < M; i++) {
        if (v3[i][0] != a3[i]) {
            return -1;
        }
    }
    return 0;
}

int test_vector_sum_with_matrix() {
    int a1[M] = {5, 7, 6, 0, 2};
    struct polynomial_vector<M, N, Q> v1 = create_vector<M, N, Q>(a1);
    int a2[M] = {5, 8, 5, 1, 3};
    struct polynomial_matrix<M, 1, N, Q> m1 = create_column_matrix<M, N, Q>(a2);
    int a3[M] = {0, 5, 1, 1, 5};
    struct polynomial_matrix<M, 1, N, Q> m2 = v1 + m1;
    for (int i = 0; i < M; i++) {
        if (m2[i][0][0] != a3[i]) {
            return -1;
        }
    }
    return 0;
}

int test_vector_subtraction_with_vector() {
    int a1[M] = {7, 7, 1, 0, 5};
    struct polynomial_vector<M, N, Q> v1 = create_vector<M, N, Q>(a1);
    int a2[M] = {4, 1, 0, 0, 6};
    struct polynomial_vector<M, N, Q> v2 = create_vector<M, N, Q>(a2);
    int a3[M] = {3, 6, 1, 0, 9};
    struct polynomial_vector<M, N, Q> v3 = v1 - v2;
    for (int i = 0; i < M; i++) {
        if (v3[i][0] != a3[i]) {
            printf("v3[%d][0] = %d; a3[%d] = %d\n", i, v3[i][0], i, a3[i]);
            return -1;
        }
    }
    return 0;
}

int test_vector_subtraction_with_matrix() {
    int a1[M] = {2, 6, 4, 8, 6};
    struct polynomial_vector<M, N, Q> v1 = create_vector<M, N, Q>(a1);
    int a2[M] = {3, 9, 4, 1, 3};
    struct polynomial_matrix<M, 1, N, Q> m1 = create_column_matrix<M, N, Q>(a2);
    int a3[M] = {9, 5, 0, 7, 3};
    struct polynomial_matrix<M, 1, N, Q> m2 = v1 - m1;
    for (int i = 0; i < M; i++) {
        if (m2[i][0][0] != a3[i]) {
            return -1;
        }
    }
    return 0;
}

int test_vector_conversion_to_matrix() {
    int a1[M] = {1, 5, 8, 9, 2};
    struct polynomial_vector<M, N, Q> v1 = create_vector<M, N, Q>(a1);
    struct polynomial_matrix<M, 1, N, Q> m1 = &v1;
    for (int i = 0; i < M; i++) {
        if (m1[i][0][0] != a1[i]) {
            return -1;
        }
    }
    return 0;
}

int test_vector_multiplication_with_matrix() {
    int a1[M] = {1, 6, 4, 2, 8};
    struct polynomial_vector<M, N, Q> v1 = create_vector<M, N, Q>(a1);
    int a2[M] = {6, 7, 1, 9, 9};
    struct polynomial_matrix<1, M, N, Q> m1 = create_line_matrix<M, N, Q>(a2);
    int a3[M][M] = {{6, 7, 1, 9, 9},
                    {6, 2, 6, 4, 4},
                    {4, 8, 4, 6, 6},
                    {2, 4, 2, 8, 8},
                    {8, 6, 8, 2, 2}};
    struct polynomial_matrix<M, M, N, Q> m2 = v1 * m1;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            if (m2[i][j][0] != a3[i][j]) {
                return -1;
            }
        }
    }
    return 0;
}

int main() {
    init();
    display_box("Testing polynomial vector operations");
    assert_value("Test vector sum with vector", test_vector_sum_with_vector());
    assert_value("Test vector sum with matrix", test_vector_sum_with_matrix());
    assert_value("Test vector subtraction with vector", test_vector_subtraction_with_vector());
    assert_value("Test vector subtraction with matrix", test_vector_sum_with_matrix());
    assert_value("Test vector conversion to matrix", test_vector_conversion_to_matrix());
    assert_value("Test vector multiplication with matrix", test_vector_multiplication_with_matrix());
    destroy();
    return 0;
}
