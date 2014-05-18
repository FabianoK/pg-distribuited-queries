CC=g++
CFLAGS=-c -Wall
LIB=-lpqxx -lpq -lpthread -lrt -g 
INC=-I/usr/include/postgresql
SOURCES=src/config.cpp src/main.cpp src/dbfunctions.cpp src/utils.cpp src/tests.cpp src/connection.cpp

EXECUTABLE=sharding
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(INC) $(OBJECTS) -o $@ $(LIB) 
	
.cpp.o:
	$(CC) $(INC) $(LIB) $(CFLAGS) $< -o $@

clean:
	rm -f src/*.o $(EXECUTABLE)
