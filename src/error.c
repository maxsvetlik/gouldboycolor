/* This file provides various methods for error handling 
 * including output messages to screen, file as well as
 * memory dumps.
 *
 * Author: Maxwell J Svetlik
 */

#include "error.h"
#include "z80.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
void crash_dump(){
    FILE *fp;
    char filename [256];
    snprintf(filename, 256, "crashdump_%d.dat", (int) time(NULL));
    fp = fopen(filename, "w");
    fprintf(stderr, "Writing crash to file: %s \n", filename);
    int i;
    fprintf(fp, "PC: %u\n", pc);
    fprintf(fp, "sp: %u\n", sp);
    fprintf(fp, "f_z: %u\n", f_z);
    fprintf(fp, "f_s: %u\n", f_s);
    fprintf(fp, "f_hc: %u\n", f_hc);
    fprintf(fp, "f_c: %u\n", f_c);
    
    for(i = 0; i < GP_REGS; i+=1)
        fprintf(fp,"Reg[%d]: %5s %d\n", i, " ", reg[i]);
    fprintf(fp, "\n");

    for(i = 0; i < MEM_SIZE; i+= 1){
        fprintf(fp,"Mem[%d]: %5s %u\n", i, " ", mem[0]);
    }
    fclose(fp);
}
