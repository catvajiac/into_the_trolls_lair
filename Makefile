CC=gcc
CFLAGS=-Wall -std=gnu99
LD=gcc
LDFLAGS=-L.
TARGET=trolls_layer

all: $(TARGET)

list.o: list.c list.h
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

utilities.o: utilities.c $(TARGET).h
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

handlers.o: handlers.c $(TARGET).h
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET).o: $(TARGET).c $(TARGET).h
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(TARGET).o utilities.o handlers.o list.o
	@echo "Linking   $@..."
	@$(LD) $(LDFLAGS) -o $@ $^

clean:
	@echo "Cleaning up..."
	@rm *.o $(TARGET)
