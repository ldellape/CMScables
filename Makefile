CXX = g++
CXXFLAGS = -Wall -I include -I src $(shell root-config --cflags)
LDFLAGS = $(shell root-config --libs)

SRCDIR = src
OBJDIR = build

TARGET = CMScables

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Default rule
all: $(TARGET)

# Rule to link the objects and create the executable
$(TARGET): $(OBJECTS) $(OBJDIR)/CMScables.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile CMScables.cpp
$(OBJDIR)/CMScables.o: CMScables.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile each source file
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(TARGET) $(OBJDIR)

.PHONY: all clean
