CC=gcc
CFLAGS=-c -I. --std=c11 -ggdb
LDFLAGS=-I.
EXECUTABLE=server.exe
EXECUTABLE2=client.exe

SOURCES=server.c tcpipnix.c tcpipwin.c
OBJECTS=$(SOURCES:.c=.o)

SOURCE2=client.c tcpipnix.c tcpipwin.c
OBJECT2=$(SOURCE2:.c=.o)

.PHONY: clean cleanall
all: $(EXECUTABLE) $(EXECUTABLE2)

$(EXECUTABLE2): $(OBJECT2)
	$(CC)  $(OBJECT2) $(LDFLAGS) -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $< -o $@

cleanall: clean
	rm -f $(EXECUTABLE) $(EXECUTABLE2)

clean:
	rm -f *.o *.gch

run:
	./$(EXECUTABLE)
