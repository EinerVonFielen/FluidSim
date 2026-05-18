CXX := /usr/bin/g++

CXX_FLAGS := \
	-std=c++17 \
	-Wextra \
	-Wpedantic \
	-g \
	-O3 \
	-march=native \
	-ffast-math

LDLIBS := \
	-ltbb \
	-pthread

RAYLIB_LDFLAGS := $(shell pkg-config --libs raylib)
RAYLIB_CFLAGS  := $(shell pkg-config --cflags raylib)

SRC := sources
INC := includes

all: clean fluidsim

fluidsim: main.cpp $(SRC)/*.cpp $(RUST_LIB)
	$(CXX) \
	$(CXX_FLAGS) \
	$(RAYLIB_CFLAGS) \
	$^ \
	-o $@ \
	-I$(INC) \
	$(RAYLIB_LDFLAGS) \
	$(LDLIBS)

run: all
	./fluidsim

clean:
	rm -f fluidsim

clean-all: clean
	cargo clean
