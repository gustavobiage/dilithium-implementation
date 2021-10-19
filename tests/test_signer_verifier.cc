#define OUTPUT

#include <time.h>
#include <key_generation.h>
#include <verifier.h>
#include <signer.h>
#include <result.h>
#include <dilithium_i.h>

#define AMOUNT 1

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

    return all_valid;
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

    return all_valid;
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

int main() {
    // display_box("Testing Signer, Verifier and Key Generation");
    // assert_value("Test multiple messages with multiple keys", test_multiple_messages_with_multiple_keys());
    // assert_value("Test multiple messages with single key", test_multiple_messages_with_single_key());
    // assert_value("Test single message with multiple key", test_single_message_with_multiple_key());
    // assert_value("Test single message with single key", test_single_message_with_single_key());
    return 0;
}