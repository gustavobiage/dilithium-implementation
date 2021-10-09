INCLUDE_DIR = include
SRC_DIR = src
MAIN = $(SRC_DIR)/main.cc
CRYPTOPP_LIB_DIR = /usr/lib/cryptopp
CRYPTOPP_LIB_FLAGS = -lcryptopp
GCC = g++

all:
	make compile

compile: $(MAIN)
	#$(GCC) $^ -I$(INCLUDE_DIR) -I$(SRC_DIR) -L$(CRYPTOPP_LIB_DIR) $(CRYPTOPP_LIB_FLAGS) -o dilithium
	$(GCC) $^ -I$(INCLUDE_DIR) -I$(SRC_DIR) -lcryptopp -o dilithium

