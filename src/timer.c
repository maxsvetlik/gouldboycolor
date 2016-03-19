/* Holds functions to keep relative time.
 *
 * Author Maxwell J Svetlik
 */

#include "gbc_time.h"
#include "z80.h"

unsigned int cycles_passed = 0;
unsigned int div_incrm = 0;
/*
 * How many cycles until we need to increment the div register?
 */
unsigned int cyc_div(unsigned int cpu_freq){
    return cpu_freq / 16384;
}

void tick(unsigned char num_cycles){
    cycles_passed = cycles_passed + num_cycles;

    //div_incrm should be set by cyc_div prior to execution
    if(cycles_passed - div_incrm < 0){
        //increment div
        unsigned char old_div = mem[DIV];
        mem[DIV] = old_div + 1;
        //Do i need to do some kind of overflow handling here?
    }

}
