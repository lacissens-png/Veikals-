# Standalone build of the engine-agnostic EclipseCore rules + unit tests.
# Usage:  make test   (from this directory)

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror -fno-exceptions
CORE     := ../Source/EclipseCore
SRCS     := CoreTests.cpp $(wildcard $(CORE)/Private/*.cpp)
SRCS     := $(filter-out $(CORE)/Private/EclipseCoreModule.cpp,$(SRCS))
BIN      := eclipse_core_tests

.PHONY: all test clean

all: $(BIN)

$(BIN): $(SRCS) $(wildcard $(CORE)/Public/EclipseCore/*.h)
	$(CXX) $(CXXFLAGS) -I$(CORE)/Public $(SRCS) -o $@

test: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)
