COMP=gcc
CFLAGS= -O0 -Wall
INC_DIR=include/
SRC_DIR=src/
BIN_DIR=bin/


all: z80


run: z80
	./$(BIN_DIR)$<
z80: $(SRC_DIR)*.c
	$(COMP) $(CFLAGS) -I$(INC_DIR) $^ -o $(BIN_DIR)$@

clean:
	rm -i $(BIN_DIR)*
