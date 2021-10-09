#ifndef __uniform_distribution_
#define __uniform_distribution_

#include <random>

template <unsigned int Q> struct uniform_distribution;

template <unsigned int Q>
struct uniform_distribution {
public:

    uniform_distribution() : generator(rd()), distrib(0, Q-1) {}

    int generate();

public:
    std::random_device rd;
    std::mt19937 generator;
    std::uniform_int_distribution<int> distrib;
};

#include <uniform_distribution.cc>
#endif