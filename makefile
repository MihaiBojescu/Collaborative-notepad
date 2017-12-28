COMPILER = g++
CFLAGS = -Wall -O2
DEBUGFLAGS = -Wall -g

LIBS =

INCLUDEDIR = include/*.h
SRCDIR = src/*.cpp
OUTPUT = build/notepad

all: release

release:
	$(COMPILER) $(CFLAGS) -I$(INCLUDEDIR) $(SRCDIR) $(LIBS) -o $(OUTPUT)

debug:
	$(COMPILER) $(DEBUGFLAGS) -I$(INCLUDEDIR) $(SRCDIR) $(LIBS) -o $(OUTPUT)-debug

clean:
	rm -f build/*
