/* Included on header file */

template <unsigned int Q>
int uniform_distribution<Q>::generate() {
    return distrib(generator);
}
