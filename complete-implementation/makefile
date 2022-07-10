INCLUDE_DIR = include
SRC_DIR = src
TESTS_DIR = tests
OUTPUT_DIR = out
TMP_DIR = /tmp
DEV_DIR = /dev
PQ_CRYSTAL_DIR = pq-crystal
PQ_CRYSTAL_DILITHIUM_DIR = $(PQ_CRYSTAL_DIR)/dilithium

TMP_DILITHIUM_DIR = $(TMP_DIR)/dilithium
INCL_DILITHIUM = $(TMP_DILITHIUM_DIR)

TMP_URANDOM = $(TMP_DIR)/urandom
DEV_URANDOM = $(DEV_DIR)/urandom

# Mode should be either 2 or 5
DILITHIUM_MODE = 2

GCC = g++
FLAGS = -I$(INCLUDE_DIR) -I$(SRC_DIR) -lcryptopp -fpermissive
PQ_CRYSTAL_FLAGS = -L$(INCL_DILITHIUM)/ref -Wl,-rpath=$(INCL_DILITHIUM)/ref -lpqcrystals_dilithium$(DILITHIUM_MODE)_ref -lpqcrystals_fips202_ref -lpqcrystals_aes256ctr_ref -lcrypto
TEST_FLAGS = $(FLAGS) -I$(TESTS_DIR) -DDILITHIUM_MODE=$(DILITHIUM_MODE) -Dpq_crystal_dilithium=$(INCL_DILITHIUM) $(PQ_CRYSTAL_FLAGS) --verbose

MAIN = $(SRC_DIR)/main.cc
TEST_FILES := $(shell find $(TESTS_DIR) -name 'test_*.cc')
SUFIX = .cc

build_output_dir := $(shell mkdir -p $(OUTPUT_DIR))
current_dir := $(shell pwd)

all:
	make compile
	make compile_tests

compile: $(MAIN)
	$(GCC) $^ $(FLAGS) -o $(OUTPUT_DIR)/dilithium

compile_tests: setup_pq_crystal $(TEST_FILES)
	$(foreach test, $(TEST_FILES), \
	$(GCC) $(test) $(TEST_FLAGS) -o $(OUTPUT_DIR)/$(shell basename $(test) $(SUFIX));)

setup_pq_crystal:
ifeq (,$(wildcard $(TMP_DILITHIUM_DIR)))
	@echo "copying pq-crystal"
	cp -r $(PQ_CRYSTAL_DILITHIUM_DIR) $(TMP_DIR)
	grep -rl '$(DEV_URANDOM)' $(TMP_DILITHIUM_DIR) | xargs sed -i "s%$(DEV_URANDOM)%$(TMP_URANDOM)%g"
endif
	(cd $(TMP_DILITHIUM_DIR)/ref && export CC=$(GCC) && make shared)

run_tests: compile_tests
	$(foreach test, $(TEST_FILES), ./$(OUTPUT_DIR)/$(shell basename $(test) $(SUFIX));)

clean:
	rm -f -d -r $(OUTPUT_DIR)
	rm -f -d -r $(TMP_DILITHIUM_DIR)
