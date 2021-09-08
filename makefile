INCLUDE_DIR = include
SRTC_DIR = src
OBJS = $(shell find $(SRC) -iname "*.cc")

GCC = g++

all:
	make compile

compile: $(OBJS)
	$(GCC) $^ -I$(INCLUDE_DIR) -o dilithium

