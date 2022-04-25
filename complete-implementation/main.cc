#include <stdlib.h>
#include <stdio.h>
// #include "include/dilithium_ii.h"

// template <unsigned int N, unsigned int Q> struct polynomial;
// template <unsigned int M, unsigned int N, unsigned int Q> struct polynomial_vector;

/*
 * Polynomial in Ring Rq = Zq[X] / (X^n + 1)
 * Multiplication of polynomial can be represented as convolution operation in Zq^n lattice.
 */
// template <unsigned int N, unsigned int Q>
// struct polynomial {
// 	int32_t coefficients[N];

// 	polynomial() {}

// 	polynomial(const int [N]);

// 	int32_t & operator[](int);

// 	int32_t operator[](int) const;
// };

// template <unsigned int N, unsigned int Q>
// int32_t & polynomial<N, Q>::operator[](int index) {
//     return coefficients[index];
// }

// template <unsigned int N, unsigned int Q>
// int32_t polynomial<N, Q>::operator[](int index) const {
//     return coefficients[index];
// }

// template <unsigned int M, unsigned int N, unsigned int Q>
// struct polynomial_vector {
//     struct polynomial<N, Q> vector[M];

// 	polynomial_vector() {}

// 	struct polynomial<N, Q> & operator[](int);

// 	struct polynomial<N, Q> operator[](int) const;
// };


// template<unsigned int M, unsigned int N, unsigned int Q>
// struct polynomial<N, Q> & polynomial_vector<M, N, Q>::operator[](int index) {
//     return vector[index];
// }

// template<unsigned int M, unsigned int N, unsigned int Q>
// struct polynomial<N, Q> polynomial_vector<M, N, Q>::operator[](int index) const {
//     return vector[index];
// }
#include <cryptopp/osrng.h>
#include <cryptopp/shake.h>
#include <stdlib.h>

int main() {
	const int L = 5;
	const int M = 5;
	const int N = 3;
	int arr[L][M][N];
	srand(time(NULL));
	printf("= {");
	for (int i = 0; i < L; i++) {
		printf("{");
		for (int j = 0; j < M; j++) {
			if (!j) {
				printf("{");
			} else {
				printf(", {");
			}
			for (int k = 0; k < N; k++) {
				arr[i][j][k] = rand() % 10;
				if (!k) {
					printf("%d", arr[i][j][k]);
				} else {
					printf(", %d", arr[i][j][k]);
				}
			}
			printf("}");
		}
		if (i == L-1) {
			printf("}");
		} else {
			printf("},\n");
		}
	}
	printf("};\n");

	printf("Mat([");
	for (int i = 0; i < L; i++) {
		if (!i) {
			printf("[");
		} else {
			printf(", [");
		}
		for (int j = 0; j < M; j++) {
			if (!j) {
				printf("Mod(");
			} else {
				printf(", Mod(");
			}
			for (int k = 0; k < N; k++) {
				if (!k) {
					printf("%d*x^%d", arr[i][j][k], k);
				} else {
					printf("+ %d*x^%d", arr[i][j][k], k);
				}
			}
			printf(", x^%d + 1)", N);
		}
		printf("]");
	}
	printf("]~)");

	// for (int i = 0; i < 1000; i++) {
	// 	int a1 = rand() % 10;
	// 	int a2 = rand() % 10;
	// 	int a3 = rand() % 10;
	// 	printf("{%d, %d, %d} - Mod(%d + %d * x + %d * x^2)\n", a1, a2, a3, a1, a2, a3);
	// }
	// int arr[5][2] = {{0,1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}};
	// for (int i = 0; i < 5; i++) {
	// 	for (int j = 0; j < 2; j++) {
	// 		printf(" %d", arr[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// for (int i = 0; i < 15; i++) {
	// 	printf("%d = %d\n", i, i - (205*i >> 10)*5);
	// }
	// char output[10];
	// CryptoPP::OS_GenerateRandomBlock(false, (CryptoPP::byte *) output, 10);
	// for (int i = 0; i < 10; i++) {
	// 	printf("%d|", output[i]);
	// }
	// printf("\n");

	// CryptoPP::SHAKE256 shake((256 + 512 + 256)/8);
	// shake.Update((const CryptoPP::byte *) &output[0], 10);
	// char stream[(256 + 512 + 256)/8 + 1];
 //    shake.Final((CryptoPP::byte *)stream);
 //    stream[(256 + 512 + 256)/8] = 2;

 //    for (int i = 0; i <= (256 + 512 + 256) / 8; i++) {
 //    	printf("%d|", stream[i]);
 //    }
 //    printf("\n");

	// for (int i = 0; i < 10; i++) {
	// 	printf("%d\n", 1 << i);
	// }
	// Test nounce for expandA
	// for(int i = 0; i < K; ++i)
	//     for(int j = 0; j < L; ++j)
	//       printf("%d\n", (i << 8) + j);
	return 0;
}
