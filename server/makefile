TARGET = ledserver

CC = g++

CFLAGS = -c -Wall -std=c++11
LDFLAGS = -pthread
SOURCES = src/main.cpp src/Led.cpp src/Server.cpp src/Processor.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf src/*.o $(TARGET)
