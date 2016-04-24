#include "interrupt.h"
#include "z80.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
void enable_interrupts(){
    f_ime = 1;
}
void disable_interrupts(){
    f_ime = 0;
}

/* In the event of an interrupt:
 *  -reset IME flag to prevent further interrupts
 *  -push pc onto stack
 *  -jump to the iterrupt vector location
 */
void interrupt_handler(){
    unsigned char f_if = mem[IFFLAG];
    //unsigned char f_ie = mem[IEFLAG];
    unsigned char mask = 0x1;
    
    //error check. we should never enter the handler if 
    //ime isn't set.
    if(!f_ime){
        printf("ERROR: Interrupt master enable flag is not set, but the interrupt handler has been entered.\n");
        crash_dump();
        exit(-1);
    }
    int i;
    //5 interrupt vector entries
    for(i = 0; i < 5; i++){
        if((f_if & mask)){// && (f_ie & mask)){
            f_ime = 0;
            mem[sp] = pc;
            sp -= 1;
            sp = interrupt_table[i];
            printf("INTERRUPT!\n");
            break;
        }
        mask = mask << 1;
    }
}
