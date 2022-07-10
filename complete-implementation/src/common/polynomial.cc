#include <queue>
#include <utility>

int32_t tcc::fast_power(int32_t a, int32_t b, int32_t m) {
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

int32_t tcc::extended_euclidean(int64_t a, int64_t b, int64_t * x, int64_t * y) {
	int64_t r, r_;
	int64_t s, s_;
	int64_t t, t_;
	r = b; r_ = a;
	s = 0; s_ = 1;
	t = 1; t_ = 0;
	int64_t q, aux;
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

int32_t tcc::find_modular_inverse(int64_t a, int64_t m) {
	int64_t x, y;
	int64_t gcd = tcc::extended_euclidean(a, m, &x, &y);
	if (gcd != 1) {
	    return -1;
	} else {
	    return (x % m + m) % m;
	}
}

template <unsigned int Q>
int32_t tcc::reduce32(int32_t a) {
	int32_t t;
	t = (a + (1 << 22)) >> 23;
	t = a - t*Q;
	return t;
}

template <unsigned int Q>
int32_t tcc::inverse_montgomery_reduction(int64_t a) {
    int32_t r = tcc::find_modular_inverse((int64_t) 1 << 32, Q);
    return (a * r) % Q;
}

int32_t tcc::multiply(int64_t a, int64_t b, int64_t m) {
	return ((a * b) + m) % m;
}

int32_t tcc::subtract(int64_t a, int64_t b, int64_t m) {
	return (a - b + m) % m;
}

int32_t tcc::add(int64_t a, int64_t b, int64_t m) {
	return (a + b + m) % m;
}

int32_t tcc::abs(int32_t a) {
	if (a < 0) {
		return -a;
	}
	return a;
}

template <unsigned int N, unsigned int Q, unsigned int W>
int32_t tcc::zeta_array<N, Q, W>::zetas[N];

template <unsigned int N, unsigned int Q, unsigned int W>
tcc::zeta_constructor<N, Q, W> tcc::zeta_array<N, Q, W>::constructor;

template <unsigned int N, unsigned int Q, unsigned int W>
int32_t tcc::zeta_array<N, Q, W>::operator[](int index) const {
	return zeta_array<N, Q, W>::zetas[index];
}

template <unsigned int N, unsigned int Q, unsigned int W>
tcc::zeta_constructor<N, Q, W>::zeta_constructor() {
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
		int zeta = tcc::fast_power(power[j], p.second, Q);
		zeta_array<N, Q, W>::zetas[i] = zeta;
		if (i + q.size() + 1 < N) {
			q.push(std::make_pair(p.first * 2, p.second));
			// (power + nth/2) % nth
			q.push(std::make_pair(p.first * 2, (p.second + p.first/2) % p.first));
		}
	}
}