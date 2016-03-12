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
#include "error.h"

#define DEBUG 1
#define DB_OPCODE 1
unsigned char mem[MEM_SIZE];
char reg[GP_REGS];
unsigned short int  pc;
unsigned short int  sp;
unsigned char       f_z;
unsigned char       f_s;
unsigned char       f_hc;
unsigned char       f_c;

/* These are TEMPORARY vars not part of the CPU state */
unsigned char       val;
unsigned char       val2;
unsigned short int  usi;
unsigned short int  usi2;
char                sval;


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
        
        /*8-bit ALU ops*/
        //add n to A. all flags affected. 4cylces unless specified
        case(0x87): val = reg[A];
                    reg[A] = reg[A] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,val);                break;
        case(0x80): val = reg[A];
                    reg[A] = reg[B] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,reg[B]);             break;
        case(0x81): val = reg[A];
                    reg[A] = reg[C] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,reg[C]);             break;
        case(0x82): val = reg[A];
                    reg[A] = reg[D] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,reg[D]);             break;
        case(0x83): val = reg[A];
                    reg[A] = reg[E] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,reg[E]);             break;
        case(0x84): val = reg[A];
                    reg[A] = reg[H] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,reg[H]);             break;
        case(0x85): val = reg[A];
                    reg[A] = reg[L] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,val);                break;
        case(0x86): val = reg[A];
                    reg[A] = mem[makeaddress(reg[H], reg[L])] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,mem[makeaddress(reg[H], reg[L])]) ;
                    break; //8 cycles
        case(0xC6): val = reg[A];
                    val2 = getData();
                    reg[A] = val2 + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,val2);               break; //8 cycles

        /* n + CARRY -> A. 4 cycles unless noted*/
        case(0x8F): val = reg[A] + f_c;
                    reg[A] = reg[A] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,val);                break;
        case(0x88): val = reg[A] + f_c;
                    reg[A] = reg[B] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,reg[B]);             break;
        case(0x89): val = reg[A] + f_c;
                    reg[A] = reg[C] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,reg[C]);             break;
        case(0x8A): val = reg[A] + f_c;
                    reg[A] = reg[D] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,reg[D]);             break;
        case(0x8B): val = reg[A] + f_c;
                    reg[A] = reg[E] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,reg[E]);             break;
        case(0x8C): val = reg[A] + f_c;
                    reg[A] = reg[H] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,reg[H]);             break;
        case(0x8D): val = reg[A] + f_c;
                    reg[A] = reg[L] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,val);                break;
        case(0x8E): val = reg[A] + f_c;
                    reg[A] = mem[makeaddress(reg[H], reg[L])] + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,mem[makeaddress(reg[H], reg[L])]) ;
                    break; //8 cycles
        case(0xCE): val = reg[A] + f_c;
                    val2 = getData();
                    reg[A] = val2 + reg[A];
                    f_s = 0; f_z = reg[A] == 0;
                    setflags_carry(ADD,val,val2);               break; //8 cycles

        //sub n to A. all flags affected. 4cycles unless specified
        case(0x97): val = reg[A];
                    reg[A] = reg[A] - reg[A];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,val);                break;
        case(0x90): val = reg[A];
                    reg[A] = reg[A] - reg[B];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,reg[B]);             break;
        case(0x91): val = reg[A];
                    reg[A] = reg[A] - reg[C];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,reg[C]);             break;
        case(0x92): val = reg[A];
                    reg[A] = reg[A] - reg[D];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,reg[D]);             break;
        case(0x93): val = reg[A];
                    reg[A] = reg[A] - reg[E];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,reg[E]);             break;
        case(0x94): val = reg[A];
                    reg[A] = reg[A] - reg[H];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,reg[H]);             break;
        case(0x95): val = reg[A];
                    reg[A] = reg[A] - reg[L];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,val);                break;
        case(0x96): val = reg[A];
                    reg[A] = reg[A] - mem[makeaddress(reg[H], reg[L])];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,mem[makeaddress(reg[H], reg[L])]) ;
                    break; //8 cycles
        case(0xD6): val = reg[A];
                    val2 = getData();
                    reg[A] = reg[A] - val2;
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,val2);               break; //8 cycles

        // A<- A - n - CARRY. all flags affected. 4cycles unless specified
        case(0x9F): val = reg[A] - f_c;
                    reg[A] = reg[A] - reg[A];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,val);                break;
        case(0x98): val = reg[A] - f_c;
                    reg[A] = reg[A] - reg[B];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,reg[B]);             break;
        case(0x99): val = reg[A] - f_c;
                    reg[A] = reg[A] - reg[C];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,reg[C]);             break;
        case(0x9A): val = reg[A] - f_c;
                    reg[A] = reg[A] - reg[D];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,reg[D]);             break;
        case(0x9B): val = reg[A] - f_c;
                    reg[A] = reg[A] - reg[E];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,reg[E]);             break;
        case(0x9C): val = reg[A] - f_c;
                    reg[A] = reg[A] - reg[H];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,reg[H]);             break;
        case(0x9D): val = reg[A] - f_c;
                    reg[A] = reg[A] - reg[L];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,val);                break;
        case(0x9E): val = reg[A] - f_c;
                    reg[A] = reg[A] - mem[makeaddress(reg[H], reg[L])];
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,mem[makeaddress(reg[H], reg[L])]) ;
                    break; //8 cycles
        /* undefined op: A<- A - # - CARRY
        case(0xD6): val = reg[A] - f_c;
                    val2 = getData();
                    reg[A] = reg[A] - val2;
                    f_s = 1; f_z = reg[A] == 0;
                    setflags_carry(SUB,val,val2);               break; //8 cycles
        */
        
        /* AND: logical and n with A -> A
            HC flag: set, C flag: reset. Set others accordingly
            4 cycles unless specified*/
        case(0xA7): val = reg[A] && reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 1; f_c = 0;    break;
        case(0xA0): val = reg[B] && reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 1; f_c = 0;    break;
        case(0xA1): val = reg[C] && reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 1; f_c = 0;    break;
        case(0xA2): val = reg[D] && reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 1; f_c = 0;    break;
        case(0xA3): val = reg[E] && reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 1; f_c = 0;    break;
        case(0xA4): val = reg[H] && reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 1; f_c = 0;    break;
        case(0xA5): val = reg[L] && reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 1; f_c = 0;    break;
        case(0xA6): val = mem[makeaddress(reg[H], reg[L])] && reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 1; f_c = 0;    break;      //8 cycles
        case(0xE6): val = getData() && reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 1; f_c = 0;    break;      //8 cycles

        /* OR: logical or n with A -> A
            Z: Set if 0, all other flags: reset
            4 cycles unless specified*/

        case(0xB7): val = reg[A] || reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xB0): val = reg[B] || reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xB1): val = reg[C] || reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xB2): val = reg[D] || reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xB3): val = reg[E] || reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xB4): val = reg[H] || reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xB5): val = reg[L] || reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xB6): val = mem[makeaddress(reg[H], reg[L])] || reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;      //8 cycles
        case(0xF6): val = getData() || reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;      //8 cycles

        /* XOR: logical xor n with A -> A
            Z: Set if 0, all other flags: reset
            4 cycles unless specified*/

        case(0xAF): val = reg[A] ^ reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xA8): val = reg[B] ^ reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xA9): val = reg[C] ^ reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xAA): val = reg[D] ^ reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xAB): val = reg[E] ^ reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xAC): val = reg[H] ^ reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xAD): val = reg[L] ^ reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;
        case(0xAE): val = mem[makeaddress(reg[H], reg[L])] ^ reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;      //8 cycles
        case(0xEE): val = getData() ^ reg[A];
                    reg[A] = val;
                    f_s = 0; f_z = !!val; f_hc = 0; f_c = 0;    break;      //8 cycles

        /*CP A with n. A-n, sets flags but tosses result
            4cycles unless specified. f_c set if no borrow*/
        case(0xBF): f_z = reg[A] == reg[A]; f_s = 1;
                    setflags_carry(SUB,reg[A],reg[A]);
                    f_c = reg[A] < reg[A];                      break;
        case(0xB8): f_z = reg[A] == reg[B]; f_s = 1;
                    setflags_carry(SUB,reg[A],reg[B]);
                    f_c = reg[A] < reg[B];                      break;
        case(0xB9): f_z = reg[A] == reg[C]; f_s = 1;
                    setflags_carry(SUB,reg[A],reg[C]);
                    f_c = reg[A] < reg[C];                      break;
        case(0xBA): f_z = reg[A] == reg[D]; f_s = 1;
                    setflags_carry(SUB,reg[A],reg[D]);
                    f_c = reg[A] < reg[D];                      break;
        case(0xBB): f_z = reg[A] == reg[E]; f_s = 1;
                    setflags_carry(SUB,reg[A],reg[E]);
                    f_c = reg[A] < reg[E];                      break;
        case(0xBC): f_z = reg[A] == reg[H]; f_s = 1;
                    setflags_carry(SUB,reg[A],reg[H]);
                    f_c = reg[A] < reg[H];                      break;
        case(0xBD): f_z = reg[A] == reg[L]; f_s = 1;
                    setflags_carry(SUB,reg[A],reg[L]);
                    f_c = reg[A] < reg[L];                      break;
        case(0xBE): val = mem[makeaddress(reg[H], reg[L])];
                    f_z = reg[A] == val; f_s = 1;
                    setflags_carry(SUB,reg[A],val);
                    f_c = reg[A] < val;                         break;      //8 cycles
        case(0xFE): val = getData();
                    f_z = reg[A] == val; f_s = 1;
                    setflags_carry(SUB,reg[A],val);
                    f_c = reg[A] < val;                         break;      //8 cycles

        /*INC n - increment register. f_s reset. f_c unaffected. 
            f_hc set for b3 carry 4 cycles unless specified*/
        case(0x3C): val = reg[A] + 1; f_z = !!val; f_s = 0;
                    set_hc_3b(ADD, reg[A], val);
                    reg[A] = val;                               break;
        case(0x04): val = reg[B] + 1; f_z = !!val; f_s = 0;
                    set_hc_3b(ADD, reg[B], val);                
                    reg[B] = val;                               break;
        case(0x0C): val = reg[C] + 1; f_z = !!val; f_s = 0;
                    set_hc_3b(ADD, reg[C], val);
                    reg[C] = val;                               break;
        case(0x14): val = reg[D] + 1; f_z = !!val; f_s = 0;
                    set_hc_3b(ADD, reg[D], val);
                    reg[D] = val;                               break;
        case(0x1C): val = reg[E] + 1; f_z = !!val; f_s = 0;
                    set_hc_3b(ADD, reg[E], val);
                    reg[E] = val;                               break;
        case(0x24): val = reg[H] + 1; f_z = !!val; f_s = 0;
                    set_hc_3b(ADD, reg[H], val);
                    reg[H] = val;                               break;
        case(0x2C): val = reg[L] + 1; f_z = !!val; f_s = 0;
                    set_hc_3b(ADD, reg[L], val);
                    reg[L] = val;                               break;
        case(0x34): val = mem[makeaddress(reg[H],reg[L])] + 1; 
                    f_z = !!val; f_s = 0;
                    set_hc_3b(ADD, val, mem[makeaddress(reg[H],reg[L])] );
                    mem[makeaddress(reg[H], reg[L])] = val;     break;      //12 cycles

        /*DEC n - deccrement register. f_s set. f_c unaffected. 
            f_hc set for if no carry from b4. 4 cycles unless specified*/
        case(0x3D): val = reg[A] - 1; f_z = !!val; f_s = 0;
                    set_hc_nb_4b(SUB, reg[A], 1);
                    reg[A] = val;                               break;
        case(0x05): val = reg[B] - 1; f_z = !!val; f_s = 0;
                    set_hc_nb_4b(SUB, reg[B], 1);                
                    reg[B] = val;                               break;
        case(0x0D): val = reg[C] - 1; f_z = !!val; f_s = 0;
                    set_hc_nb_4b(SUB, reg[C], 1);
                    reg[C] = val;                               break;
        case(0x15): val = reg[D] - 1; f_z = !!val; f_s = 0;
                    set_hc_nb_4b(SUB, reg[D], 1);
                    reg[D] = val;                               break;
        case(0x1D): val = reg[E] - 1; f_z = !!val; f_s = 0;
                    set_hc_nb_4b(SUB, reg[E], 1);
                    reg[E] = val;                               break;
        case(0x25): val = reg[H] - 1; f_z = !!val; f_s = 0;
                    set_hc_nb_4b(SUB, reg[H], 1);
                    reg[H] = val;                               break;
        case(0x2D): val = reg[L] - 1; f_z = !!val; f_s = 0;
                    set_hc_nb_4b(SUB, reg[L], 1);
                    reg[L] = val;                               break;
        case(0x35): val = mem[makeaddress(reg[H],reg[L])] - 1; 
                    f_z = !!val; f_s = 0;
                    set_hc_nb_4b(SUB, mem[makeaddress(reg[H],reg[L])], 1);
                    mem[makeaddress(reg[H], reg[L])] = val;     break;      //12 cycles
        
        /*
         * 16 bit ALU
         */

        /* add n to HL. 8 cycles.
         * f_z = not affected, f_s reset, f_hc = if carry from bit 11,
         * f_c = if carry from bit 15 */
        case(0x09): usi = mem[makeaddress(reg[H], reg[L])];
                    usi2 = mem[makeaddress(reg[B], reg[C])];
                    setflags_carry(ADD, usi, usi2); f_s = 0;
                    mem[makeaddress(reg[H], reg[L])] = usi + usi2; break;
        case(0x19): usi = mem[makeaddress(reg[H], reg[L])];
                    usi2 = mem[makeaddress(reg[D], reg[E])];
                    setflags_carry(ADD, usi, usi2); f_s = 0;
                    mem[makeaddress(reg[H], reg[L])] = usi + usi2; break;
        case(0x29): usi = mem[makeaddress(reg[H], reg[L])];
                    usi2 = mem[makeaddress(reg[H], reg[L])];
                    setflags_carry(ADD, usi, usi2); f_s = 0;
                    mem[makeaddress(reg[H], reg[L])] = usi + usi2; break;
        case(0x39): usi = mem[makeaddress(reg[H], reg[L])];
                    usi2 = sp;
                    setflags_carry(ADD, usi, usi2); f_s = 0;
                    mem[makeaddress(reg[H], reg[L])] = usi + usi2; break;
        /* Add n to sp, n is a 1byte signed immediate.
         * f_z reset, f_s reset, f_hc f_c set  16 cycles. */
        case(0xE8): sval = getData();
                    usi = sp + sval;
                    f_hc = ((usi & 0x80) >> 3) ^ ((sp & 0x80) >> 3);
                    f_c = ((usi & 0x8000) >> 15) ^ ((sp & 0x8000) >> 15);
                    f_z = 0; f_s = 0;
                    sp = usi;                                   break;
        /* Inc nn register. No flags affected. 8 cycles*/
        case(0x03): mem[makeaddress(reg[B], reg[C])] += 1;      break;
        case(0x13): mem[makeaddress(reg[D], reg[E])] += 1;      break;
        case(0x23): mem[makeaddress(reg[H], reg[L])] += 1;      break;
        case(0x33): sp += 1;                                    break;
        /* Dec nn register. No flags affected. 8 cycles*/
        case(0x0B): mem[makeaddress(reg[B], reg[C])] -= 1;      break;
        case(0x1B): mem[makeaddress(reg[D], reg[E])] -= 1;      break;
        case(0x2B): mem[makeaddress(reg[H], reg[L])] -= 1;      break;
        case(0x3B): sp -= 1;                                    break;
        
        /* 
         * END OF ALU
         * Misc Operations ->
         */
        /*Swap upper and lower nibbles of n. f_z set if applic., 
         * f_s, f_hc, f_c reset. 8 cycles unless specified.*/
        case(0xCB): val = getData();
                    switch(val){
                        case(0x37): val2 = 0; val2 = val2 | (reg[A] & 0x0F);
                            val2 = (val2 << 3) | (reg[A] & 0xF0);   break;
                        case(0x30): val2 = 0; val2 = val2 | (reg[B] & 0x0F);
                            val2 = (val2 << 3) | (reg[B] & 0xF0);   break;
                        case(0x31): val2 = 0; val2 = val2 | (reg[C] & 0x0F);
                            val2 = (val2 << 3) | (reg[C] & 0xF0);   break;
                        case(0x32): val2 = 0; val2 = val2 | (reg[D] & 0x0F);
                            val2 = (val2 << 3) | (reg[D] & 0xF0);   break;
                        case(0x33): val2 = 0; val2 = val2 | (reg[E] & 0x0F);
                            val2 = (val2 << 3) | (reg[E] & 0xF0);   break;
                        case(0x34): val2 = 0; val2 = val2 | (reg[H] & 0x0F);
                            val2 = (val2 << 3) | (reg[H] & 0xF0);   break;
                        case(0x35): val2 = 0; val2 = val2 | (reg[L] & 0x0F);
                            val2 = (val2 << 3) | (reg[L] & 0xF0);   break;
                        case(0x36): val = mem[makeaddress(reg[H], reg[L])];
                                    val2 = 0; val2 = val2 | (val & 0x0F);
                                    val2 = (val2 << 3) | (val & 0xF0);   break; //16 cycles
                    }
                    //potentially unsafe
                    f_s = 0; f_hc = 0; f_c = 0; f_z = !!val2;
                                                                break;
        /*DAA - Decimal adjust register A. */

        /*Complement Register A. set f_s, f_hc. others not affected. 4 cycles*/
        case(0x2F): reg[A] = ~reg[A]; f_s = 1; f_hc = 1;            break;
        
        /*Complement Carry flag. f_z unaffected. f_s, f_hc reset. 4 cycles*/
        case(0x3F): f_c = !f_c; f_s = 0; f_hc = 0;                  break;

        /*Set carry flag. f_z unaffected. f_s, f_hc reset. 4 cycles.*/
        case(0x37): f_c = 1; f_s = 0; f_hc = 0;                     break;

        /*Noop. 4 cycles*/
        case(0x00):                                                 break;
        /*halt. Power down until interrupt. Power saving feature. 4 cycles*/
        case(0x76): halt();                                         break;
        /*stop. Halts CPU and screen until button pressed. 4 cycles.
         * note: 2byte instruction 0x1000 that I'm shortening to 0x10*/
        case(0x10): stop();                                         break;
        /* Disable interrupts after the instruction AFTER this one 
         * is executed. 4 cycles*/
        case(0xF3): disable_interrupt_on_next();                    break;
        /* Enable interrupts after the instruction AFTER this one 
         * is executed. 4 cycles*/
        case(0xFB): enable_interrupt_on_next();                     break;
        
        default: fprintf(stderr, "Undefined opcode. %uc at %u\n. Halting. \n", op, sp);
                    crash_dump();                                   break;
    }
}

