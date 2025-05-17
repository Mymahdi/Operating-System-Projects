# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11 -pthread

# Targets
TARGETS = serial parallel


# Source files
SRCS_SERIAL = serial.cpp
SRCS_PARALLEL = parallel.cpp

# Object files
OBJS_SERIAL = $(SRCS_SERIAL:.cpp=.o)
OBJS_PARALLEL = $(SRCS_PARALLEL:.cpp=.o)

all: $(TARGETS)

serial: $(OBJS_SERIAL)
	$(CXX) $(CXXFLAGS) -o $@ $^

parallel: $(OBJS_PARALLEL)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation rule for .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
