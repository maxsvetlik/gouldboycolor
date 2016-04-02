/* Specific visualization definitions for SDL library handling
 * 
 */
#ifndef VIS_HEADER_
#define VIS_HEADER_
#if defined (__cplusplus)
extern "C" {
#endif

#include <SDL2/SDL.h>

void initilize_colors(SDL_Surface);
Uint32 get_pixel32(SDL_Surface, int, int);
Uint32 put_pixel32(SDL_Surface, int, int, Uint32);
void draw_line(int[], int, int);

extern void init_visualization();
extern void exit_clean();
extern void draw_tile(unsigned char mem[]);

#if defined (__cplusplus)
}
#endif
#endif
