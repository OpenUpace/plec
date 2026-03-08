CXX = clang++
CXXFLAGS = -std=c++17 -I generated/lib -I/usr/include/antlr4-runtime -lantlr4-runtime

TARGET = plec
SRCS_GEN = $(wildcard generated/lib/*.cpp)
SRCS = lib/main.cpp $(SRCS_GEN)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@