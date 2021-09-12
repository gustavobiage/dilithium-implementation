#include <key_generation.h>
#include <dilithium_i.h>

int main(int argc, const char ** argv) {
	struct key_pair<K_PAR, L_PAR, N_PAR, Q_PAR> key_pair = generate_key_pair<K_PAR, L_PAR, N_PAR, Q_PAR, ETA_PAR>();
	return 0;
}