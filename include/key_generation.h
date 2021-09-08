#ifndef __key_generation_
#define __key_generation_

#include <polynomial.h>
#include <dilithium.h>

struct secret_key {
	pmatrix_st A;
	pvector_st t;
};

typedef struct secret_key secret_key_st;

struct public_key {
	pmatrix_st A;
	pvector_st t;
	pvector_st s1;
	pvector_st s2;
};

typedef struct public_key public_key_st;

struct key_pair {
	public_key_st * public_key;
	secret_key_st * secret_key;
};

typedef struct key_pair key_pair_st;

key_pair_st * generate_key_pair(const configuration_st &);

polynomial_st generate_random_polynomial(const configuration_st &);

#endif