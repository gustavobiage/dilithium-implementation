INCLUDE_DIR = include
SRC_DIR = src
TESTS_DIR = tests
OUTPUT_DIR = out

GCC = g++
CRYPTOPP_LIB_DIR = /usr/lib/cryptopp
CRYPTOPP_LIB_FLAGS = -lcryptopp

MAIN = $(SRC_DIR)/main.cc
TEST_FILES := $(shell find $(TESTS_DIR) -name 'test_*.cc')
SUFIX = .cc

build_output_dir := $(shell mkdir -p $(OUTPUT_DIR))

all:
	make compile
	make compile_tests

compile: $(MAIN)
	$(GCC) $^ -I$(INCLUDE_DIR) -I$(SRC_DIR) -lcryptopp -fpermissive -o $(OUTPUT_DIR)/dilithium

compile_tests: $(TEST_FILES)
	$(foreach test, $(TEST_FILES), \
	$(GCC) $(test) -I$(INCLUDE_DIR) -I$(SRC_DIR) -I$(TESTS_DIR) -lcryptopp -fpermissive -o $(OUTPUT_DIR)/$(shell basename $(test) $(SUFIX));)

run_tests: compile_tests
	$(foreach test, $(TEST_FILES), ./$(OUTPUT_DIR)/$(shell basename $(test) $(SUFIX));)

clean:
	rm -f -d -r $(OUTPUT_DIR)