/* Sets the HC (half carry, bit 3), and C (carry, bit 7) flags directly
 * Note: This is an 8bit function*/
void setflags_carry(char op, unsigned char x, unsigned char y){
    if(op == ADD){
        f_hc = ((x << 5) >> 7) & ((y << 5) >> 7);
        f_c = ((x << 1) >> 7) & ((y << 1) >> 7);
    }
    else if(op == SUB){
        char x7 = x >> 7;
        char y7 = y >> 7;
        char r7 = (x - y) >> 7;
        f_c = (~x7 & y7) | (y7 & r7) | (r7 & ~x7);
        
        char x3 = x >> 4;
        char y3 = y >> 4;
        char r3 = (x - y) >> 4;
        f_hc = (~x3 & y3) | (y3 & r3) | (r3 & ~x3);
    }
}
/*Sets the HC flag for a carry on bit 11.
 * Sets the C flag for a carry on bit 15.
 * Note: This is a 16 bit function (obviously) */
void setflags_carry16(char op, unsigned short int x, unsigned short int y){
    if(op == ADD){
        f_hc = ((x << 4) >> 15) & ((y << 4) >> 15);
        f_c = ((x << 1) >> 15) & ((y << 1) >> 15);
    }
    else if(op == SUB){
        unsigned short int x7 = x >> 7;
        unsigned short int y7 = y >> 7;
        unsigned short int r7 = (x - y) >> 7;
        f_c = (~x7 & y7) | (y7 & r7) | (r7 & ~x7);
        
        unsigned short int x3 = x >> 4;
        unsigned short int y3 = y >> 4;
        unsigned short int r3 = (x - y) >> 4;
        f_hc = (~x3 & y3) | (y3 & r3) | (r3 & ~x3);
    }
}

/* Sets the hc flag if there is carry in bit 3
 */
void set_hc_3b(char op, unsigned char x, unsigned char y){
    if(op == ADD)
        f_hc = ((x << 5) >> 7) & ((y << 5) >> 7);
}

/* Sets the hc flag if there is is `no borrow` from bit 4
 */
void set_hc_nb_4b(char op, unsigned char x, unsigned char y){
    if(op == SUB){
        char x4 = x >> 4;
        char y4 = y >> 4;
        char r4 = (x - y) >> 4;
        f_hc = !((~x4 & y4) | (y4 & r4) | (r4 & ~x4));
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

void clear_mem(){
    int i;
    for(i = 0; i < MEM_SIZE; i+=1)
        mem[i] = 0;
}
//power on or reset sequence.
//initializes the state as the gameboy expects
void reset(){
    clear_mem();
    pc = 0x100; //beginning of the bootup rom sequence
    sp = 0xFFFE;
}
void halt(){}
void stop(){}
void disable_interrupt_on_next(){}
void enable_interrupt_on_next(){}

void cpu_init(){
    reset();
    mem[0x100] = 0x12;
}
