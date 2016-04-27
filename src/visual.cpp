/* This file will handle (as a minimum) the visualization of the emulator 
 * in the form of interfacing with a user's input.
 *
 * Author Maxwell J Svetlik
 */

#include <SDL2/SDL.h>
#include <stdio.h>

#include "visualization.h"
#include "video_const.h"
#include "interrupt.h"

const int SCREEN_WIDTH = 166;
const int SCREEN_HEIGHT = 140;

const int BUFFER_WIDTH = 255;
const int BUFFER_HEIGHT = 255;

Uint32 white;   //transparent 
Uint32 lgray;   //light gray
Uint32 gray;    //gray
Uint32 dgray;   //dark gray
Uint32 black;   //black

unsigned char color_map[4];

SDL_Surface* surface;   //holds the surface of the screen
SDL_Window* window;     //holds the window
SDL_Surface* fullBuff;  //holds the full 'background' buffer, 256x256 pixels

SDL_Renderer* renderer; 
SDL_Texture* fullTexture;

SDL_Rect screen_rect;

int cyc_count  = REWRITE_CYCLES;

void initilize_colors(SDL_Surface *surface){
    white = SDL_MapRGBA(surface->format, 255,255,255,0xFF); 
    lgray = SDL_MapRGBA(surface->format, 195, 195, 195, 0xFF);
    gray = SDL_MapRGBA(surface->format, 100, 100, 100, 0xFF);
    dgray = SDL_MapRGBA(surface->format, 15, 15, 15, 0xFF);
    black = SDL_MapRGBA(surface->format, 0, 0, 0, 0xFF);
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
        win = SDL_CreateWindow( "GouldBoyColor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( win == NULL){
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            exit(-1);
        } else {
        //Get window surface
        //screenSurface = SDL_GetWindowSurface( win );
        renderer = SDL_CreateRenderer( win, -1, SDL_RENDERER_ACCELERATED );
        //SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
        //SDL_UpdateWindowSurface( win );
        printf("Graphics Initialization successful.\n");
        }
    }
    fullBuff =  SDL_CreateRGBSurface(0,BUFFER_WIDTH,BUFFER_HEIGHT,32,0,0,0,0);
    initilize_colors(fullBuff);
    surface = screenSurface;
    window = win;
    screen_rect.w = SCREEN_WIDTH;
    screen_rect.h = SCREEN_HEIGHT;
}

/* This draws a line of pixels onto a window and pushes changes through
 * Note. This is a terrible way to do it; writing directly to window is very slow.
 *      Must utilized SDL's tile mapper for any decent performance.
 * It is assumed that 'line' is an array of 8 pixels, each denoted by a color value in a supported pallet.
 */
void draw_line(unsigned int line[], int x, int y){
    Uint32 *pixels = (Uint32 *)fullBuff->pixels;
    int i;
    Uint32 color;
    int pallet_col;
    for(i = 0; i < 8; i+=1){
        pallet_col = line[i];
        color = color_map[pallet_col];
        if(color  == WHITE)
            color = white;
        else if(color  == LGRAY)
            color = lgray;
        else if(color == DGRAY)
            color = dgray;
        else if(color == BLACK)
            color = black;
        pixels[(y * fullBuff->w) + x + i] = color;
    }
}

/* This method draws the appropriate region of the fullBuffer to the screen
 * buffer and window.
 * This is accounted for by visual registers in VRAM
 */
void flush_to_screen(unsigned char mem[]){
    unsigned char xpos = mem[SCX];
    unsigned char ypos = mem[SCY];
    /*Uint32 *buff_pixels = (Uint32 *)fullBuff->pixels;
    Uint32 *screen_pixels = (Uint32 *)surface->pixels;

    int i,j;
    for(i = 0; i < SCREEN_WIDTH; i += 1)
        for(j = 0; j < SCREEN_HEIGHT; j += 1){
            screen_pixels[(j * surface->w) + i] = buff_pixels[((ypos + j) * fullBuff->w) + xpos + i];        
        }
    */
    fullTexture = SDL_CreateTextureFromSurface( renderer, fullBuff );
    screen_rect.x = xpos;
    screen_rect.y = ypos;
    //Render texture to screen 
    SDL_RenderCopy( renderer, fullTexture, &screen_rect, NULL ); 
    //Update screen 
    SDL_RenderPresent( renderer );
    SDL_DestroyTexture(fullTexture);
}

