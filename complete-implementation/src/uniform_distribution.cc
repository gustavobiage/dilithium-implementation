/* Included on header file */

template <unsigned int Q>
int tcc::uniform_distribution<Q>::generate() {
    return distrib(generator);
}
