CC=gcc
CFLAGS=-Wall -std=gnu99
LD=gcc
LDFLAGS=-L.
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=trolls_lair

all: $(TARGET)

$(OBJECTS): %.o: %.c 
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS)
	@echo "Linking	  $@..."
	@$(LD) $(LDFLAGS) -o $@ $^

clean:
	@echo "Cleaning up..."
	@rm *.o $(TARGET)
