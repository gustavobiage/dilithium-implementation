#ifndef __signature_scheme_utils_
#define __signature_scheme_utils_

#include <utility>

typedef unsigned char byte;

template <unsigned int Q>
int32_t high_order_bits(int32_t, int32_t);

template <unsigned int Q>
int32_t low_order_bits(int32_t, int32_t);

template <unsigned int Q>
int32_t cmod(int32_t, int32_t);

template <unsigned int Q>
std::pair<int32_t, int32_t> decompose(int32_t, int32_t);

template <unsigned int M, unsigned int N, unsigned int Q>
void bit_packing(polynomial_vector<M, N, Q>, byte *);

template <unsigned int N, unsigned int Q>
polynomial<N, Q> sample_in_ball(byte *, byte *, int, int);

#include <signature_scheme_utils.cc>

#endif