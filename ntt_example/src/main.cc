#include <stdlib.h>
#include <stdio.h>
#include <ntt_polynomial.h>
#include <polynomial.h>

const int N_TESTS = 1000;
const u_int32_t Q = 8380417;
const u_int32_t N = 256;
const u_int32_t r = 1753;
const u_int32_t D = 13;
const u_int32_t GAMMA2 = (Q - 1) / 88;

namespace test {
	const unsigned int QINV = 58728449;
	int32_t montgomery_reduce(int64_t a) {
		int32_t t;

		t = (int64_t)(int32_t)a*QINV;
		t = (a - (int64_t)t*Q) >> 32;
		return ((int64_t)t + Q) % Q;
	}

	static const int32_t zetas[N] = {
	         0,    25847, -2608894,  -518909,   237124,  -777960,  -876248,   466468,
	   1826347,  2353451,  -359251, -2091905,  3119733, -2884855,  3111497,  2680103,
	   2725464,  1024112, -1079900,  3585928,  -549488, -1119584,  2619752, -2108549,
	  -2118186, -3859737, -1399561, -3277672,  1757237,   -19422,  4010497,   280005,
	   2706023,    95776,  3077325,  3530437, -1661693, -3592148, -2537516,  3915439,
	  -3861115, -3043716,  3574422, -2867647,  3539968,  -300467,  2348700,  -539299,
	  -1699267, -1643818,  3505694, -3821735,  3507263, -2140649, -1600420,  3699596,
	    811944,   531354,   954230,  3881043,  3900724, -2556880,  2071892, -2797779,
	  -3930395, -1528703, -3677745, -3041255, -1452451,  3475950,  2176455, -1585221,
	  -1257611,  1939314, -4083598, -1000202, -3190144, -3157330, -3632928,   126922,
	   3412210,  -983419,  2147896,  2715295, -2967645, -3693493,  -411027, -2477047,
	   -671102, -1228525,   -22981, -1308169,  -381987,  1349076,  1852771, -1430430,
	  -3343383,   264944,   508951,  3097992,    44288, -1100098,   904516,  3958618,
	  -3724342,    -8578,  1653064, -3249728,  2389356,  -210977,   759969, -1316856,
	    189548, -3553272,  3159746, -1851402, -2409325,  -177440,  1315589,  1341330,
	   1285669, -1584928,  -812732, -1439742, -3019102, -3881060, -3628969,  3839961,
	   2091667,  3407706,  2316500,  3817976, -3342478,  2244091, -2446433, -3562462,
	    266997,  2434439, -1235728,  3513181, -3520352, -3759364, -1197226, -3193378,
	    900702,  1859098,   909542,   819034,   495491, -1613174,   -43260,  -522500,
	   -655327, -3122442,  2031748,  3207046, -3556995,  -525098,  -768622, -3595838,
	    342297,   286988, -2437823,  4108315,  3437287, -3342277,  1735879,   203044,
	   2842341,  2691481, -2590150,  1265009,  4055324,  1247620,  2486353,  1595974,
	  -3767016,  1250494,  2635921, -3548272, -2994039,  1869119,  1903435, -1050970,
	  -1333058,  1237275, -3318210, -1430225,  -451100,  1312455,  3306115, -1962642,
	  -1279661,  1917081, -2546312, -1374803,  1500165,   777191,  2235880,  3406031,
	   -542412, -2831860, -1671176, -1846953, -2584293, -3724270,   594136, -3776993,
	  -2013608,  2432395,  2454455,  -164721,  1957272,  3369112,   185531, -1207385,
	  -3183426,   162844,  1616392,  3014001,   810149,  1652634, -3694233, -1799107,
	  -3038916,  3523897,  3866901,   269760,  2213111,  -975884,  1717735,   472078,
	   -426683,  1723600, -1803090,  1910376, -1667432, -1104333,  -260646, -3833893,
	  -2939036, -2235985,  -420899, -2286327,   183443,  -976891,  1612842, -3545687,
	   -554416,  3919660,   -48306, -1362209,  3937738,  1400424,  -846154,  1976782
	};

	/*************************************************
	* Name:        ntt
	*
	* Description: Forward NTT, in-place. No modular reduction is performed after
	*              additions or subtractions. Output vector is in bitreversed order.
	*
	* Arguments:   - uint32_t p[N]: input/output coefficient array
	**************************************************/
	void ntt(int32_t a[N]) {
	  unsigned int len, start, j, k;
	  int32_t zeta, t;

	  k = 0;
	  for(len = 128; len > 0; len >>= 1) {
	    for(start = 0; start < N; start = j + len) {
	      zeta = zetas[++k];
	      for(j = start; j < start + len; ++j) {
	        t = montgomery_reduce((int64_t)zeta * a[j + len]);
	        a[j + len] = a[j] - t;
	        a[j] = a[j] + t;
	      }
	    }
	  }
	}

