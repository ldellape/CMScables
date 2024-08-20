PREFIX = .
LDFLAGS = $(shell root-config --libs)
SRCDIR = src
OBJDIR = build
DEPDIR = .deps
WORKDIR = $(PWD) 
TARGET = $(PREFIX)/CMScables
TARGET_STAT = $(PREFIX)/statistics

CXX = g++
CXXFLAGS = -Wall -I include -I src $(shell root-config --cflags) -DPREFIX='"$(PREFIX)"' -DWORKDIR='"$(PWD)"'
CXXFLAGS_STAT = -Wall $(shell root-config --cflags) 

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

$(TARGET_STAT): $(OBJDIR)/statistics.o 
	$(CXX) $(CXXFLAGS_STAT) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/statistics.o : statistics.cpp 
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS_STAT) -c $< -o $@

ifneq ($(MAKECMDGOALS),statistics)
-include $(DEPS)
endif

$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	@mkdir -p $(DEPDIR)
	$(CXX) $(CXXFLAGS) -M $< -MF $@ -MT $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$<)

clean: 
	rm -rf $(TARGET) $(TARGET_STAT) .deps $(OBJDIR)

.PHONY: all clean 

