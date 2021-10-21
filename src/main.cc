#define OUTPUT

#include <key_generation.h>
#include <signer.h>
#include <verifier.h>
#include <dilithium_i.h>
#include <time.h>

int main(int argc, const char ** argv) {
	struct key_pair<K, L, N, Q> key_pair = generate_key_pair<K, L, N, Q, ETA>();

	// Generate random message
	byte message[48];
	for (int i = 0; i < 48; i++) {
		message[i] = rand() % 127;
	}

	struct signature<L, N, Q> signature = sign<BETA, GAMMA1, GAMMA2, K, L, N, Q>(key_pair.secret_key, message, 48);

	if (verify<BETA, GAMMA1, GAMMA2, K, L, N, Q>(signature, message, 48, key_pair.public_key)) {
		return 0;
	} else {
		return -1;
	}
}