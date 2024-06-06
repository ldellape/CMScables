CXX = g++
CXXFLAGS = -Wall -I include $(shell root-config --cflags)

INCLUDEDIR = ./include
SRCDIR = ./src
OBJ_DIR = ./build
TARGET = $(OBJ_DIR)/execute_cables

# List of source files including def_variables.cpp in the src directory
SRCS = main.cpp $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

ROOTGLIBS = $(shell root-config --glibs)
ROOTLIBS = $(shell root-config --libs)

# default
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(ROOTLIBS) $(ROOTGLIBS)

$(OBJ_DIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ_DIR)/*.o

.PHONY: all clean
