/* This file will handle (as a minimum) the visualization of the emulator 
 * in the form of interfacing with a user's screen.
 *
 * Author Maxwell J Svetlik
 */

#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 166;
const int SCREEN_HEIGHT = 140;

int main(int argc, char** args){
    SDL_Window* window = NULL;
    
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    } else{
        window = SDL_CreateWindow( "GouldBoyColor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL){
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return -1;
        } 
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        SDL_Texture* texture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING,
            SCREEN_WIDTH,
            SCREEN_HEIGHT
        );
        SDL_Surface* surface = SDL_CreateRGBSurface(
            0,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            32,
            0x00000000,
            0x00000000,
            0x00000000,
            0xff000000
        );
        Uint32 gray = SDL_MapRGBA(surface->format, 0x1E, 0x1E, 0x1E, 0xFF);
        SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

    
        SDL_Delay( 1000 );
        SDL_FreeSurface(surface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow( window );
        SDL_Quit();
    }
    return 0;
}
