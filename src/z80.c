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
unsigned char f_z;
unsigned char f_s;
unsigned char f_hc;
unsigned char f_c;


void cycle(){
    unsigned short int inst = mem[pc];
    pc += 1;

    unsigned char op = inst;
    
    if(DEBUG && DB_OPCODE){
        printf("Instruction: %x\n",inst);
        printf("Op code: %x\n", op);
    }
    //the "big switch"
    switch(op) {
        case(0x00): break;                                      //noop. 4 cycles
        case(0x10): reset(); break;                             //HALT. 4 cycles
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
            reg[A] = mem[makeaddress(reg[L], reg[H])]; break;   //8 cycles
        case(0xFA): 
            reg[A] = mem[makeaddress(getData(), getData())];    //16 cycles
            break;

        case(0x3E): reg[A] = getData(); break;                  //8 cycles
        case(0x40): break;                                      // B = B
        case(0x41): reg[B] = reg[C]; break;
        case(0x42): reg[B] = reg[D]; break;
        case(0x43): reg[B] = reg[E]; break;
        case(0x44): reg[B] = reg[H]; break;
        case(0x45): reg[B] = reg[L]; break;
        case(0x46): 
            reg[B] = mem[makeaddress(reg[L], reg[H])]; break;   //8 cycles
        
        case(0x48): reg[C] = reg[B]; break;
        case(0x49): break;                                      // C = C
        case(0x4A): reg[C] = reg[D]; break;
        case(0x4B): reg[C] = reg[E]; break;
        case(0x4C): reg[C] = reg[H]; break;
        case(0x4D): reg[C] = reg[L]; break;
        case(0x4E): 
            reg[C] = mem[makeaddress(reg[L], reg[H])]; break;   //8 cycles
    
        case(0x50): reg[D] = reg[B]; break;
        case(0x51): reg[D] = reg[C]; break;
        case(0x52): break;                                      // D = D
        case(0x53): reg[D] = reg[E]; break;
        case(0x54): reg[D] = reg[H]; break;
        case(0x55): reg[D] = reg[L]; break;
        case(0x56): 
            reg[D] = mem[makeaddress(reg[L], reg[H])]; break;   //8 cycles
    
        case(0x58): reg[E] = reg[B]; break;
        case(0x59): reg[E] = reg[C]; break;
        case(0x5A): reg[E] = reg[D]; break;
        case(0x5B): break;                                      // E = E
        case(0x5C): reg[E] = reg[H]; break;
        case(0x5D): reg[E] = reg[L]; break;
        case(0x5E): 
            reg[E] = mem[makeaddress(reg[L], reg[H])]; break;   //8 cycles
    
        
        case(0x60): reg[H] = reg[B]; break;
        case(0x61): reg[H] = reg[C]; break;
        case(0x62): reg[H] = reg[D]; break;
        case(0x63): reg[H] = reg[E]; break;
        case(0x64): break;                                      // H = H
        case(0x65): reg[H] = reg[L]; break;
        case(0x66): 
            reg[H] = mem[makeaddress(reg[L], reg[H])]; break;   //8 cycles
    
        case(0x68): reg[L] = reg[B]; break;
        case(0x69): reg[L] = reg[C]; break;
        case(0x6A): reg[L] = reg[D]; break;
        case(0x6B): reg[L] = reg[E]; break;
        case(0x6C): reg[L] = reg[H]; break;
        case(0x6D): break;                                      // L = L
        case(0x6E): 
            reg[L] = mem[makeaddress(reg[L], reg[H])]; break;   //8 cycles
        
        case(0x70): 
            mem[makeaddress(reg[L], reg[H])] = reg[B]; break;   //8 cycles
        case(0x71): 
            mem[makeaddress(reg[L], reg[H])] = reg[C]; break;   //8 cycles
        case(0x72): 
            mem[makeaddress(reg[L], reg[H])] = reg[D]; break;   //8 cycles
        case(0x73): 
            mem[makeaddress(reg[L], reg[H])] = reg[E]; break;   //8 cycles   
        case(0x74): 
            mem[makeaddress(reg[L], reg[H])] = reg[H]; break;   //8 cycles
        case(0x75): 
            mem[makeaddress(reg[L], reg[H])] = reg[L]; break;   //8 cycles
        case(0x36):
            mem[makeaddress(reg[L], reg[H])] = getData();       //12 cycles
            break;
    
        /* LD n,A
         * 4 cycles, unless specified */
        case(0x47): reg[B] = reg[A]; break;
        case(0x4F): reg[B] = reg[A]; break;
        case(0x57): reg[C] = reg[A]; break;
        case(0x5F): reg[D] = reg[A]; break;
        case(0x67): reg[H] = reg[A]; break;
        case(0x6F): reg[L] = reg[A]; break;
        case(0x02): 
            mem[makeaddress(reg[C], reg[B])] = reg[A]; break;  //8 cycles
        case(0x12): 
            mem[makeaddress(reg[C], reg[B])] = reg[A]; break;  //8 cycles
        case(0x77): 
            mem[makeaddress(reg[C], reg[B])] = reg[A]; break;  //8 cycles
        case(0xEA):
            mem[makeaddress(getData(), getData())] = reg[A];    //16 cycles
            break;
    
        /*LD A,(C) where 0xFF00 is the base*/
        case(0xF2): reg[A] = mem[0xFF00 + reg[C] ]; break;      //8 cycles
        case(0xE2): mem[0xFF00 + reg[C]] = reg[A]; break;       //8 cycles
        
        /*LD A,(HLD) - HL dec*/
        case(0x3A): reg[A] = mem[makeaddress(reg[L], reg[H])];  //8 cycles
                    reg[L] -= 1; break;
        /*LD (HLD),A - HL dec*/
        case(0x32): mem[makeaddress(reg[L], reg[H])] = reg[A];
                    reg[L] -= 1; break;                         //8 cycles
        /*LD A,(HLD) - HL inc */
        case(0x2A): reg[A] = mem[makeaddress(reg[L], reg[H])];
                    reg[L] += 1; break;                         //8 cycles
        /*LD (HLD),A - HL inc */
        case(0x22): mem[makeaddress(reg[L], reg[H])] = reg[A];
                    reg[L] += 1; break;                         //8 cycles
        /*LD (n),A where 0xFF00 is the base */
        case(0xE0): mem[0xFF00 + getData()] = reg[A]; break;    //12 cycles
        /*LD A,(n) where 0xFF00 is the base */
        case(0xF0): reg[A] = mem[0xFF00 + getData()]; break;    //12 cycles 
 
        /*
         * 16-bit Loads
         */
        /*LD n,nn for a 16bit nn
         * 12 cycles*/
        case(0x01): reg[B] = getData(); reg[C] = getData(); break;
        case(0x11): reg[D] = getData(); reg[E] = getData(); break;
        case(0x21): reg[H] = getData(); reg[L] = getData(); break;
        case(0x31): sp = makeaddress(getData(), getData()); break;
        /*LD HL,sp - 8cycles*/
        case(0xF9): sp = makeaddress(reg[L], reg[H]); break;
        /*LD HL, SP+n - 12cycles
         * flags affected: Z-reset, N-reset, H-re/set C-re/set */
        case(0xF8): ;unsigned short int res = sp + getData();
            reg[H] = res >> 8;
            reg[L] = (res << 8) >> 8;
            f_z = 0; f_s = 0;
            f_hc = ((sp << 8) >> 8) >= reg[L];
            f_c = sp >= res;
            break;
        /*LD (nn),SP - 20 cycles*/
        case(0x08): mem[makeaddress(getData(), getData())] = sp; break;
        /*PUSH nn - 16 cycles*/
        case(0xF5): 
            mem[sp] = reg[A]; sp-=1; mem[sp] = reg[F]; sp-=1; break;
        case(0xC5): 
            mem[sp] = reg[B]; sp-=1; mem[sp] = reg[C]; sp-=1; break;
        case(0xD5): 
            mem[sp] = reg[D]; sp-=1; mem[sp] = reg[E]; sp-=1; break;
        case(0xE5): 
            mem[sp] = reg[H]; sp-=1; mem[sp] = reg[L]; sp-=1; break;
        /*POP nn - 12 cycles*/
        case(0xF1): 
            reg[F] = mem[sp]; sp+=1; reg[A] = mem[sp]; sp+=1; break;
        case(0xC1): 
            reg[C] = mem[sp]; sp+=1; reg[B] = mem[sp]; sp+=1; break;
        case(0xD1): 
            reg[E] = mem[sp]; sp+=1; reg[D] = mem[sp]; sp+=1; break;
        case(0xE1): 
            reg[L] = mem[sp]; sp+=1; reg[H] = mem[sp]; sp+=1; break;

        /*
         * ALU - Arithmetic operations
         */



    }
}

/* Gets the next byte of data in memory */
unsigned char getData(){
    char res = mem[pc];
    pc += 1;
    return res;
}

/* Creates a 16-bit address from given high and low bit registers */
unsigned short int makeaddress(char low, char high){
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
