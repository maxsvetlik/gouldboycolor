/* This file is a dumb visualizer- it writes all contents as raw values to stdout
 * 
 * A debugging and conceptual proof tool.
 */

#include <stdio.h>

#include "z80.h"
#include "interrupt.h"
#include "video.h"

/* Fetches a tile and writes the colors to screen
 * uses the x and y inputs as a base for 
 */
void gettile(unsigned short int loc){
    int i,j;
    unsigned char tile[8];
    //each tile is made of 16 bytes
    //but each line uses 2 bytes
    for(i = 0; i < 8; i+=2){
        unsigned short int lsig = mem[loc+i];
        unsigned short int msig = mem[loc+i+1];
        
        //compute the resultant color by combining the bytes above
        for(j = 0; j < 8; j+=1){
            tile[j] = 2*((msig << (7-j)) >> 7) + ((lsig << (7-j)) >> 7);
        }
        //write to stdout
        for(j = 0; j < 8; j += 1);
            printf("%u", tile[j]);
        printf("\n");
    }

}
