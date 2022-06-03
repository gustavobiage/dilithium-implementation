#ifndef __common_signature_
#define __common_signature_

namespace tcc {
    template <unsigned int K, unsigned int L, unsigned int N, unsigned int Q, unsigned int W>
    struct signature {
    	byte c_til[SAMPLED_C_TIL_SIZE];
        polynomial_vector<L, N, Q, W> z;
        polynomial_vector<K, N, Q, W> h;
    };
}

#endif