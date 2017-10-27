SOURCE_EXT=cpp
HEAD_EXT=h
CC=g++
CFLAGS=-c
LDFLAGS=

EXECUTABLE=joytestprog

SOURCES=$(wildcard *.$(SOURCE_EXT))
OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: clean cleanall run test debug

all: $(SOURCES) $(EXECUTABLE)

debug: CFLAGS += -DDEBUG -g -Wall
debug: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $< -o $@


cleanall: clean
	rm -f $(EXECUTABLE)

proper: clean
	rm -f $(EXECUTABLE)

re: proper all

redo: proper debug
	
clean:
	rm -f *.o *.gch

run: 
	./$(EXECUTABLE)

test:
	gdb $(EXECUTABLE)
