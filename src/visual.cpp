/* This file will handle (as a minimum) the visualization of the emulator 
 * in the form of interfacing with a user's screen.
 *
 * Author Maxwell J Svetlik
 */

#include <SDL2/SDL.h>
#include <stdio.h>

#include "visualization.h"
#include "video.h"

const int SCREEN_WIDTH = 166;
const int SCREEN_HEIGHT = 140;

const int BUFFER_WIDTH = 255;
const int BUFFER_HEIGHT = 255;

Uint32 trans;   //transparent 
Uint32 lgray;   //light gray
Uint32 gray;    //gray
Uint32 dgray;   //dark gray

SDL_Surface* surface;
SDL_Window* window;

void initilize_colors(SDL_Surface *surface){
    trans = SDL_MapRGBA(surface->format, 255,255,255,0xFF); 
    lgray = SDL_MapRGBA(surface->format, 195, 195, 195, 0xFF);
    gray = SDL_MapRGBA(surface->format, 100, 100, 100, 0xFF);
    dgray = SDL_MapRGBA(surface->format, 15, 15, 15, 0xFF);
}
Uint32 get_pixel32( SDL_Surface *surface, int x, int y ) { 
    Uint32 *pixels = (Uint32 *)surface->pixels; 
    return pixels[ ( y * surface->w ) + x ]; 
} 

void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel ) { 
    Uint32 *pixels = (Uint32 *)surface->pixels; 
    pixels[ ( y * surface->w ) + x ] = pixel; 
}

/* Initilialize needed SDL structures.
 * This must be run before any visualization methods are called
 */
void init_visualization(){
    SDL_Window* win = NULL;
    
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    } else{
        win = SDL_CreateWindow( "GouldBoyColor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BUFFER_WIDTH, BUFFER_HEIGHT, SDL_WINDOW_SHOWN );
        if( win == NULL){
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            exit(-1);
        } else {
       //Get window surface
        screenSurface = SDL_GetWindowSurface( win );

         initilize_colors(screenSurface);
        
        //Set background color (white, here)
        SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
           
        //Write to window
        SDL_UpdateWindowSurface( win );
       }
    }
    surface = screenSurface;
    window = win;
}

/* This draws a line of pixels onto a window and pushes changes through
 * Note. This is a terrible way to do it; writing directly to window is very slow.
 *      Must utilized SDL's tile mapper for any decent performance.
 * It is assumed that 'line' is an array of 8 pixels, each denoted by a color value in a supported pallet.
 */
void draw_line(unsigned int line[], int x, int y){
    Uint32 *pixels = (Uint32 *)surface->pixels;
    int i;
    Uint32 color;
    int pallet_col;
    for(i = 0; i < 8; i+=1){
        pallet_col = line[i];
        if(pallet_col  == LGRAY)
            color = lgray;
        else if(pallet_col  == DGRAY)
            color = gray;
        else if(pallet_col == BLACK)
            color = dgray;
        else if(pallet_col == WHITE)
            color = trans;
        pixels[(y * surface->w) + x + i] = color;
    }
    SDL_UpdateWindowSurface( window );    
}

/* Draw all the tiles referenced in 0x8800 to 8fff or 0x8800 to 97ff depending on LCDC reg.
 * These locations map to tiles in 0x9800 ti 9bff or 0x9c00 to 9fff
 *
 * For now, just drawing 9800 to 9bff straight
 * 
 * In this area, colors for pixels within a tile is determined by a two byte quantity, the first describing the LSBs and the 2nd describing the MSBs
 */
void draw_tile(unsigned char mem[]){
    int start = 0x9800;
    int tile_base = 0x8800;
    unsigned int draw_vect[8];
    int i,j,k;
    int x = 0; int y = 0;
    for(i = 0; i < 1024; i+=1){
        unsigned char tile_num = 16 * mem[start+i]; //each tile identifier corresponds to 16 bytes
        if(!(i%32)) x = 0; else x += 8;
        //draw one single tile reference
        for(k = 0; k < 16; k+=2){
            unsigned char lsb = mem[tile_base+tile_num];
            unsigned char msb = mem[tile_base+tile_num+1];
            //printf("Found lsb: %x msb: %x at %x\n", lsb, msb, tile_base+tile_num);
            for(j = 2; j < 8; j+=1){
                unsigned char mask = 1 << j;
                draw_vect[j] = ((msb & mask) >> (j - 1)) + ((lsb & mask) >> (j));
            }
            draw_vect[0] = (msb & 0x1) << 1 + (lsb & 0x1);
            draw_vect[1] = (msb & 0x2) + ((lsb & 0x2) >> 1);
            //int bs;
            //for(bs = 0; bs < 8; bs++)
            //    printf("%d", draw_vect[bs]);
            //printf("\n");
            draw_line(draw_vect, x, y+(k/2));
        }
        if(i % 32 == 0 && i !=0)
            y += 8;
    }
}

//Exit SDL cleanly
void exit_clean(){
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
/*
int main(int argc, char** args){
    init();
    exit_clean();
}*/

