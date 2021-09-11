#ifdef __uniform_distribution_
#define __uniform_distribution_

#include <random>

template <unsigned int Q> struct uniform_distribution;

template <unsigned int Q>
struct uniform_distribution {
    int gen();

    static std::random_device rd;
    static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, Q);
};

#endif