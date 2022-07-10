#ifndef __abstract_test_
#define __abstract_test_

// C
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

#if DILITHIUM_MODE == 2

#include <dilithium_ii.h>
#define pqcrystals_dilithium_ref_PUBLICKEYBYTES pqcrystals_dilithium2_ref_PUBLICKEYBYTES
#define pqcrystals_dilithium_ref_SECRETKEYBYTES pqcrystals_dilithium2_ref_SECRETKEYBYTES
#define pqcrystals_dilithium_ref_keypair pqcrystals_dilithium2_ref_keypair
#define pqcrystals_dilithium_BYTES pqcrystals_dilithium2_BYTES
#define pqcrystals_dilithium_ref_signature pqcrystals_dilithium2_ref_signature
#define pqcrystals_dilithium_ref_verify pqcrystals_dilithium2_ref_verify

#elif DILITHIUM_MODE == 5

#include <dilithium_v.h>
#define pqcrystals_dilithium_ref_PUBLICKEYBYTES pqcrystals_dilithium5_ref_PUBLICKEYBYTES
#define pqcrystals_dilithium_ref_SECRETKEYBYTES pqcrystals_dilithium5_ref_SECRETKEYBYTES
#define pqcrystals_dilithium_ref_keypair pqcrystals_dilithium5_ref_keypair
#define pqcrystals_dilithium_BYTES pqcrystals_dilithium5_BYTES
#define pqcrystals_dilithium_ref_signature pqcrystals_dilithium5_ref_signature
#define pqcrystals_dilithium_ref_verify pqcrystals_dilithium5_ref_verify

#else
FORCE COMPILATION ERROR
#endif

#include <signature_scheme_utils.h>
#include <common/packing.h>

const unsigned int MESSAGE_LENGTH = 1000;
const unsigned int NUMBER_OF_TESTS = 1000;

#define PRTBOOL(b) printf(b ? "true" : "false")

#define DEBUG 0

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

// includes from PQ-CRYSTAL
#include DILITHIUM_DIR(ref/api.h)

void randombytes(unsigned char* out, unsigned long outlen) {
	FILE * fd;
	int32_t ret;
	int32_t length = outlen;
	unsigned char * pointer = out;

	fd = fopen("/tmp/urandom", "rb");
	if (fd == NULL) {
		abort();
	}

	while(length > 0) {
		ret = fread(pointer, sizeof(unsigned char), length, fd);
		if (ferror(fd)) {
			printf("Tentando novamente\n");
			continue;
		} else if (feof(fd)) {
			printf("(pq-crystal) Não foi possível ler dados do arquivo /tmp/urandom\n");
			abort();
		}

		pointer += ret;
		length -= ret;
	}
	fclose(fd);
}

void generate_random_message(byte * message, int message_length) {
	// It is good enought for testing
	for (int i = 0; i < message_length; i++) {
		message[i] = rand() % 256;
	}
}

#endif