#define GP_REGS 8
#define MEM_SIZE 4096

/*
 * Registers:
 * 8-bit standard. Indexing in this fashion due to
 * instructions that use combinations to form a 
 * 16-bit value. Imagine the registers in this form
 *
 * +-------------+
 * |  A  |   F   | -> (AF)
 * |  B  |   C   | -> (BC)
 * |  D  |   E   | -> (DE)
 * |  H  |   L   | -> (HL)
 * +-------------+
 */
#define A 0
#define F 1
#define B 2
#define C 3
#define D 4
#define E 5
#define H 6
#define L 7



void cycle();
unsigned char getData();
unsigned short int makeaddress(char high, char low);
void reset();
