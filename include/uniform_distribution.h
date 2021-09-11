#ifndef __uniform_distribution_
#define __uniform_distribution_

#include <random>

template <unsigned int Q> struct uniform_distribution;

template <unsigned int Q>
struct uniform_distribution {
    uniform_distribution(): distrib(0, Q) {}

    int generate();

    const static std::random_device rd;
    const static std::mt19937 generator; //Standard mersenne_twister_engine seeded with rd()
    const std::uniform_int_distribution<> distrib;
};

#include <uniform_distribution.cc>
#endif