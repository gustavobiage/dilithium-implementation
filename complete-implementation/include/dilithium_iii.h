#ifndef __dilithium_
#define __dilithium_

#include <common/dilithium.h>

namespace tcc {
	const unsigned int K = 6;
	const unsigned int L = 5;
	const unsigned int N = 256;
	const unsigned int Q = 8380417;
	const unsigned int W = 1753;
	const unsigned int ETA = 4;
	const unsigned int D = 13;
	const unsigned int GAMMA1 = (1 << 19);
	const unsigned int GAMMA2 = (Q - 1) / 32;
	const unsigned int BETA = 196;
	const unsigned int OMEGA = 55;
	const unsigned int TAU = 49;

	const unsigned int S1_PACKED_SIZE = L*128;
	const unsigned int S2_PACKED_SIZE = K*128;
	const unsigned int W1_PACKED_SIZE = K*128;
	const unsigned int Z_PACKED_SIZE  = L*640;
}

#include <dilithium_iii.cc>

#endif