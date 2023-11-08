# all:pi.c
# 	g++ pi.c -o pi.out
# clean:
# 	rm -f pi.out
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O3
LDFLAGS = -pthread

# Output executable name
TARGET = pi.out

# Source file
SOURCE = pi.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f $(TARGET)