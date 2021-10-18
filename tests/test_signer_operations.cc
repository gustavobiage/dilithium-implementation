#include <signer.h>
#include <result.h>
#include <polynomial.h>
#include <dilithium_i.h>

#define INDEX(x) (x+1)
#define MINUS_ONE 0
#define ZERO 1
#define ONE 2
const byte digest_byte = 85; // 01010101

int test_sample_in_ball() {
    byte digest[8];
    for (int i = 0; i < 8; i++) {
        digest[i] = digest_byte;
    }
    struct polynomial<N, Q> c = sample_in_ball<N, Q>(digest, 60);
    int cnt[3] = {0, 0, 0};
    for (int i = 0; i < N; i++) {
        cnt[INDEX(c[i])]++;
    }
    if (cnt[MINUS_ONE] != 30 || cnt[ZERO] != N - 60 || cnt[ONE] != 30) {
        return -1;
    }
    return 0;
}

int test_modular_operation() {
    int b;
    int gamma1 = GAMMA1;
    for (int i = 0; i <= 3*gamma1/2; i++) {
        b = cmod(i, gamma1);
        if (b <= -(gamma1)/2 || b > gamma1/2) {
            return -1;
        }
    }
    return 0;
}

int test_higher_and_low_order_bits() {
    unsigned int a = 123000; // TODO: test all values
    unsigned int b, c;
    b = high_order_bits<Q>(a, GAMMA2);
    c = low_order_bits<Q>(a, GAMMA2);
    if ((b | c) != a) {
        return -1;
    }
    return 0;
}

int test_bit_packing() {
    return -1;
}

int main() {
    display_box("Testing dilithium signer operations");
    assert_value("Test sample in ball", test_sample_in_ball());
    assert_value("Test modular operation", test_modular_operation());
    assert_value("Test higher and low order bits", test_higher_and_low_order_bits());
    return 0;
}