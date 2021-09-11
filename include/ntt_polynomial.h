// #ifndef __ntt_polynomial_
// #define __ntt_polynomial_

// #include <stdlib.h>
// #include <stdint.h>
// #include <math_utils.h>

// template <unsigned int Q> struct ntt_polynomial;
// template <unsigned int N, unsigned int Q> struct ntt_polynomial_vector;
// template <unsigned int M, unsigned int N, unsigned int Q> struct ntt_polynomial_matrix;

// // TODO: make clear that is a polynomial in polynomial ring q
// template <unsigned int Q>
// struct ntt_polynomial {
// 	int32_t coefficients[Q];

// 	struct ntt_polynomial& operator=(const struct ntt_polynomial&);

// 	struct ntt_polynomial& operator+(const struct ntt_polynomial&);

// 	struct ntt_polynomial& operator*(const struct ntt_polynomial&);

// 	struct ntt_polynomial& operator/(const struct ntt_polynomial&);
// };

// template<unsigned int N, unsigned int Q>
// struct ntt_polynomial_vector {
//     struct ntt_polynomial<Q> vector[N];

// 	struct ntt_polynomial<Q>& operator[](int);
// 	// Referencing returns matrix
// 	struct ntt_polynomial_matrix<N, 1, Q>& operator&();
// };

// template<unsigned int M, unsigned int N, unsigned int Q>
// struct ntt_polynomial_matrix {
//     struct ntt_polynomial_vector<N, Q> matrix[M];

// 	struct ntt_polynomial_vector<N, Q>& operator[](int);

// 	struct ntt_polynomial_matrix<M, N, Q>& operator+(const struct ntt_polynomial_matrix&);

// 	template <unsigned int M2, unsigned int N2>
// 	struct ntt_polynomial_matrix<M, N2, Q>& operator*(const struct ntt_polynomial_matrix<M2, N2, Q>&);
// };

// template<unsigned int Q>
// struct ntt_polynomial<Q> generate_random_polynomial();

// #define polynomial ntt_polynomial
// #endif