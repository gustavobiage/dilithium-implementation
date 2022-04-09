INCLUDE_DIR = include
SRC_DIR = src
TESTS_DIR = tests
OUTPUT_DIR = out

GCC = g++
CRYPTOPP_LIB_DIR = /usr/lib/cryptopp
CRYPTOPP_LIB_FLAGS = -lcryptopp

MAIN = $(SRC_DIR)/main.cc

build_output_dir := $(shell mkdir -p $(OUTPUT_DIR))

all:
	make compile

compile: $(MAIN)
	$(GCC) $^ -I$(INCLUDE_DIR) -I$(SRC_DIR) -lcryptopp -fpermissive -o $(OUTPUT_DIR)/ntt_example

clean:
	rm -f -d -r $(OUTPUT_DIR)
