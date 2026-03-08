CXX = clang++
CXXFLAGS = -std=c++17 -g -fexceptions \
			-Iinclude -Igenerated/lib -I/usr/include/antlr4-runtime -lantlr4-runtime \
			-lLLVM-21

TARGET = plec
SRCS_IR_LIB = $(wildcard lib/IR/*.cpp)
SRCS_GEN = $(wildcard generated/lib/*.cpp)
SRCS = lib/main.cpp $(SRCS_IR_LIB) $(SRCS_GEN)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@