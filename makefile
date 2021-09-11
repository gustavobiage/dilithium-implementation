INCLUDE_DIR = include
SRC_DIR = src
MAIN = $(SRC_DIR)/main.cc

GCC = g++

all:
	make compile

compile: $(MAIN)
	$(GCC) $^ -I$(INCLUDE_DIR) -I$(SRC_DIR) -o dilithium

