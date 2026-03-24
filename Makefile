CXX = g++
GTEST_PREFIX = /opt/homebrew/opt/googletest
GTEST_INC = $(GTEST_PREFIX)/include
GTEST_LIB = $(GTEST_PREFIX)/lib
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I$(GTEST_INC)

TARGET = marketcalc_tests
SRC = marketcalc_tests.cc marketcalc.cc

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) -L$(GTEST_LIB) -lgtest -lgtest_main -pthread

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
