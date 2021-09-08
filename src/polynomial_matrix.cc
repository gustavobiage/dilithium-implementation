#include <stdlib.h>
#include <polynomial.h>

polynomial_matrix::polynomial_matrix() {
    matrix = 0;
    lines = 0;
}

polynomial_matrix::polynomial_matrix(int m, int n) {
    matrix = new pvector_st[m];
    lines = m;
    for (int i = 0; i < m; i++) {
        matrix[i].init(n);
    }
}

polynomial_matrix::~polynomial_matrix() {
    if (matrix) {
        delete matrix;
    }
}

pvector_st& polynomial_matrix::operator[](int index) {
    return matrix[index];
}

pmatrix_st& polynomial_matrix::operator+(const pmatrix_st& _matrix) {
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < matrix[i].columns; j++) {
            matrix[i][j] += _matrix[i][j];
        }
    }
    return *this;
}

pmatrix_st& polynomial_matrix::operator*(const pmatrix_st& _m) {
    pmatrix * res = new pmatrix(lines(), _m.columns());
    for (int i = 0; i < res.lines(); i++) {
        for (int j = 0; j < res.columns(); j++) {
            (*res)[i][j] = 0;
            for (int k = 0; k < _m.lines(); k++) {
                (*res)[i][j] += matrix[i][j] * _m[k][j]; 
            }
        }
    }
    return res;
}
