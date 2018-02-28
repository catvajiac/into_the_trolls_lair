CC=gcc
CFLAGS=-Wall -std=gnu99 -g
LD=gcc
LDFLAGS=-L.
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
DEPENDS=$(wildcard *.h)
TARGET=trolls_lair

all: $(TARGET)

$(OBJECTS): %.o: %.c  $(DEPENDS)
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS)
	@echo "Linking	  $@..."
	@$(LD) $(LDFLAGS) -o $@ $^

clean:
	@echo "Cleaning up..."
	@rm *.o $(TARGET)
