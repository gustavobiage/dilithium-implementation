// std::random_device rd;
// std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
// std::uniform_int_distribution<int> distrib(0, 256);
// char p[32];
// bool has_generated_seed = false;

// template<unsigned int Q>
// struct ntt_polynomial<Q> generate_random_polynomial() {
//     if (!has_generated_seed) {
//         for (int i = 0; i < 32; i++) {
//             p[i] = distrib(gen);
//         }
//         has_generated_seed = true;
//     }

    
// }