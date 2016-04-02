#ifndef Z80_H_INCLUDED
#define Z80_H_INCLUDED

#define GP_REGS 8
#define MEM_SIZE 65535

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

#define ADD 1
#define SUB 2

extern unsigned char       mem[];
extern char                reg[];
extern unsigned short int  pc;
extern unsigned short int  sp;
extern unsigned char       f_z;
extern unsigned char       f_s;
extern unsigned char       f_hc;
extern unsigned char       f_c;

int cycle();
void setflags_carry(char op, unsigned char val1, unsigned char val2);
void setflags_carry16(char op, unsigned short int val1, unsigned short int val2);
void set_hc_3b(char op, unsigned char val1, unsigned char val2);
void set_hc_nb_4b(char op, unsigned char val1, unsigned char val2);
unsigned char getData();
unsigned short int makeaddress(unsigned char high, unsigned char low);
void halt();
void stop();
void reset();
void clear_mem();
void cpu_init();
unsigned char get_bit(unsigned char reg, unsigned char bit);
unsigned char set_bit(unsigned char reg, unsigned char bit);
unsigned char reset_bit(unsigned char reg, unsigned char bit);

#endif
