CXX = g++
TARGET = CharacterDataConverter.bin
CXXFLAGS = -Wall -O2 -pipe
SRCS = Main.cpp tools/Tool.cpp tools/ToolInvoker.cpp tools/ArgumentParser.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)


