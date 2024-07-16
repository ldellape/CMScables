PREFIX = .
LDFLAGS = $(shell root-config --libs)
SRCDIR = src
OBJDIR = build
DEPDIR = .deps
WORKDIR = $(PWD) 
TARGET = $(PREFIX)/CMScables

CXX = g++
CXXFLAGS = -Wall -I include -I src $(shell root-config --cflags) -DPREFIX='"$(PREFIX)"' -DWORKDIR='"$(PWD)"'



SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
DEPS = $(patsubst $(SRCDIR)/%.cpp,$(DEPDIR)/%.d,$(SOURCES))


OPTION ?= NONE
TIME ?= NONE

ifeq ($(OPTION), AUTO_TEST)
	CXXFLAGS+=-DAUTO_TEST
else
	CXXFLAGS+=-DINTER_TEST
endif

ifeq ($(TIME), YES)
	CXXFLAGS+=-DTIME_RES
endif


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
	rm -r build && rm -r .deps

.PHONY: all clean 

