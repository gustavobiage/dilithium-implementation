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
#include <verifier.h>
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
const unsigned int OMEGA = tcc::OMEGA;
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
	byte packed_public_key[tcc::PUBLIC_KEY_SIZE]; tcc::pack_public_key<K, L, N ,Q, W>(key_pair.public_key, (byte*) packed_public_key);

	byte message[MESSAGE_LENGTH];
	memcpy(message, "MINHA MENSAGEM ASSINADA", 24);
	// compute random message

	tcc::signature<K, L, N, Q, W> signature = tcc::sign<BETA, GAMMA1, GAMMA2, K, L, N, Q, W, OMEGA, TAU>(key_pair.secret_key, message, MESSAGE_LENGTH);
	byte packed_signature[tcc::SIGNATURE_SIZE]; tcc::pack_signature<K, L, N ,Q, W, OMEGA>(signature, (byte*) packed_signature);

	bool signature_valid = tcc::verify<BETA, GAMMA1, GAMMA2, K, L, N, Q, W, D, TAU>(signature, message, MESSAGE_LENGTH, key_pair.public_key);
	bool pq_crystal_signature_valid = pqcrystals_dilithium2_ref_verify(packed_signature, tcc::SIGNATURE_SIZE, message, MESSAGE_LENGTH, packed_public_key) == 0;

	printf("signature valid             : ");
	if (signature_valid) printf("VALID\n"); else  printf("INVALID\n");
	printf("signature valid (pq-crystal): ");
	if (pq_crystal_signature_valid) printf("VALID\n"); else  printf("INVALID\n");

	if (signature_valid != pq_crystal_signature_valid) {
		return -1;
	}

	return 0;
}
