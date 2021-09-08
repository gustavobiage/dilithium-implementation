#include <dilithium.h>
#include <key_generation.h>

int main(int argc, const char ** argv) {
	configuration_st conf;
	key_pair_st * key_pair = generate_key_pair(conf);
}