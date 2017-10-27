# File extensions
SOURCE_EXT=c
OBJ_EXT=o
HEAD_EXT=h
OBJ_HEAD_EXT=gch

LINEX_EXT=x
WINEX_EXT=exe

T_EXT=cmocka
TOBJ_EXT=ocmocka

# Compilers and flags
WIN64CC=x86_64-w64-mingw32-gcc
WIN32CC=i686-w64-mingw32-gcc
CC=gcc

ALLCFLAGS =-c -I. -std=c11
ALLLDFLAGS =

WINCFLAGS=$(ALLCFLAGS)
WINLDFLAGS=$(ALLLDFLAGS)

LINCFLAGS=$(ALLCFLAGS)
LINLDFLAGS=$(ALLLDFLAGS)

DFLAGS=-DDEBUG -ggdb3 -Wall -fdiagnostics-color=auto
RFLAGS=-O2
PFLAGS=-pg
TCFLAGS=-x c
TLFLAGS=-lcmocka
COVFLAGS=-fprofile-arcs -ftest-coverage -O0
DEFAULT_DEBUG=y

# Files
SOURCES=$(wildcard *.$(SOURCE_EXT))
OBJECTS=$(SOURCES:.$(SOURCE_EXT)=.$(OBJ_EXT))
EXECUTABLE=out.x
WINEXE=$(EXECUTABLE:.$(LINEX_EXT)=.$(WINEX_EXT))

# Test files
TFILE=$(wildcard *.$(T_EXT))
TOBJECT=$(TFILE:.$(T_EXT)=.$(TOBJ_EXT))
TEXECUTABLE=test.x
MAINOBJ=main.o


# Non-file targets
.PHONY: clean cleanall run

ifeq ($(DEFAULT_DEBUG),y)
ALL_TARGET=debug
else
ALL_TARGET=release
endif

all: $(ALL_TARGET)

debug: CFLAGS = $(LINCFLAGS)
debug: CFLAGS += $(DFLAGS)
debug: LDFLAGS = $(LINLDFLAGS)
debug: $(SOURCES) $(EXECUTABLE)

profile: CFLAGS +=$(PFLAGS)
profile: release

coverage: CFLAGS +=$(COVFLAGS)
coverage: $(SOURCES) $(EXECUTABLE)

release: CFLAGS += $(RFLAGS)
release: $(SOURCES) $(EXECUTABLE)

win32: CC=$(WIN32CC)
win32: CFLAGS = $(WINCFLAGS)
win32: LDFLAGS = $(WINLDFLAGS)
win32: $(SOURCES) $(WINEXE)

win64: CC=$(WIN64CC)
win64: CFLAGS = $(WINCFLAGS)
win64: LDFLAGS = $(WINLDFLAGS)
win64: $(SOURCES) $(WINEXE)

$(WINEXE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

%.$(OBJ_EXT): %.$(SOURCE_EXT) $(wildcard *.$(HEAD_EXT))
	$(CC) $(CFLAGS) $< -o $@

build-test: all
	$(CC) $(CFLAGS) $(TCFLAGS) $(TFILE) -o $(TOBJECT) && \
	$(CC) $(TOBJECT) $(filter-out $(MAINOBJ),$(OBJECTS)) $(LDFLAGS) $(TLFLAGS) -o $(TEXECUTABLE)

test: build-test
	./$(TEXECUTABLE)

cleanall: clean
	rm -f $(EXECUTABLE) $(WINEXE)

proper: clean cleanall

re: proper all

redo: proper debug

clean:
	rm -f $(wildcard *.$(OBJ_EXT)) $(wildcard *.$(OBJ_HEAD_EXT))

tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q

run: all
	./$(EXECUTABLE)

gdb: debug
	gdb -q $(EXECUTABLE)

grind: debug
	valgrind ./$(EXECUTABLE)

leak-check: debug
	valgrind --leak-check=full ./$(EXECUTABLE)

analyze-profile: profile run
	gprof $(EXECUTABLE) > profile
