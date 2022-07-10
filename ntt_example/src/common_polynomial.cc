#include <queue>
#include <utility>

// const unsigned int Q_ = 58728449;
const unsigned int Q_ = 8265825;
const unsigned int r_ = 8265825;
const unsigned int r_e2 = 7593442;
// const unsigned int r = ;

template <unsigned int Q>
int32_t inverse_montgomery_reduction(int64_t a) {
    // t = ((int64_t) t * Q_) % ((int64_t) 1 << 32);
    // return (t + Q) % Q;
    // return (t * tcc::Q_) >> 32;
      
      // int32_t t;
      // t = (int64_t)(int32_t)a*Q_;
      // t = (a - (int64_t)t*Q) >> 32;
      // return t;
	// return (a*Q_) % Q;

	int32_t t;

	t = (int64_t)(int32_t)a*58728449;
	t = (a - (int64_t)t*Q) >> 32;
	return ((int64_t)t + Q) % Q;
}

template <unsigned int Q>
int32_t montgomery_reduction(int64_t a) {
      return (a << 32) % Q;
}

template <unsigned int Q>
int32_t multiply_montgomery(int64_t a, int64_t b, int64_t m) {
	// return ((a * b) << 32) % m;
	// return montgomery_reduction<Q>(a*b);
	return inverse_montgomery_reduction<Q>(a*b);
	// return ((a * b) + m) % m;
}

int32_t fast_power(int32_t a, int32_t b, int32_t m) {
    if (b == 0) {
        return 1;
    } else if (b == 1) {
        return a;
    }

    int32_t half = b/2;
    int64_t x = fast_power(a, half, m);
    x = (x * x) % m;
    if (b & 1) {
        x = (x * a) % m;
    }
    return x;
}

int32_t extended_euclidean(int32_t a, int32_t b, int32_t * x, int32_t * y) {
	int32_t r, r_;
	int32_t s, s_;
	int32_t t, t_;
	r = b; r_ = a;
	s = 0; s_ = 1;
	t = 1; t_ = 0;
	int32_t q, aux;
	while (r != 0) {
		q = r_ / r;
		aux = r;
		r = r_ - q * r; r_ = aux;
		aux = s;
		s = s_ - q * s; s_ = aux;
		aux = t;
		t = t_ - q * t; t_ = aux;
	}
	*x = s_;
	*y = t_;
	return r_;
}


int32_t find_modular_inverse(int32_t a, int32_t m) {
	int32_t x, y;
	int32_t gcd = extended_euclidean(a, m, &x, &y);
	if (gcd != 1) {
	    return -1;
	} else {
	    return (x % m + m) % m;
	}
}


int32_t multiply(int64_t a, int64_t b, int64_t m) {
	return ((a * b) + m) % m;
}

int32_t subtract(int64_t a, int64_t b, int64_t m) {
	return (a - b + m) % m;
}

int32_t add(int64_t a, int64_t b, int64_t m) {
	return (a + b + m) % m;
}

template <unsigned int N, unsigned int Q, unsigned int W>
int32_t zeta_array<N, Q, W>::zetas[N];

template <unsigned int N, unsigned int Q, unsigned int W>
zeta_constructor<N, Q, W> zeta_array<N, Q, W>::constructor;

template <unsigned int N, unsigned int Q, unsigned int W>
int32_t zeta_array<N, Q, W>::operator[](int index) const {
	return zeta_array<N, Q, W>::zetas[index];
}

template <unsigned int N, unsigned int Q, unsigned int W>
zeta_constructor<N, Q, W>::zeta_constructor() {
	int power[N*2] = {0};
	for (int i = 1; i < 2*N; i = i*2) {
		power[i] = fast_power(W, i, Q);
	}
	std::queue<std::pair<int, int>> q;
	q.push(std::make_pair(4, 1));
	zeta_array<N, Q, W>::zetas[0] = 0;
	for (int i = 1; i < N; i++) {
		std::pair<int, int> p = q.front();
		q.pop();
		int j = (2*N) / p.first;
		int zeta = fast_power(power[j], p.second, Q);
		zeta_array<N, Q, W>::zetas[i] = zeta;
		if (i + q.size() + 1 < N) {
			q.push(std::make_pair(p.first * 2, p.second));
			// (power + nth/2) % nth
			q.push(std::make_pair(p.first * 2, (p.second + p.first/2) % p.first));
		}
	}
}