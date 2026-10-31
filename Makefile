CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -Itests

# Automatically list all .cpp files in src/ and tests/
ALL_SRC = $(wildcard src/*.cpp)
TEST_SRC = $(wildcard tests/*.cpp)

# Filter out src/main.cpp so the test executable uses doctest's main instead
SRC_NO_MAIN = $(filter-out src/main.cpp, $(ALL_SRC))

TEST_TARGET = unit_tests

# Default target
all: $(TEST_TARGET)

# Build the test binary dynamically
$(TEST_TARGET): $(SRC_NO_MAIN) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $(TEST_TARGET)

clean:
	rm -f $(TEST_TARGET) report.xmls