/* Draw all the tiles referenced in 0x8800 to 8fff or 0x8800 to 97ff depending on LCDC reg.
 * These locations map to tiles in 0x9800 ti 9bff or 0x9c00 to 9fff
 *
 * For now, just drawing 9800 to 9bff straight
 * 
 * In this area, colors for pixels within a tile is determined by a two byte quantity, the first describing the LSBs and the 2nd describing the MSBs
 */
void draw_tile(unsigned char mem[]){
    unsigned int draw_vect[8];
    int i,j,k;
    int x = 0; int y = 0;

    int start = 0x9800;
    int tile_base = 0x8000;
    char signed_tiles = 0;
    int bindex;

    Uint32 color;
    int pallet_col;
    Uint32 * buff_pix = (Uint32 *)fullBuff->pixels;
    //mem[0xFF44] = 0x90;
    //check the LCDC register, set approriate flags
    unsigned char lcdc = mem[LCDC];
    char lcd_enable = !!(lcdc & 0x80);
    
    if(lcd_enable){
        //TODO:set window tile map display region
        //TODO:set window display enable
        //TODO:set Background and Window tile region
        if(!(lcdc & 0x10)){
            tile_base = 0x8800;
            signed_tiles = 1;
        }
        else {
            tile_base = 0x8000;
            signed_tiles = 0;
        }

        //set tile map display region
        if(!(lcdc & 0x8))
            start = 0x9800;
        else
            start = 0x9C00;
        
        //TODO:set sprite size
        //TODO:set sprite display enable
        //TODO:set background display

        //set the internal color pallete
        unsigned char pallete = mem[BGP];
        color_map[0] = pallete & 0x3;
        color_map[1] = (pallete >> 2) & 0x3;
        color_map[2] = (pallete >> 4) & 0x3;
        color_map[3] = (pallete >> 6) & 0x3;
        
        unsigned char ly = mem[LY];
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

    //update window only after all lines are updated
    //flush_to_screen(mem);
}


/* Features may expand, but currently makes interrupt requests based on state, 
 * and handles actual rendering.
 *
 * Helper function for graphics_main()
 */
void draw(unsigned char mem[]){
    if(mem[LCDC] & (1 << 7)){
        mem[LY] = mem[LY] + 1;
        cyc_count = REWRITE_CYCLES;
        unsigned char cur_line = mem[LY];
        
        if(cur_line == LY_BASE)
            mem[IFFLAG] = mem[IFFLAG] | 1; //set the Vblank interrupt    
        
        if(cur_line > LY_MAX){
            mem[LY] = 0;
            //draw_tile(mem);
            flush_to_screen(mem);
        }
        if(cur_line < LY_BASE)
            draw_tile(mem);
    }
}

/* The main control function for drawing and rendering, which does so conditionally on the internal state 
 * of the gameboy.
 * This function crucially maintains timing for graphic rendering
 */
void graphics_main(unsigned char mem[], unsigned int cycles){
    set_lcd(mem);

    if(mem[LCDC] & (1 << 7))        //keep relative time
        cyc_count -= cycles;

    if (mem[LY] > LY_MAX)           //reset once maximum is reached
        mem[LY] = 0;

    if (cyc_count <= 0)             //write to screen buffer
        draw(mem);
}
/*
 * This method regards the state of the LCD status, including modes.
 * When the lcd switches modes, various interrupts are generated.
 * The generation of interrupts are facilitated here.
 */
void set_lcd(unsigned char mem[]){
    unsigned char lcds      = mem[LCDS];
    unsigned char lcdc      = mem[LCDC];
    
    if(!(lcdc & (0x1<<7))){ //0 => LCD disabled
        mem[LY] = 0;
        lcds = lcds & 0xFC;
        lcds = lcds | 1;
        mem[LCDS] = lcds;
        return;
    }

    unsigned char ly = mem[LY];
    unsigned char cur_mode = lcds & 0x3;
    
    // set mode as vertical blank
    //TODO: add mode tracking for Vblank
    //TODO: add mode tracking & interrupt reqs for other interrupts
    
}

//Exit SDL cleanly
void exit_clean(){
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(fullTexture);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

