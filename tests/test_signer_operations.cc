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

int main() {
    display_box("Testing dilithium signer operations");
    assert_value("Test sample in ball", test_sample_in_ball());
    return 0;
}