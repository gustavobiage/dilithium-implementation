#include <abstract_test.h>

int main() {
	int ntests = NUMBER_OF_TESTS;
	while (ntests--) {
		tcc::key_pair<K, L, N, Q, W> key_pair = tcc::generate_key_pair<K, L, N, Q, W, ETA, D>();
		byte packed_secret_key[tcc::SECRET_KEY_SIZE]; tcc::pack_secret_key<K, L, N ,Q, W, D>(key_pair.secret_key, (byte*) packed_secret_key);

		byte message[MESSAGE_LENGTH];
		generate_random_message(message, MESSAGE_LENGTH);

		tcc::signature<K, L, N, Q, W> signature = tcc::sign<BETA, GAMMA1, GAMMA2, K, L, N, Q, W, OMEGA, TAU>(key_pair.secret_key, message, MESSAGE_LENGTH);
		byte packed_signature[tcc::SIGNATURE_SIZE]; tcc::pack_signature<K, L, N ,Q, W, OMEGA>(signature, (byte*) packed_signature);

		uint8_t crystal_packed_signature[pqcrystals_dilithium_BYTES]; size_t siglen;
		pqcrystals_dilithium_ref_signature(crystal_packed_signature, &siglen, (uint8_t *) message, MESSAGE_LENGTH, (uint8_t *) packed_secret_key);

		tcc::signature<K, L, N, Q, W> pq_crystal_signature = tcc::unpack_signature<K, L, N, Q, W, OMEGA>(crystal_packed_signature);

		bool correct_signature_size = true;
		bool packed_signature_equal = true;
		bool sampled_c_til_equal = true;
		bool vector_z_equal = true;
		bool vector_h_equal = true;

		// Compare encoded signature
		if (DEBUG) {
			printf("signature size              : %d\n", tcc::SIGNATURE_SIZE);
			printf("signature size (pq-crystal) : %d\n", pqcrystals_dilithium_BYTES);
		}

		correct_signature_size = siglen == pqcrystals_dilithium_BYTES && pqcrystals_dilithium_BYTES == tcc::SIGNATURE_SIZE;
		// Compare encoded signature
		if (DEBUG) printf("encoded signature           : ");
		for (int i = 0; i < tcc::SIGNATURE_SIZE && packed_signature_equal; i++) {
			packed_signature_equal = (packed_signature[i] == crystal_packed_signature[i]);
		}
		if (DEBUG) if (!packed_signature_equal) printf("not equal!!\n"); else printf("equal!!\n");

		// Compare signature vector
		if (DEBUG) printf("signature sampled c~        : ");
		for (int i = 0; i < tcc::SAMPLED_C_TIL_SIZE && sampled_c_til_equal; i++) {
			sampled_c_til_equal = (signature.c_til[i] == pq_crystal_signature.c_til[i]);
		}
		if (DEBUG) if (!sampled_c_til_equal) printf("not equal!!\n"); else printf("equal!!\n");

		// Compare vector Z
		if (DEBUG) printf("signature vector z          : ");
		for (int i = 0; i < L && vector_z_equal; i++) {
			for (int j = 0; j < N && vector_z_equal; j++) {
				vector_z_equal = (signature.z[i][j] == pq_crystal_signature.z[i][j]);
			}
		}
		if (DEBUG) if (!vector_z_equal) printf("not equal!!\n"); else printf("equal!!\n");

		// Compare hint vector
		if (DEBUG) printf("signature hint              : ");
		for (int i = 0; i < K && vector_h_equal; i++) {
			for (int j = 0; j < N && vector_h_equal; j++) {
				vector_h_equal = (signature.h[i][j] == pq_crystal_signature.h[i][j]);
				if (!vector_h_equal) {
					printf("at position (%d, %d) %d vs %d\n", i, j, signature.h[i][j], pq_crystal_signature.h[i][j]);
				}
			}
		}
		if (DEBUG) if (!vector_h_equal) printf("not equal!!\n"); else printf("equal!!\n");

		if (!correct_signature_size || !packed_signature_equal ||                  // packed signature is not equal
			!sampled_c_til_equal    || !vector_z_equal         || !vector_h_equal) // unpacked signature is not equal
		{
			printf("correct_signature_size = "); PRTBOOL(correct_signature_size); printf("\n");
			printf("packed_signature_equal = "); PRTBOOL(packed_signature_equal); printf("\n");

			printf("sampled_c_til_equal = "); PRTBOOL(sampled_c_til_equal); printf("\n");
			printf("vector_z_equal = "); PRTBOOL(vector_z_equal); printf("\n");
			printf("vector_h_equal = "); PRTBOOL(vector_h_equal); printf("\n");

			printf("Error testing message signing\n");
			return -1;
		}
	}

	printf("Success at message signing with %d test.\n", NUMBER_OF_TESTS);
	return 0;
}
