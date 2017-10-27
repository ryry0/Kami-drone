CC=g++
CFLAGS=-c
LDFLAGS=
EXECUTABLE=server
EXECUTABLE2=client

SOURCES=server.cpp tcpipnix.cpp
OBJECTS=$(SOURCES:.cpp=.o)

SOURCE2=client.cpp tcpipnix.cpp
OBJECT2=$(SOURCE2:.cpp=.o)

.PHONY: clean cleanall
all: $(EXECUTABLE) $(EXECUTABLE2) 

$(EXECUTABLE2): $(OBJECT2)
	$(CC) $(LDFLAGS) $(OBJECT2) -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $< -o $@

cleanall: clean 
	rm -f $(EXECUTABLE) $(EXECUTABLE2)

clean:
	rm -f *.o *.gch

run: 
	./$(EXECUTABLE)
