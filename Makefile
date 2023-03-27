# Flags
CXX = g++
CCFLAGS = -g -std=c++17 -Wall -Werror
LDLIBS = -lm
OBJFILES = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
TARGET = proj

all: $(TARGET)

# Compile files
$(TARGET): $(OBJFILES)
	$(CXX) -o $(TARGET) $(OBJFILES) $(CCFLAGS) $(LDLIBS)
	rm -f *.o

clean:
	rm -f *.o $(TARGET)