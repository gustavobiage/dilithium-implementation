#ifndef __dilithium_
#define __dilithium_

namespace tcc {
	const unsigned int K = 8;
	const unsigned int L = 7;
	const unsigned int N = 256;
	const unsigned int Q = 8380417;
	const unsigned int W = 1753;
	const unsigned int ETA = 2;
	const unsigned int D = 13;
	const unsigned int GAMMA1 = (1 << 19);
	const unsigned int GAMMA2 = (Q - 1) / 32;
	const unsigned int BETA = 120;
	const unsigned int OMEGA = 75;
	const unsigned int TAU = 60;

	const unsigned int S1_PACKED_SIZE = L*96;
	const unsigned int S2_PACKED_SIZE = K*96;
	const unsigned int W1_PACKED_SIZE = K*128;
	const unsigned int Z_PACKED_SIZE  = L*640;
}

#include <common/dilithium.h>

#include <dilithium_v.cc>

#endif