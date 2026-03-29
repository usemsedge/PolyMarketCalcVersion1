CXX = g++
GTEST_PREFIX = /opt/homebrew/opt/googletest
GTEST_INC = $(GTEST_PREFIX)/include
GTEST_LIB = $(GTEST_PREFIX)/lib
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g -I$(GTEST_INC) -I. -Itests

TARGET = marketcalc_tests
TEST_SRC = $(wildcard tests/*_tests.cc)
SRC = $(TEST_SRC) tests/test_main.cc marketcalc.cc

EMCC ?= emcc
WASM_JS = marketcalc.js
# Sync with extern "C" in marketcalc.cc. Use _main_wasm here only if you define it.
EMFLAGS = -std=c++17 -O2 \
	-s MODULARIZE=1 \
	-s EXPORT_NAME=createModule \
	-s EXPORT_ES6=1 \
	-s EXPORTED_FUNCTIONS='["_malloc","_free","_findBestMarketLayout_wasm"]' \
	-s EXPORTED_RUNTIME_METHODS='["HEAP32"]'

.PHONY: all test run clean wasm

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) -L$(GTEST_LIB) -lgtest -lgtest_main -pthread

test: $(TARGET)
	./$(TARGET)

$(WASM_JS): marketcalc.cc marketcalc.h
	$(EMCC) marketcalc.cc -o $(WASM_JS) $(EMFLAGS)

wasm: $(WASM_JS)

run: $(WASM_JS)
	node server.js

clean:
	rm -f $(TARGET) $(WASM_JS) marketcalc.wasm 
	rm -rf marketcalc_tests.dSYM
