#define OUTPUT

#include <dilithium_ii.h>
#include <key_generation.h>
#include <signer.h>
#include <verifier.h>
#include <time.h>

int main(int argc, const char ** argv) {
	struct tcc::key_pair<tcc::K, tcc::L, tcc::N, tcc::Q, tcc::W> key_pair = tcc::generate_key_pair<tcc::K, tcc::L, tcc::N, tcc::Q, tcc::W, tcc::ETA, tcc::D>();

	// Generate random message
	byte message[48];
	for (int i = 0; i < 48; i++) {
		message[i] = rand() % 127;
	}

	struct tcc::signature<tcc::L, tcc::N, tcc::Q, tcc::W> signature = tcc::sign<tcc::BETA, tcc::GAMMA1, tcc::GAMMA2, tcc::K, tcc::L, tcc::N, tcc::Q, tcc::W>(key_pair.secret_key, message, 48);

	if (verify<tcc::BETA, tcc::GAMMA1, tcc::GAMMA2, tcc::K, tcc::L, tcc::N, tcc::Q, tcc::W>(signature, message, 48, key_pair.public_key)) {
		return 0;
	} else {
		return -1;
	}
}