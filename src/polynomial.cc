#include <polynomial.h>

polynomial::polynomial() {
    coefficients = 0;
    degree = 0;
    _free = true;
}

polynomial::polynomial(int n) {
    coefficients = new int32_t[n];
    degree = n;
    _free = true;
}

polynomial::~polynomial() {
    if (coefficients && _free) {
        delete coefficients;
    }
}

polynomial_st & polynomial::operator=(const polynomial_st& a) {
    degree = a.degree;
    coefficients = new int32_t[degree];
    for (int i = 0; i < a.degree; i++) {
        coefficients[i] = a.coefficients[i];
    }
    return *this;
}

polynomial_st& polynomial::operator+(const polynomial_st& a) {
    for (int i = 0; i < degree; i++) {
        coefficients[i] += a.coefficients[i];
    }
    return *this;
}

polynomial_st& polynomial::operator*(const polynomial_st& _p) {
    int a, b, c;
    for (int i = 0; i < degree; i++) {
        for (int j = 0; j < degree; j++) {
            c = (i+j) % degree;
            a = j;
            b = (i - j + degree) % degree;
            coefficients[c] = _p.coefficients[a] coefficients[b];
        }
    }
    return *this;
}

polynomial_st& polynomial::operator/(const polynomial_st& a) {
    // TODO
}