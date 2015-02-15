CXX = g++
CPPFLAGS=-I../Tool/Include
TARGET = ../ConvertEnv/bin/CharacterDataConverter
CXXFLAGS = -Wall -O2 -pipe
SRCS = Main.cpp ../Tool/Source/Tool.cpp ../Tool/Source/ToolInvoker.cpp ../Tool/Source/ArgumentParser.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)