	/*************************************************
	* Name:        invntt_tomont
	*
	* Description: Inverse NTT and multiplication by Montgomery factor 2^32.
	*              In-place. No modular reductions after additions or
	*              subtractions; input coefficients need to be smaller than
	*              Q in absolute value. Output coefficient are smaller than Q in
	*              absolute value.
	*
	* Arguments:   - uint32_t p[N]: input/output coefficient array
	**************************************************/
	void invntt_tomont(int32_t a[N]) {
	  unsigned int start, len, j, k;
	  int32_t t, zeta;
	  const int32_t f = 41978; // mont^2/256

	  k = 256;
	  for(len = 1; len < N; len <<= 1) {
	    for(start = 0; start < N; start = j + len) {
	      zeta = -zetas[--k];
	      for(j = start; j < start + len; ++j) {
	        t = a[j];
	        a[j] = t + a[j + len];
	        a[j + len] = t - a[j + len];
	        a[j + len] = montgomery_reduce((int64_t)zeta * a[j + len]);
	      }
	    }
	  }

	  for(j = 0; j < N; ++j) {
	    a[j] = montgomery_reduce((int64_t)f * a[j]);
	  }
	}

	/*************************************************
	* Name:        poly_pointwise_montgomery
	*
	* Description: Pointwise multiplication of polynomials in NTT domain
	*              representation and multiplication of resulting polynomial
	*              by 2^{-32}.
	*
	* Arguments:   - poly *c: pointer to output polynomial
	*              - const poly *a: pointer to first input polynomial
	*              - const poly *b: pointer to second input polynomial
	**************************************************/
	void poly_pointwise_montgomery(int32_t * c, const int32_t *a, const int32_t *b) {
	  unsigned int i;
	  for(i = 0; i < N; ++i)
	    c[i] = montgomery_reduce((int64_t)a[i] * b[i]);
	}

	/*************************************************
	* Name:        reduce32
	*
	* Description: For finite field element a with a <= 2^{31} - 2^{22} - 1,
	*              compute r \equiv a (mod Q) such that -6283009 <= r <= 6283007.
	*
	* Arguments:   - int32_t: finite field element a
	*
	* Returns r.
	**************************************************/
	int32_t reduce32(int32_t a) {
	  int32_t t;
	  t = (a + (1 << 22)) >> 23;
	  t = a - t*Q;
	  return t;
	}

	/*************************************************
	* Name:        power2round
	*
	* Description: For finite field element a, compute a0, a1 such that
	*              a mod^+ Q = a1*2^D + a0 with -2^{D-1} < a0 <= 2^{D-1}.
	*              Assumes a to be standard representative.
	*
	* Arguments:   - int32_t a: input element
	*              - int32_t *a0: pointer to output element a0
	*
	* Returns a1.
	**************************************************/
	int32_t power2round(int32_t *a0, int32_t a)  {
	  int32_t a1;

	  a1 = (a + (1 << (D-1)) - 1) >> D;
	  *a0 = a - (a1 << D);
	  return a1;
	}

	int32_t decompose(int32_t *a0, int32_t a) {
		int32_t a1;

		a1  = (a + 127) >> 7;
	#if GAMMA2 == (Q-1)/32
		a1  = (a1*1025 + (1 << 21)) >> 22;
		a1 &= 15;
	#elif GAMMA2 == (Q-1)/88
		a1  = (a1*11275 + (1 << 23)) >> 24;
		a1 ^= ((43 - a1) >> 31) & a1;
	#endif

		*a0  = a - a1*2*GAMMA2;
		*a0 -= (((Q-1)/2 - *a0) >> 31) & Q;
		return a1;
	}
}
#include <utility>

template <unsigned int Q>
int32_t cmod(int32_t r, int32_t alpha) {
    if (r < 0 || r >= Q) {
        char message[100];
        sprintf(message, "centralized reduction expects r (as %d) in interval [0, %d]", r, Q);
        throw std::domain_error(message);
    } else {
        /* https://d-nb.info/1204223297/34; Reference Implementation p254
         *
         * Instead we use the following well-known trick to compute the centralized remainder 
         * r′= r mod ± α where 0 ≤ r ≤ 3α/2. Subtracting α/2 + 1 from r yields a negative result
         * if and only if r ≤ α/2. Therefore, shifting this result arithmetically to the right by
         * 31 bits gives −1, i.e. the integer with all bits equal to 1, if r ≤ α/2 and 0 otherwise.
         * Then the logical AND of the shifted value and α is added to r and α/2 − 1 subtracted.
         * This results in r − α if r > α/2 and r if r ≤ α/2, i.e. the centralized remainder.
         */
        r = r % alpha;
        r = r - (alpha/2 + 1);
        int shift = (r >> 31);
        r = r + (shift & alpha);
        r = r - (alpha/2 - 1);
        return r;
    }
}

