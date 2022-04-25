#ifndef __dilithium_
#define __dilithium_

#include <common/dilithium.h>

namespace tcc {
	const unsigned int K = 4;
	const unsigned int L = 4;
	const unsigned int N = 256;
	const unsigned int Q = 8380417;
	const unsigned int W = 1753;
	const unsigned int ETA = 2;
	const unsigned int D = 13;
	const unsigned int GAMMA1 = (Q - 1) / 16;
	const unsigned int GAMMA2 = GAMMA1 / 2;
	const unsigned int BETA = 375;
}


// TODO corrigir
#include <dilithium_ii.cc>

#endif