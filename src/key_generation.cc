#include <key_generation.h>

key_pair_st * generate_key_pair(const configuration_st& conf) {

	pmatrix_st A(conf.k, conf.l);
	for (int i = 0; i < conf.k; i++) {
		for (int j = 0; j < conf.k; j++) {
			A[i][j] = generate_random_polynomial(conf);
		}
	}

	pvector_st s1(conf.l);
	for (int i = 0; i < conf.l; i++) {
		s1[i] = generate_random_polynomial(conf);
	}

	pvector_st s2(conf.k);
	for (int i = 0; i < conf.k; i++) {
		s2[i] = generate_random_polynomial(conf);
	}

	pvector_st t = *((A * &s1) + &s2);

	// Assignment generates new copies
	public_key_st * pk = (public_key_st*) malloc(sizeof(public_key_st));
	pk->A = A;
	pk->t = t;
	pk->s1 = s1;
	pk->s2 = s2;

	secret_key_st * sk = (secret_key_st*) malloc(sizeof(secret_key_st));
	sk->A = A;
	sk->t = t;

	key_pair_st * key_pair = (key_pair_st*) malloc(sizeof(key_pair_st));
	key_pair->public_key = pk;
	key_pair->secret_key = sk;

	return key_pair;
}

polynomial_st generate_random_polynomial(const configuration_st & conf) {

}