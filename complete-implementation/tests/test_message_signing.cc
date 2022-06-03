// includes from std library
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DECORATE(x)             <x>
#define MAKE_PATH(root, file)   DECORATE(root file)
#define DILITHIUM_DIR(file)     MAKE_PATH(pq_crystal_dilithium/, file)

// includes from my implementation
#include <key_generation.h>
#include <signer.h>
#include <dilithium_ii.h>
#include <signature_scheme_utils.h>
#include <common/packing.h>

const unsigned int K = tcc::K;
const unsigned int L = tcc::L;
const unsigned int N = tcc::N;
const unsigned int Q = tcc::Q;
const unsigned int W = tcc::W;
const unsigned int ETA = tcc::ETA;
const unsigned int D = tcc::D;

const unsigned int BETA = tcc::BETA;
const unsigned int GAMMA1 = tcc::GAMMA1;
const unsigned int GAMMA2 = tcc::GAMMA2;
const unsigned int  OMEGA = tcc::OMEGA;
const unsigned int TAU = tcc::TAU;

const unsigned int MESSAGE_LENGTH = 1000;

// includes from PQ-CRYSTAL
#define DILITHIUM_MODE 2
#include DILITHIUM_DIR(ref/api.h)

void randombytes(unsigned char* out, unsigned long outlen) {
	static int fd = -1;
	int32_t ret;
	int32_t length = outlen;
	unsigned char * pointer = out;
	while(fd == -1) {
		fd = open("/tmp/urandom", O_RDONLY);
		if(fd == -1 && errno == EINTR)
			continue;
		else if(fd == -1)
			abort();
	}

	while(length > 0) {
		ret = read(fd, pointer, length);
		if(ret == -1 && errno == EINTR)
			continue;
		else if(ret == -1)
			abort();

		pointer += ret;
		length -= ret;
	}
}

int main() {
	tcc::key_pair<K, L, N, Q, W> key_pair = tcc::generate_key_pair<K, L, N, Q, W, ETA, D>();
	byte packed_secret_key[tcc::SECRET_KEY_SIZE]; tcc::pack_secret_key<K, L, N ,Q, W, D>(key_pair.secret_key, (byte*) packed_secret_key);

	byte message[MESSAGE_LENGTH];
	memcpy(message, "MINHA MENSAGEM ASSINADA", 24);
	// compute random message

	tcc::signature<K, L, N, Q, W> signature = tcc::sign<BETA, GAMMA1, GAMMA2, K, L, N, Q, W, OMEGA, TAU>(key_pair.secret_key, message, MESSAGE_LENGTH);

	for (int i = 0; i < 10; i++) {
		printf("\n\n\n");
	}

	uint8_t crystal_packed_signature[pqcrystals_dilithium2_BYTES];
	size_t siglen;
	pqcrystals_dilithium2_ref_signature(crystal_packed_signature, &siglen, (uint8_t *) message, MESSAGE_LENGTH, (uint8_t *) packed_secret_key);
	if (siglen != pqcrystals_dilithium2_BYTES) {
		return -1;
	}

	return 0;
}
