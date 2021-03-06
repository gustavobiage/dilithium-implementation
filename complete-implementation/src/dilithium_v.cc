/* included in header file "dilithium_v.h" */
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
		for(int j = 0; j < N/2; j++) {
			output[128*i + j + 0]  = w0[i][2*j + 0] | (w0[i][2*j + 1] << 4);
		}
	}
}

template <unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
void tcc::pack_vector_z(struct tcc::polynomial_vector<L, N, Q, W> z, byte * output) {
	int32_t aux[4];
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < N/2; j++) {
			aux[0] = GAMMA1 - z[i][2*j + 0];
			aux[1] = GAMMA1 - z[i][2*j + 1];

			output[640*i + 5*j + 0]  = aux[0];
			output[640*i + 5*j + 1]  = aux[0] >> 8;
			output[640*i + 5*j + 2]  = aux[0] >> 16;
			output[640*i + 5*j + 2] |= aux[1] << 4;
			output[640*i + 5*j + 3]  = aux[1] >> 4;
			output[640*i + 5*j + 4]  = aux[1] >> 12;
		}
	}
}

template <unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
struct tcc::polynomial_vector<L, N, Q, W> tcc::unpack_vector_z(byte * input) {
	tcc:polynomial_vector<L, N, Q, W> z;
	for(int i = 0; i < L; ++i) {
		for (int j = 0; j < N/2; j++) {
			z[i][2*j + 0]  = input[640*i + 5*j + 0];
			z[i][2*j + 0] |= (uint32_t) input[640*i + 5*j + 1] << 8;
			z[i][2*j + 0] |= (uint32_t) input[640*i + 5*j + 2] << 16;
			z[i][2*j + 0] &= 0xFFFFF;

			z[i][2*j + 1]  = input[640*i + 5*j + 2] >> 4;
			z[i][2*j + 1] |= (uint32_t) input[640*i + 5*j + 3] << 4;
			z[i][2*j + 1] |= (uint32_t) input[640*i + 5*j + 4] << 12;
			z[i][2*j + 1] &= 0xFFFFF;

			z[i][2*j + 0] = GAMMA1 - z[i][2*j + 0];
			z[i][2*j + 1] = GAMMA1 - z[i][2*j + 1];
		}
	}
	return z;
}