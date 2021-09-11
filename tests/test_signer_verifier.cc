#include <time.h>
#include <key_generation.h>
#include <verifier.h>
#include <signer.h>
#include <result.h>
#include <dilithium_i.h>

#define AMOUNT 15

byte * create_message() {
    // Generate random message
	byte * message = (byte *) malloc(sizeof(byte) * 48);
	for (int i = 0; i < 48; i++) {
		message[i] = rand() % 127;
	}
    return message;
}

void delete_message(byte* message) {
    free(message);
}

int test_multiple_messages_with_multiple_keys() {
    byte * messages[AMOUNT];
    for (int i = 0; i < AMOUNT; i++) {
        messages[i] = create_message();
    }
    struct key_pair<K, L, N, Q> key_pairs[AMOUNT];
    for (int i = 0; i < AMOUNT; i++) {
        key_pairs[i] = generate_key_pair<K, L, N, Q, ETA>();
    }
    struct signature<L, N, Q> signatures[AMOUNT];
    for (int i = 0; i < AMOUNT; i++) {
        signatures[i] = sign<BETA, GAMMA1, GAMMA2, K, L, N, Q>(key_pairs[i].secret_key, messages[i], 48);
    }

    bool all_valid = true;
    for (int i = 0; i < AMOUNT && all_valid; i++) {
        if (!verify<BETA, GAMMA1, GAMMA2, K, L, N, Q>(signatures[i], messages[i], 48, key_pairs[i].public_key)) {
            all_valid = false;
        }
    }

    for (int i = 0; i < AMOUNT; i++) {
        delete_message(messages[i]);
    }

    if (!all_valid) {
        return -1;
    }
    return 0;
}

int test_multiple_messages_with_single_key() {
    byte * messages[AMOUNT];
    for (int i = 0; i < AMOUNT; i++) {
        messages[i] = create_message();
    }
    
    struct key_pair<K, L, N, Q> key_pair = generate_key_pair<K, L, N, Q, ETA>();

    struct signature<L, N, Q> signatures[AMOUNT];
    for (int i = 0; i < AMOUNT; i++) {
        signatures[i] = sign<BETA, GAMMA1, GAMMA2, K, L, N, Q>(key_pair.secret_key, messages[i], 48);
    }

    bool all_valid = true;
    for (int i = 0; i < AMOUNT && all_valid; i++) {
        if (!verify<BETA, GAMMA1, GAMMA2, K, L, N, Q>(signatures[i], messages[i], 48, key_pair.public_key)) {
            all_valid = false;
        }
    }

    for (int i = 0; i < AMOUNT; i++) {
        delete_message(messages[i]);
    }

    if (!all_valid) {
        return -1;
    }
    return 0;
}

int test_single_message_with_multiple_key() {
    byte * message = create_message();
    struct key_pair<K, L, N, Q> key_pairs[AMOUNT];
    for (int i = 0; i < AMOUNT; i++) {
        key_pairs[i] = generate_key_pair<K, L, N, Q, ETA>();
    }
    struct signature<L, N, Q> signatures[AMOUNT];
    for (int i = 0; i < AMOUNT; i++) {
        signatures[i] = sign<BETA, GAMMA1, GAMMA2, K, L, N, Q>(key_pairs[i].secret_key, message, 48);
    }
    for (int i = 0; i < AMOUNT; i++) {
        if (!verify<BETA, GAMMA1, GAMMA2, K, L, N, Q>(signatures[i], message, 48, key_pairs[i].public_key)) {
            delete_message(message);
            return -1;
        }
    }
    delete_message(message);
    return 0;
}

int test_single_message_with_single_key() {
    byte * message = create_message();
    struct key_pair<K, L, N, Q> key_pair = generate_key_pair<K, L, N, Q, ETA>();
    struct signature<L, N, Q> signature = sign<BETA, GAMMA1, GAMMA2, K, L, N, Q>(key_pair.secret_key, message, 48);

    if (!verify<BETA, GAMMA1, GAMMA2, K, L, N, Q>(signature, message, 48, key_pair.public_key)) {
        delete_message(message);
        return -1;
    }

    delete_message(message);
    return 0;
}

int test_scheme_correctness() {
    const unsigned int iterations = 100;
    const int c1 = 30;
    const int c2 = 60;

    for (int it = 0; it < iterations; it++) {

        REJECT:

        polynomial_vector<L, N, Q> y;
        for (int i = 0; i < L; i++) {
            y[i] = polynomial<N, GAMMA1>::generate_random_polynomial();
        }

        polynomial<N, Q> c;
        for (int i = 0; i < c1; i++) {
            c[i] = -1;
        }
        for (int i = c1; i < c2; i++) {
            c[i] = 1;
        }
        for (int i = c2; i < N; i++) {
            c[i] = 0;
        }

        struct key_pair<K, L, N, Q> kp = generate_key_pair<K, L, N, Q, ETA>();
        polynomial_vector<L, N, Q> z = y + (c * kp.secret_key.s1);

        polynomial_vector<K, N, Q> Ay = kp.secret_key.A * y;
        polynomial_vector<K, N, Q> cs2 = c * kp.secret_key.s2;
        polynomial_vector<K, N, Q> a = Ay - cs2;

        polynomial_vector<K, N, Q> Az = kp.secret_key.A * z;
        polynomial_vector<K, N, Q> ct = c * kp.secret_key.t;
        polynomial_vector<K, N, Q> b = Az - ct;

        for (int i = 0; i < L; i++) {
            for (int j = 0; j < N; j++) {
                if (z[i][j] > GAMMA1 - BETA) {
                    goto REJECT;
                }
            }
        }

        for (int i = 0; i < K; i++) {
            for (int j = 0; j < N; j++) {
                int lower_bits = low_order_bits<Q>(b[i][j], 2*GAMMA2);
                if (lower_bits > (int)(GAMMA2 - BETA)) {
                    goto REJECT;
                }
            }
        }

        // Ay - cs2 must be equal to Az - ct
        for (int i = 0; i < K; i++) {
            for (int j = 0; j < N; j++) {
                if (a[i][j] != b[i][j]) {
                    return -1;
                }
            }
        }
    }
    return 0;
}

int main() {
    display_box("Testing Signer, Verifier and Key Generation");
    assert_value("Test scheme correctness", test_scheme_correctness());
    assert_value("Test multiple messages with multiple keys", test_multiple_messages_with_multiple_keys());
    assert_value("Test multiple messages with single key", test_multiple_messages_with_single_key());
    assert_value("Test single message with multiple key", test_single_message_with_multiple_key());
    assert_value("Test single message with single key", test_single_message_with_single_key());
    return 0;
}