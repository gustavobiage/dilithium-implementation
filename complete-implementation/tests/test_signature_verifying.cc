#include <abstract_test.h>

int main() {
	int ntests = NUMBER_OF_TESTS;
	while (ntests--) {
		tcc::key_pair<K, L, N, Q, W> key_pair = tcc::generate_key_pair<K, L, N, Q, W, ETA, D>();
		byte packed_public_key[tcc::PUBLIC_KEY_SIZE]; tcc::pack_public_key<K, L, N ,Q, W>(key_pair.public_key, (byte*) packed_public_key);

		byte message[MESSAGE_LENGTH];
		generate_random_message(message, MESSAGE_LENGTH);

		tcc::signature<K, L, N, Q, W> signature = tcc::sign<BETA, GAMMA1, GAMMA2, K, L, N, Q, W, OMEGA, TAU>(key_pair.secret_key, message, MESSAGE_LENGTH);
		byte packed_signature[tcc::SIGNATURE_SIZE]; tcc::pack_signature<K, L, N ,Q, W, OMEGA>(signature, (byte*) packed_signature);

		bool signature_valid = tcc::verify<BETA, GAMMA1, GAMMA2, K, L, N, Q, W, D, OMEGA, TAU>(signature, message, MESSAGE_LENGTH, key_pair.public_key);
		bool pq_crystal_signature_valid = pqcrystals_dilithium2_ref_verify(packed_signature, tcc::SIGNATURE_SIZE, message, MESSAGE_LENGTH, packed_public_key) == 0;

		if (DEBUG) {
			printf("signature valid             : "); if (signature_valid) printf("VALID\n"); else  printf("INVALID\n");
			printf("signature valid (pq-crystal): "); if (pq_crystal_signature_valid) printf("VALID\n"); else  printf("INVALID\n");
		}
		if (signature_valid != pq_crystal_signature_valid) {
			printf("Error testing singature verification\n");
			return -1;
		}
	}

	printf("Success at signature verification test\n");
	return 0;
}
