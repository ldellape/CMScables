CXX = g++
CXXFLAGS = -Wall -I include -I src $(shell root-config --cflags)
LDFLAGS = $(shell root-config --libs)

SRCDIR = src
OBJDIR = build_CMScables
DEPDIR = .deps

TARGET = CMScables

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
DEPS = $(patsubst $(SRCDIR)/%.cpp,$(DEPDIR)/%.d,$(SOURCES))


all: $(TARGET)

$(TARGET): $(OBJECTS) $(OBJDIR)/CMScables.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/CMScables.o: CMScables.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPDIR)/%.d
	@mkdir -p $(OBJDIR) $(DEPDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	@mkdir -p $(DEPDIR)
	$(CXX) $(CXXFLAGS) -M $< -MF $@ -MT $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$<)

clean:
	rm -rf $(TARGET) $(OBJDIR) $(DEPDIR)

.PHONY: all clean
