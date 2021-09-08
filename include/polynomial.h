#ifndef __polynomial_
#define __polynomial_

#include <stdlib.h>
#include <stdint.h>
#include <math_utils.h>

struct polynomial;
struct polynomial_vector;
struct polynomial_matrix;

// TODO: make clear that is a polynomial in polynomial ring q
// template<typename Q>
struct polynomial {
	int32_t coefficients[Q];

	polynomial();

	struct polynomial& operator=(const struct polynomial&);

	struct polynomial& operator+(const struct polynomial&);

	struct polynomial& operator*(const struct polynomial&);

	struct polynomial& operator/(const struct polynomial&);

	~polynomial();
};

typedef struct polynomial polynomial_st;

struct polynomial_vector {
    struct polynomial * vector;
	int32_t columns;
	bool _free;

	polynomial_vector();

	polynomial_vector(int32_t);

	~polynomial_vector();

	struct polynomial& operator[](int);
	// Referencing returns matrix
	struct polynomial_matrix& operator&();

	void init(int32_t);
};

typedef struct polynomial_vector pvector_st;

struct polynomial_matrix {
    struct polynomial_vector * matrix;
	int32_t lines;
	bool _free;

	polynomial_matrix();

	polynomial_matrix(int32_t, int32_t);

	~polynomial_matrix();

	struct polynomial_vector& operator[](int);

	struct polynomial_matrix& operator+(const struct polynomial_matrix&);

	struct polynomial_matrix& operator*(const struct polynomial_matrix&);
	// Dereferencing returns vector
	struct polynomial_vector& operator*();
};

typedef struct polynomial_matrix pmatrix_st;

#endif