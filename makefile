COMP=gcc
CFLAGS= -O0 -Wall
LINK= -lSDL2
INC_DIR=include/
SRC_DIR=src/
BIN_DIR=bin/

all: z80

run: z80
	./$(BIN_DIR)$<
z80: $(SRC_DIR)*.c
	$(COMP) $(CFLAGS) -I$(INC_DIR) $(LINK) $^ -o $(BIN_DIR)$@

screen: $(SRC_DIR)*.cpp
	gcc $< -I$(INC_DIR) $(LINK) -o $(BIN_DIR)$@

clean:
	rm -i $(BIN_DIR)*
