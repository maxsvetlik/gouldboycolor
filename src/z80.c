/* This program emulates a rough version of the Zilog Z80
 * for particular use in the emulation of a gameboy color.
 * 
 * Generally, this CPU has the 8 8-bit general purpose registers,
 * and 2 16-bit registers (SP and PC)
 *
 * See DOCS for a comprehensive list of the differences
 *
 *
 *
 * Author: Maxwell J Svetlik
 *
 */

#include<stdio.h>

#include "z80.h"

#define DEBUG 1
#define DB_OPCODE 1
unsigned char mem[MEM_SIZE];
char reg[GP_REGS];
unsigned short int pc;
unsigned short int sp;
//add flags


void cycle(){
    unsigned short int inst = mem[pc];
    pc += 1;

    char op = inst;
    
    if(DEBUG && DB_OPCODE){
        printf("Instruction: %x\n",inst);
        printf("Op code: %x\n", op);
    }
    //the "big switch"
    switch(op) {
        case(0x00): break;                                      //noop. 4 cycles
        case(0x01): reset(); break;                             //HALT. 4 cycles
        /* 8-bit loads*/
        /* LD nn,n
         * 8 cycles, unless specified */
        case(0x06): reg[B] = getData(); break;                  //B<-data
        case(0x0E): reg[C] = getData(); break;
        case(0x16): reg[D] = getData(); break;
        case(0x1E): reg[E] = getData(); break;
        case(0x26): reg[H] = getData(); break;
        case(0x2E): reg[L] = getData(); break;
        /* LD r1,r2 r1<-r2
         * 4 cycles unless specified*/
        case(0x7F): break;                                      // A = A
        case(0x78): reg[A] = reg[B]; break;
        case(0x79): reg[A] = reg[C]; break;
        case(0x7A): reg[A] = reg[D]; break;
        case(0x7B): reg[A] = reg[E]; break;
        case(0x7C): reg[A] = reg[H]; break;
        case(0x7D): reg[A] = reg[L]; break;
        case(0x7E): 
            reg[A] = mem[makeaddress(reg[H], reg[L])]; break;   //8 cycles
    
        case(0x40): break;                                      // B = B
        case(0x41): reg[B] = reg[C]; break;
        case(0x42): reg[B] = reg[D]; break;
        case(0x43): reg[B] = reg[E]; break;
        case(0x44): reg[B] = reg[H]; break;
        case(0x45): reg[B] = reg[L]; break;
        case(0x46): 
            reg[B] = mem[makeaddress(reg[H], reg[L])]; break;   //8 cycles
        
        case(0x48): reg[C] = reg[B]; break;
        case(0x49): break;                                      // C = C
        case(0x4A): reg[C] = reg[D]; break;
        case(0x4B): reg[C] = reg[E]; break;
        case(0x4C): reg[C] = reg[H]; break;
        case(0x4D): reg[C] = reg[L]; break;
        case(0x4E): 
            reg[C] = mem[makeaddress(reg[H], reg[L])]; break;   //8 cycles
    
        case(0x50): reg[D] = reg[B]; break;
        case(0x51): reg[D] = reg[C]; break;
        case(0x52): break;                                      // D = D
        case(0x53): reg[D] = reg[E]; break;
        case(0x54): reg[D] = reg[H]; break;
        case(0x55): reg[D] = reg[L]; break;
        case(0x56): 
            reg[D] = mem[makeaddress(reg[H], reg[L])]; break;   //8 cycles
    
        case(0x58): reg[E] = reg[B]; break;
        case(0x59): reg[E] = reg[C]; break;
        case(0x5A): reg[E] = reg[D]; break;
        case(0x5B): break;                                      // E = E
        case(0x5C): reg[E] = reg[H]; break;
        case(0x5D): reg[E] = reg[L]; break;
        case(0x5E): 
            reg[E] = mem[makeaddress(reg[H], reg[L])]; break;   //8 cycles
    
        
        case(0x60): reg[H] = reg[B]; break;
        case(0x61): reg[H] = reg[C]; break;
        case(0x62): reg[H] = reg[D]; break;
        case(0x63): reg[H] = reg[E]; break;
        case(0x64): break;                                      // H = H
        case(0x65): reg[H] = reg[L]; break;
        case(0x66): 
            reg[H] = mem[makeaddress(reg[H], reg[L])]; break;   //8 cycles
    
        case(0x68): reg[L] = reg[B]; break;
        case(0x69): reg[L] = reg[C]; break;
        case(0x6A): reg[L] = reg[D]; break;
        case(0x6B): reg[L] = reg[E]; break;
        case(0x6C): reg[L] = reg[H]; break;
        case(0x6D): break;                                      // L = L
        case(0x6E): 
            reg[L] = mem[makeaddress(reg[H], reg[L])]; break;   //8 cycles
        
        case(0x70): 
            mem[makeaddress(reg[H], reg[L])] = reg[B]; break;   //8 cycles
        case(0x71): 
            mem[makeaddress(reg[H], reg[L])] = reg[C]; break;   //8 cycles
        case(0x72): 
            mem[makeaddress(reg[H], reg[L])] = reg[D]; break;   //8 cycles
        case(0x73): 
            mem[makeaddress(reg[H], reg[L])] = reg[E]; break;   //8 cycles   
        case(0x74): 
            mem[makeaddress(reg[H], reg[L])] = reg[H]; break;   //8 cycles
        case(0x75): 
            mem[makeaddress(reg[H], reg[L])] = reg[L]; break;   //8 cycles
        case(0x36):
            mem[makeaddress(reg[H], reg[L])] = getData();       //12 cycles
            break;
    }
}

/* Gets the next byte of data in memory */
char getData(){
    char res = mem[pc];
    pc += 1;
    return res;
}

/* Creates a 16-bit address from given high and low bit registers */
unsigned short int makeaddress(char high, char low){
    unsigned short int result = high;
    result = result << 8;
    result += low;
    return result;
}

//power on or reset sequence.
//initializes the state as the gameboy expects
void reset(){

    pc = 0x100; //beginning of the bootup rom sequence
    sp = 0xFFFE;
}

int main(int argc, char** argv){
    reset();
    mem[0x100] = 0x12;  
    cycle();
    return 0;
}
