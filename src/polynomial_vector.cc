#include <stdlib.h>
#include <polynomial.h>

polynomial_vector::polynomial_vector() {
    vector = 0;
    columns = 0;
}

polynomial_vector::polynomial_vector(int32_t n) {
    vector = new polynomial_st[n];
    columns = n;
}

polynomial_vector::~polynomial_vector() {
    if (vector) {
        delete vector;
    }
}

polynomial_st& polynomial_vector::operator[](int index) {
    return vector[index];
}

// Referencing returns matrix
struct polynomial_matrix& operator&() {
    pmatrix _m;
    _m.matrix = this;
    _m.lines = 1;
    return _m;
}

void polynomial_vector::init(int32_t n) {
    if (vector) {
        delete vector;
    }
    vector = new polynomial_st[n];
    columns = n;
}
