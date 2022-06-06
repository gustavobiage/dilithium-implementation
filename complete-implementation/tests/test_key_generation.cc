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
	return 0;
	tcc::key_pair<K, L, N, Q, W> key_pair = tcc::generate_key_pair<K, L, N, Q, W, ETA, D>();
	uint8_t packed_public_key[tcc::PUBLIC_KEY_SIZE]; tcc::pack_public_key<K, L, N ,Q, W>(key_pair.public_key, (byte*) packed_public_key);
	uint8_t packed_secret_key[tcc::SECRET_KEY_SIZE]; tcc::pack_secret_key<K, L, N ,Q, W, D>(key_pair.secret_key, (byte*) packed_secret_key);

	uint8_t crystal_packed_public_key[pqcrystals_dilithium2_ref_PUBLICKEYBYTES];
	uint8_t crystal_packed_secret_key[pqcrystals_dilithium2_ref_SECRETKEYBYTES];
	pqcrystals_dilithium2_ref_keypair(crystal_packed_public_key, crystal_packed_secret_key);

	tcc::public_key<K, L, N, Q, W> public_key = tcc::unpack_public_key<K, L, N, Q, W, D>(crystal_packed_public_key);
	tcc::secret_key<K, L, N, Q, W> secret_key = tcc::unpack_secret_key<K, L, N, Q, W, D>(crystal_packed_secret_key);

	bool correct_key_sizes = true;
	bool packed_public_key_equal = true;
	bool packed_secret_key_equal = true;
	bool pk_seed_p_equal = true;
	bool pk_vector_t1_equal = true;
	bool sk_seed_p_equal = true;
	bool sk_seed_k_equal = true;
	bool sk_seed_tr_equal = true;
	bool sk_vector_s1_equal = true;
	bool sk_vector_s2_equal = true;
	bool sk_vector_t0_equal = true;

	// compare compressed keys
	printf("public key size             : %d\n", tcc::PUBLIC_KEY_SIZE);
	printf("public key size (pq-crystal): %d\n", pqcrystals_dilithium2_ref_PUBLICKEYBYTES);
	printf("secret key size             : %d\n", tcc::SECRET_KEY_SIZE);
	printf("secret key size (pq-crystal): %d\n", pqcrystals_dilithium2_ref_SECRETKEYBYTES);
	correct_key_sizes = (tcc::PUBLIC_KEY_SIZE == pqcrystals_dilithium2_ref_PUBLICKEYBYTES) && (tcc::SECRET_KEY_SIZE == pqcrystals_dilithium2_ref_SECRETKEYBYTES);
	printf("encoded public key          : ");
	for (int i = 0; i < tcc::PUBLIC_KEY_SIZE && packed_public_key_equal; i++) {
		packed_public_key_equal = (packed_public_key[i] == crystal_packed_public_key[i]);
	}
	if (!packed_public_key_equal) printf("not equal!!\n"); else printf("equal!!\n");
	printf("encoded secret key          : ");
	for (int i = 0; i < tcc::SECRET_KEY_SIZE && packed_secret_key_equal; i++) {
		packed_secret_key_equal = (packed_secret_key[i] == crystal_packed_secret_key[i]);
	}
	if (!packed_secret_key_equal) printf("not equal!!\n"); else printf("equal!!\n"); printf("\n");

	// Compare P in public key
	printf("public key p                : "); tcc::print_hex_string(key_pair.secret_key.p, tcc::SEED_P_SIZE); printf("\n");
	printf("public key p    (pq-crystal): "); tcc::print_hex_string(secret_key.p, tcc::SEED_P_SIZE); printf("\n");
	for (int i = 0; i < tcc::SEED_P_SIZE && pk_seed_p_equal; i++) {
		pk_seed_p_equal = key_pair.public_key.p[i] == public_key.p[i];
	}

	// compare vector t1 in public key
	printf("public key t1               : ");
	for (int i = 0; i < K && pk_vector_t1_equal; i++)
		for (int j = 0; j < N && pk_vector_t1_equal; j++)
			pk_vector_t1_equal = key_pair.public_key.t1[i][j] == public_key.t1[i][j];
	if (!pk_vector_t1_equal) printf("not equal!!\n"); else printf("equal!!\n");  printf("\n");

	// compare P in secret key
	printf("secret key p                : "); tcc::print_hex_string(key_pair.secret_key.p, tcc::SEED_P_SIZE); printf("\n");
	printf("secret key p    (pq-crystal): "); tcc::print_hex_string(secret_key.p, tcc::SEED_P_SIZE); printf("\n");
	for (int i = 0; i < tcc::SEED_P_SIZE && sk_seed_p_equal; i++) {
		sk_seed_p_equal = key_pair.secret_key.p[i] == secret_key.p[i];
	}
	// compare K in secret key
	printf("secret key k                : "); tcc::print_hex_string(key_pair.secret_key.k, tcc::SEED_K_SIZE); printf("\n");
	printf("secret key k    (pq-crystal): "); tcc::print_hex_string(secret_key.k, tcc::SEED_K_SIZE); printf("\n");
	for (int i = 0; i < tcc::SEED_K_SIZE && sk_seed_k_equal ; i++) {
		sk_seed_k_equal = key_pair.secret_key.k[i] == secret_key.k[i];
	}
	// compare TR in secret key
	printf("secret key tr               : "); tcc::print_hex_string(key_pair.secret_key.tr, tcc::SEED_TR_SIZE); printf("\n");
	printf("secret key tr   (pq-crystal): "); tcc::print_hex_string(secret_key.tr, tcc::SEED_TR_SIZE); printf("\n");
	for (int i = 0; i < tcc::SEED_TR_SIZE && sk_seed_tr_equal; i++) {
		sk_seed_tr_equal = key_pair.secret_key.tr[i] == secret_key.tr[i];
	}

	// compare vector s1 in secret key
	printf("secret key s1               : ");
	for (int i = 0; i < L && sk_vector_s1_equal; i++)
		for (int j = 0; j < N && sk_vector_s1_equal; j++)
			sk_vector_s1_equal = key_pair.secret_key.s1[i][j] == secret_key.s1[i][j];
	if (!sk_vector_s1_equal) printf("not equal!!\n"); else printf("equal!!\n");
	// compare vector s2 in secret key
	printf("secret key s2               : ");
	for (int i = 0; i < K && sk_vector_s2_equal; i++)
		for (int j = 0; j < N && sk_vector_s2_equal; j++)
			sk_vector_s2_equal = key_pair.secret_key.s2[i][j] == secret_key.s2[i][j];
	if (!sk_vector_s2_equal) printf("not equal!!\n"); else printf("equal!!\n");

	printf("secret key t0               : ");
	for (int i = 0; i < K && sk_vector_t0_equal; i++)
		for (int j = 0; j < N && sk_vector_t0_equal; j++)
			sk_vector_t0_equal = key_pair.secret_key.t0[i][j] == secret_key.t0[i][j];
	if (!sk_vector_t0_equal) printf("not equal!!\n"); else printf("equal!!\n");

	if (!correct_key_sizes || !packed_public_key_equal || !packed_secret_key_equal ||                      // Encoded keys are correct
		!pk_seed_p_equal   || !pk_vector_t1_equal      ||                                                  // Decoded public key is correct
		!sk_seed_p_equal   || !sk_seed_k_equal         || !sk_seed_tr_equal        || !sk_vector_s1_equal || !sk_vector_t0_equal) // Decoded secret key is correct
	{
		return -1;
	}

	return 0;
}
