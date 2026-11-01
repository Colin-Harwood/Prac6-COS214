CXX = g++
# Evaluates whether vmime is already installed via pkg-config
HAS_VMIME := $(shell pkg-config --exists vmime 2>/dev/null && echo 1 || echo 0)

CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -Itests -g $(shell pkg-config --cflags vmime 2>/dev/null)
LDFLAGS  = $(shell pkg-config --libs vmime 2>/dev/null)

ALL_SRC     = $(wildcard *.cpp)
TEST_SRC    = $(wildcard tests/*.cpp)
SRC_NO_MAIN = $(filter-out main.cpp, $(ALL_SRC))
TEST_TARGET = unit_tests

all: check-deps $(TEST_TARGET)

# Automatically installs pkg-config and libvmime-dev ONLY if missing on the machine
check-deps:
ifeq ($(HAS_VMIME), 0)
	@echo "[Setup] Missing VMime or pkg-config. Installing once..."
	sudo apt-get update && sudo apt-get install -y pkg-config libvmime-dev
endif

$(TEST_TARGET): $(SRC_NO_MAIN) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $(TEST_TARGET) $(LDFLAGS)

clean:
	rm -f $(TEST_TARGET) report.xml