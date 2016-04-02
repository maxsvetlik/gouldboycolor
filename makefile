COMP=gcc
CFLAGS= -O0 -Wall
LINK= -lSDL2
INC_DIR=include/
SRC_DIR=src/
BIN_DIR=bin/

all: visual z80

run: z80
	./$(BIN_DIR)$<

z80: $(SRC_DIR)*.c
	$(COMP) $(CFLAGS) -I$(INC_DIR) $^ visual.o -o $(BIN_DIR)$@ `sdl2-config --cflags --libs`

visual: $(SRC_DIR)*.cpp
	g++ $< -I$(INC_DIR) $(LINK) -c

clean:
	rm -i $(BIN_DIR)*
	rm visual.o