template <unsigned int Q, unsigned int D>
std::pair<int32_t, int32_t> power_2_round(int32_t r) {
    r = r % Q;
    int32_t r0, r1;
    r0 = cmod<Q>(r, (1 << D));
    return std::make_pair( ((r - r0) >> D), r0);
}

#include <utility>

template <unsigned int Q>
std::pair<int32_t, int32_t> decompose(int32_t w, int32_t alpha) {
    w = ((int64_t) w + Q) % Q;
    int32_t w0, w1;
    w0 = cmod<Q>(w, alpha);
    // w0 = ((int64_t) w0 + Q) % Q;
    if (w - w0 == Q - 1) {
        w1 = 0;
        w0 = Q-1;
    } else {
        w1 = (w - w0) / alpha;
    }
    return std::make_pair(w1, w0);
}

int main() {
	int n, n2, n2_0, n2_1;
	n = 8328515;
	// for (n = 0; n < Q; n++) {
		std::pair<int32_t, int32_t> p = decompose<Q>((int32_t) n, (int32_t) 2*GAMMA2);
		n2 = n;
		n2_1 = test::decompose(&n2_0, n2);
		if (n2_0 != p.second || n2_1 != p.first) {
			printf("ERROR ON %d: %d %d %d %d\n", n, p.first, p.second, n2_1, n2_0);
		}
	// }
	printf("END\n");
	return 0;

	// uint32_t fa = 39613835;
	// uint32_t fb = 6092167;

	// bit reverse + inverse montgomery
	// {
	// 	int aux = (int64_t) fa * -114592 % Q;
	// 	aux = inverse_montgomery_reduction<Q>(aux);
	// 	printf("bit reverse + inverse montgomery = %d\n", aux);
	// }
	// // bit reverse + montgomery
	// {
	// 	int aux = (int64_t) fa * -114592 % Q;
	// 	aux = montgomery_reduction<Q>(aux);
	// 	printf("bit reverse + montgomery = %d\n", aux);
	// }
	// // inverse montgomery
	// {
	// 	int aux = fa;
	// 	aux = inverse_montgomery_reduction<Q>(aux);
	// 	printf("inverse montgomery = %d\n", aux);
	// }
	// // bit reverse + montgomery
	// {
	// 	int aux = fa;
	// 	aux = montgomery_reduction<Q>(aux);
	// 	printf("montgomery = %d\n", aux);
	// }
	// // bit reverse + montgomery
	// {
	// 	int aux = test::reduce32(fa);
	// 	printf("montgomery = %d\n", aux);
	// }
	// // reduce + inverse montgomery
	// {
	// 	int aux = test::reduce32(fa);
	// 	aux = inverse_montgomery_reduction<Q>(aux);
	// 	printf("reduce inverse montgomery = %d\n", aux);
	// }
	// // reduce + bit reverse + montgomery
	// {
	// 	int aux = test::reduce32(fa);
	// 	aux = montgomery_reduction<Q>(aux);
	// 	printf("reduce montgomery = %d\n", aux);
	// }

	// {
	// 	int aux = multiply(2365951, fa, Q);
	// 	// aux = montgomery_reduction<Q>(aux);
	// 	printf("reduce montgomery = %d\n", aux);
	// }
	// return;
	//


	// for (int i = 0; i < N; i++) {
	// 	printf(" %d", polynomial<N, Q, r>::zetas[i]);
	// }
	// printf("\n======\n");
	// for (int i = 0; i < N; i++) {
	// 	int32_t zeta_reducted = montgomery_reduction<Q>(polynomial<N, Q, r>::zetas[i]);
	// 	printf(" %d", zeta_reducted);
	// }
	// printf("\n======\n");
	// for (int i = 0; i < N; i++) {
	// 	int32_t zeta_reducted = montgomery_reduction<Q>(polynomial<N, Q, r>::zetas[i]);
	// 	printf(" %d(%d)", inverse_montgomery_reduction<Q>(zeta_reducted), test::montgomery_reduce(zeta_reducted));
	// }
	// for (int i = 1; i < 10; i++) {
	// 	for (int k = 1; k < 10; k++) {
	// 		printf("%d %d\n", montgomery_reduction<Q>(i*k), multiply_montgomery<Q>(i, k, Q));
	// 	}
	// }

	// for (int i = 0; i < Q; i++) {
	// 	int32_t a0;
	// 	int32_t a1 = test::power2round(&a0, i);
	// 	std::pair<int32_t, int32_t> pair_ = power_2_round<Q, D>(i);
	// 	int32_t b0, b1;
	// 	b1 = pair_.first;
	// 	b0 = pair_.second;
	// 	printf("(%d %d); (%d, %d) -", a1, a0, b1, b0);
	// }

	bool correct = true;
	for (int t = 0; t < N_TESTS && correct; t++) {
		struct polynomial<N, Q, r> p1 = polynomial<N, Q, r>::generate_random_polynomial();
		struct ntt_polynomial<N, Q, r> ntt_p1 = p1.foward_transform();
		struct ntt_polynomial<N, Q, r> other_ntt_p1;

		int32_t aa[N];
		for (int i = 0; i < N; i++)
			aa[i] = p1[i];

		test::ntt(aa);
		printf("%d - %d***\n", aa[0], ntt_p1[0]);
		// for (int i = 0; i < N; i++) {
			// other_ntt_p1[i] = aa[i];
			// printf("%d", aa[i]);
			// aa[i] = (int64_t) aa[i] * -114592 % Q;
			// printf(" - %d\n", aa[i]);
			// aa[i] = inverse_montgomery_reduction<Q>(aa[i]);
		// }
		for (int i = 0; i < N; i++) {
			aa[i] = inverse_montgomery_reduction<Q>(aa[i]);
			if (aa[i] < 0) {
				aa[i] += Q;
			}
			if (ntt_p1[i] != aa[i]) {
				printf("%d vs %d\n", aa[i], ntt_p1[i]);
			}
		}
		return 0;

		// for (int i = 0; i < N; i++)
  //     		aa[i] = (int64_t) aa[i] * -114592 % Q;
  //     	for (int i = 0; i < N; i++) {
  //     		printf("%d %d\n", inverse_montgomery_reduction<Q>(aa[i]), expected_ntt_p1[i]);
  //     		printf("%d %d\n", aa[i], montgomery_reduction<Q>(ntt_p1[i]));
  //     	}

      	// test::invntt_tomont(aa);
      	// for (int i = 0; i < N; i++) {
      	// 	printf("%d %d\n", aa[i], p1[i]);
      	// }

		struct polynomial<N, Q, r> p2 = polynomial<N, 4, r>::generate_random_polynomial();
		int32_t bb[N];
		for (int i = 0; i < N; i++)
			bb[i] = p2[i] - 2;
		for (int i = 0; i < N; i++) {
			p2[i] = (p2[i] - 2 + Q) % Q;
		}
		struct ntt_polynomial<N, Q, r> ntt_p2 = p2.foward_transform();
		test::ntt(bb);

		struct polynomial<N, Q, r> expected_p3 = p1 * p2;
		struct ntt_polynomial<N, Q, r> ntt_p3 = ntt_p1 * ntt_p2;
		struct polynomial<N, Q, r> p3 = ntt_p3.backward_transform();

		int32_t cc[N];
		test::poly_pointwise_montgomery(cc, aa, bb);
		for(unsigned int i = 0; i < N; ++i) {
			cc[i] = test::reduce32(cc[i]);
		}
		test::invntt_tomont(cc);

		// for (int i = 0; i < N; i++)
  //     		cc[i] = (int64_t) cc[i] * -114592 % Q;
  //     	for (int i = 0; i < N; i++)
  //     		cc[i] = inverse_montgomery_reduction<Q>(cc[i]);

		for (int i = 0; i < N; i++) {
			correct = (p3[i] == expected_p3[i]) && (p3[i] == cc[i]);
			if (!correct) {
				printf("int a%d[] = { %d", t, p1[0]);
				for (int j = 1; j < N; j++) {
					printf(", %d", p1[j]);
				}
				printf(" };\n");
				printf("int b%d[] = { %d", t, p2[0]);
				for (int j = 1; j < N; j++) {
					printf(", %d", p2[j]);
				}
				printf(" };\n");
				printf("ERROR: c%d[%d] = %d != %d\n", t, i, p3[i], expected_p3[i]);
				return -1;
			}
		}
	}
	if (correct) {
		printf("OK!\n");
	}
	return 0;
}
