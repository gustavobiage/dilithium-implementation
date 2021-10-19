#include <signer.h>
#include <result.h>
#include <polynomial.h>
#include <dilithium_i.h>

#define INDEX(x) (x+1)
#define MINUS_ONE 0
#define ZERO 1
#define ONE 2

#define PACKED(x, y) ((x << 4) | y)

const byte digest_byte = 85; // 01010101

const unsigned int m = 10;
const unsigned int n = 1;
const unsigned int q = 10;

struct polynomial<n, q> * p[Q];

void init() {
    const int v[q][n] = {{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}};
    for (int i = 0; i < q; i++) {
        p[i] = new polynomial<n, q>(v[i]);
    }
}

void destroy() {
    for (int i = 0; i < q; i++) {
        delete p[i];
    }
}
template <unsigned int M_, unsigned int N_, unsigned int Q_>
struct polynomial_vector<M_, N_, Q_> create_vector(int polynomes_indexes[M_]) {
    struct polynomial_vector<M_, N_, Q_> v;
    for (int i = 0; i < M_; i++) {
        v[i] = *p[polynomes_indexes[i]];
    }
    return v;
}

int test_sample_in_ball() {
    const int w1_packed_size = 10;

    byte message[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    byte w1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    struct polynomial<N, Q> c1 = sample_in_ball<N, Q>(message, w1, w1_packed_size, 60);
    struct polynomial<N, Q> c2 = sample_in_ball<N, Q>(message, w1, w1_packed_size, 60);
    
    int cnt1[3] = {0, 0, 0};
    int cnt2[3] = {0, 0, 0};
    for (int i = 0; i < N; i++) {
        cnt1[INDEX(c1[i])]++;
        cnt2[INDEX(c2[i])]++;
        if (c1[i] != c2[i]) {
            return -1;
        }
    }

    if (cnt1[MINUS_ONE] + cnt1[ONE] != 60 || cnt1[ZERO] != N - 60 ||
        cnt2[MINUS_ONE] + cnt2[ONE] != 60 || cnt2[ZERO] != N - 60) {
        return -1;
    }

    return 0;
}

int test_modular_operation() {
    int b;
    int _2_gamma2 = 2*GAMMA2;
    for (int i = 0; i <= 3*_2_gamma2/2; i++) {
        b = cmod(i, _2_gamma2);
        // assert range
        if (b <= -(_2_gamma2)/2 || b > _2_gamma2/2) {
            return -1;
        }
        // assert result
        if (i <= _2_gamma2/2 && b != i) {
            return -1;
        } else if (i > _2_gamma2/2 && b + _2_gamma2 != i) {
            return -1;
        }
        
    }
    return 0;
}

int test_higher_and_low_order_bits() {
    int _2_gamma2 = 2*GAMMA2;
    for (int i = 0; i <= 3*_2_gamma2/2; i++) {
        unsigned int w = i;
        unsigned int higher, lower;
        higher = high_order_bits<Q>(w, _2_gamma2);
        lower = low_order_bits<Q>(w, _2_gamma2);
        if (higher * _2_gamma2 + lower != w) {
            return -1;
        }
        if (GAMMA2 < lower && lower < -GAMMA2) {
            return -1;
        }
    }
    return 0;
}

int test_bit_packing() {
    int a1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct polynomial_vector<m, n, q> p = create_vector<m, n, q>(a1);
    byte packed[5];
    bit_packing<m, n, q>(p, packed);
    byte a2[] = {PACKED(0, 1), PACKED(2, 3), PACKED(4, 5), PACKED(6, 7), PACKED(8, 9)};
    for (int i = 0; i < 5; i++) {
        if (packed[i] != a2[i]) {
            printf("%d != %d\n", packed[i], a2[i]);
            return -1;
        }
    }
    return 0;
}

int main() {
    init();
    display_box("Testing signature scheme utils");
    assert_value("Test sample in ball", test_sample_in_ball());
    assert_value("Test modular operation", test_modular_operation());
    assert_value("Test higher and low order bits", test_higher_and_low_order_bits());
    assert_value("Test bit packing", test_bit_packing());
    destroy();
    return 0;
}