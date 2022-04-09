#include <stdlib.h>
#include <stdio.h>
#include <ntt_polynomial.h>
#include <polynomial.h>

const int N_TESTS = 1000;
const u_int32_t Q = 8380417;
const u_int32_t N = 256;
const u_int32_t r = 1753;

int main() {
	bool correct = true;
	for (int t = 0; t < N_TESTS && correct; t++) {
		struct polynomial<N, Q, r> p1 = polynomial<N, Q, r>::generate_random_polynomial();
		struct ntt_polynomial<N, Q, r> ntt_p1 = p1.foward_transform();

		struct polynomial<N, Q, r> p2 = polynomial<N, Q, r>::generate_random_polynomial();
		struct ntt_polynomial<N, Q, r> ntt_p2 = p2.foward_transform();

		struct polynomial<N, Q, r> expected_p3 = p1 * p2;
		struct ntt_polynomial<N, Q, r> ntt_p3 = ntt_p1 * ntt_p2;
		struct polynomial<N, Q, r> p3 = ntt_p3.backward_transform();

		for (int i = 0; i < N; i++) {
			correct = (p3[i] == expected_p3[i]);
			if (!correct) {
				printf("int a[] = { %d", p1[0]);
				for (int j = 1; j < N; j++) {
					printf(", %d", p1[j]);
				}
				printf(" };\n");
				printf("int b[] = { %d", p2[0]);
				for (int j = 1; j < N; j++) {
					printf(", %d", p2[j]);
				}
				printf(" };\n");
				printf("ERROR: c[%d] = %d != %d\n", i, p3[i], expected_p3[i]);
				return -1;
			}
		}
	}
	if (correct) {
		printf("OK!\n");
	}
	return 0;
}
