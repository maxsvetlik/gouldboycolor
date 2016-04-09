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
        if(pallet_col  == 0)
            color = lgray;
        else if(pallet_col  == 2)
            color = dgray;
        else if(pallet_col == 3)
            color = dgray;
        else if(pallet_col == 1)
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
    int tile_base = 0x8000;
    unsigned int draw_vect[8];
    int i,j,k;
    int x = 0; int y = 0;
    //mem[0xFF44] = 0x90;
    int tempy = 0;
    for(i = 0; i < 1024; i+=1){
        unsigned char tile_num = mem[start+i]; 
        
        //draw one single tile reference
        for(k = 0; k < 16; k+=2){
            unsigned char lsb = mem[tile_base+(16*tile_num)+k];     //every tile is 16 bytes
            unsigned char msb = mem[tile_base+(16*tile_num)+k+1];   // ...
            //if(lsb || msb)
            //    printf("Found lsb: %x msb: %x at %x from map %x\n", lsb, msb, tile_base+tile_num, start+i);
            
            draw_vect[0] = ((msb & 1 << 7) >> 6) + ((lsb & 1 << 7) >> 7);
            draw_vect[1] = ((msb & 1 << 6) >> 5) + ((lsb & 1 << 6) >> 6);
            draw_vect[2] = ((msb & 1 << 5) >> 4) + ((lsb & 1 << 5) >> 5);
            draw_vect[3] = ((msb & 1 << 4) >> 3) + ((lsb & 1 << 4) >> 4);
            draw_vect[4] = ((msb & 1 << 3) >> 2) + ((lsb & 1 << 3) >> 3);
            draw_vect[5] = ((msb & 1 << 2) >> 1) + ((lsb & 1 << 2) >> 2);
            draw_vect[6] = ((msb & 1 << 1) >> 0) + ((lsb & 1 << 1) >> 1);
            draw_vect[7] = ((msb & 1 << 0) << 1) + ((lsb & 1 << 0) >> 0);
            
            draw_line(draw_vect, x, y+(k/2));
            
        }
        if(x >= 248){ 
            x = 0; 
            y += 8;
        }
        else 
            x += 8;
    }
}

//Exit SDL cleanly
void exit_clean(){
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

