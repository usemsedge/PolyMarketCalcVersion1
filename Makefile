CXX = g++
GTEST_PREFIX = /opt/homebrew/opt/googletest
GTEST_INC = $(GTEST_PREFIX)/include
GTEST_LIB = $(GTEST_PREFIX)/lib
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g -I$(GTEST_INC) -I. -Itests

TARGET = marketcalc_tests
TEST_SRC = $(wildcard tests/*_tests.cc)
SRC = $(TEST_SRC) tests/test_main.cc marketcalc.cc

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) -L$(GTEST_LIB) -lgtest -lgtest_main -pthread

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
