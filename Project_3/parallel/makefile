CXX = g++
CXXFLAGS = -std=c++11
LIBS = -lsndfile -pthread

TARGET = VoiceFilters.out
SRCS = main.cpp
OBJS = main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

main.o: $(SRCS)
	$(CXX) $(CXXFLAGS) -c $(SRCS)

run: $(TARGET)
	./$(TARGET) ../input.wav

clean:
	rm -f $(TARGET) $(OBJS)
