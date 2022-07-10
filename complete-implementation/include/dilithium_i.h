#ifndef __dilithium_
#define __dilithium_

namespace tcc {

	const unsigned int K = 3;
	const unsigned int L = 2;
	const unsigned int Q = 8380417;
	const unsigned int N = 256;
	const unsigned int ETA = 7;
	const unsigned int GAMMA1 = (Q - 1) / 16;
	const unsigned int GAMMA2 = GAMMA1 / 2;
	const unsigned int BETA = 375;

}

#endif