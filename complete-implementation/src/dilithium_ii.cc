/* included in header file "dilithium_ii.h" */
#include <signature_scheme_utils.h>
#include <common/polynomial.h>

template <unsigned int LENGTH, unsigned int N, unsigned int Q, unsigned int W>
void tcc::pack_vector_s(struct tcc::polynomial_vector<LENGTH, N, Q, W> s, byte * output) {
  	int32_t aux[8];
	for (int i = 0; i < LENGTH; i++) {
		for (int j = 0; j < N/8; j++) {
			aux[0] = tcc::ETA - s[i][8*j + 0];
			aux[1] = tcc::ETA - s[i][8*j + 1];
			aux[2] = tcc::ETA - s[i][8*j + 2];
			aux[3] = tcc::ETA - s[i][8*j + 3];
			aux[4] = tcc::ETA - s[i][8*j + 4];
			aux[5] = tcc::ETA - s[i][8*j + 5];
			aux[6] = tcc::ETA - s[i][8*j + 6];
			aux[7] = tcc::ETA - s[i][8*j + 7];

			output[96*i + 3*j + 0] = (aux[0] >> 0) | (aux[1] << 3) | (aux[2] << 6);
			output[96*i + 3*j + 1] = (aux[2] >> 2) | (aux[3] << 1) | (aux[4] << 4) | (aux[5] << 7);
			output[96*i + 3*j + 2] = (aux[5] >> 1) | (aux[6] << 2) | (aux[7] << 5);
		}
	}
}

template <unsigned int LENGTH, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<LENGTH, N, Q, W> tcc::unpack_vector_s(byte * input, int input_size) {
	struct tcc::polynomial_vector<LENGTH, N, Q, W> s;
	for (int i = 0; i < LENGTH; i++) {
		for (int j = 0; j < N/8; j++) {
			s[i][8*j+0] =  (input[96*i + 3*j+0] >> 0) & 7;
		    s[i][8*j+1] =  (input[96*i + 3*j+0] >> 3) & 7;
		    s[i][8*j+2] = ((input[96*i + 3*j+0] >> 6) | (input[96*i + 3*j+1] << 2)) & 7;
		    s[i][8*j+3] =  (input[96*i + 3*j+1] >> 1) & 7;
		    s[i][8*j+4] =  (input[96*i + 3*j+1] >> 4) & 7;
		    s[i][8*j+5] = ((input[96*i + 3*j+1] >> 7) | (input[96*i + 3*j+2] << 1)) & 7;
		    s[i][8*j+6] =  (input[96*i + 3*j+2] >> 2) & 7;
		    s[i][8*j+7] =  (input[96*i + 3*j+2] >> 5) & 7;

		    s[i][8*j+0] = (ETA - s[i][8*j+0]);
		    s[i][8*j+1] = (ETA - s[i][8*j+1]);
		    s[i][8*j+2] = (ETA - s[i][8*j+2]);
		    s[i][8*j+3] = (ETA - s[i][8*j+3]);
		    s[i][8*j+4] = (ETA - s[i][8*j+4]);
		    s[i][8*j+5] = (ETA - s[i][8*j+5]);
		    s[i][8*j+6] = (ETA - s[i][8*j+6]);
		    s[i][8*j+7] = (ETA - s[i][8*j+7]);
		}
	}
	return s;
}

template <unsigned int K, unsigned int N, unsigned int Q, unsigned int W>
void tcc::pack_vector_w1(struct tcc::polynomial_vector<K, N, Q, W> w0, byte * output) {
	for (int i = 0; i < K; i++) {
		for(int j = 0; j < N/4; j++) {
			output[192*i + 3*j + 0]  = w0[i][4*j + 0];
			output[192*i + 3*j + 0] |= w0[i][4*j + 1] << 6;
			output[192*i + 3*j + 1]  = w0[i][4*j + 1] >> 2;
			output[192*i + 3*j + 1] |= w0[i][4*j + 2] << 4;
			output[192*i + 3*j + 2]  = w0[i][4*j + 2] >> 4;
			output[192*i + 3*j + 2] |= w0[i][4*j + 3] << 2;
		}
	}
}

template <unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
void tcc::pack_vector_z(struct tcc::polynomial_vector<L, N, Q, W> z, byte * output) {
	int32_t aux[4];
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < N/4; j++) {
			aux[0] = GAMMA1 - z[i][4*j + 0];
			aux[1] = GAMMA1 - z[i][4*j + 1];
			aux[2] = GAMMA1 - z[i][4*j + 2];
			aux[3] = GAMMA1 - z[i][4*j + 3];

			output[576*i + 9*j + 0]  = aux[0];
			output[576*i + 9*j + 1]  = aux[0] >> 8;
			output[576*i + 9*j + 2]  = aux[0] >> 16;
			output[576*i + 9*j + 2] |= aux[1] << 2;
			output[576*i + 9*j + 3]  = aux[1] >> 6;
			output[576*i + 9*j + 4]  = aux[1] >> 14;
			output[576*i + 9*j + 4] |= aux[2] << 4;
			output[576*i + 9*j + 5]  = aux[2] >> 4;
			output[576*i + 9*j + 6]  = aux[2] >> 12;
			output[576*i + 9*j + 6] |= aux[3] << 6;
			output[576*i + 9*j + 7]  = aux[3] >> 2;
			output[576*i + 9*j + 8]  = aux[3] >> 10;
		}
	}
}

template <unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<L, N, Q, W> tcc::unpack_vector_z(byte * input) {
	tcc:polynomial_vector<L, N, Q, W> z;
	for(int i = 0; i < L; ++i) {
		for (int j = 0; j < N/4; j++) {
			z[i][4*j + 0]  = input[576*i + 9*j + 0];
			z[i][4*j + 0] |= (uint32_t) input[576*i + 9*j + 1] << 8;
			z[i][4*j + 0] |= (uint32_t) input[576*i + 9*j + 2] << 16;
			z[i][4*j + 0] &= 0x3FFFF;

			z[i][4*j + 1]  = input[576*i + 9*j + 2] >> 2;
			z[i][4*j + 1] |= (uint32_t) input[576*i + 9*j + 3] << 6;
			z[i][4*j + 1] |= (uint32_t) input[576*i + 9*j + 4] << 14;
			z[i][4*j + 1] &= 0x3FFFF;

			z[i][4*j + 2]  = input[576*i + 9*j + 4] >> 4;
			z[i][4*j + 2] |= (uint32_t) input[576*i + 9*j + 5] << 4;
			z[i][4*j + 2] |= (uint32_t) input[576*i + 9*j + 6] << 12;
			z[i][4*j + 2] &= 0x3FFFF;

			z[i][4*j + 3]  = input[576*i + 9*j + 6] >> 6;
			z[i][4*j + 3] |= (uint32_t) input[576*i + 9*j + 7] << 2;
			z[i][4*j + 3] |= (uint32_t) input[576*i + 9*j + 8] << 10;
			z[i][4*j + 3] &= 0x3FFFF;

			z[i][4*j + 0] = GAMMA1 - z[i][4*j + 0];
			z[i][4*j + 1] = GAMMA1 - z[i][4*j + 1];
			z[i][4*j + 2] = GAMMA1 - z[i][4*j + 2];
			z[i][4*j + 3] = GAMMA1 - z[i][4*j + 3];
		}
	}
	return z;
}