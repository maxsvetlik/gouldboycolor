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
void set_lcd(unsigned char mem[]);

extern void init_visualization();
extern void exit_clean();
extern void draw_tile(unsigned char mem[]);
extern void flush_to_screen(unsigned char mem[]);
extern void graphics_main(unsigned char mem[], unsigned int cycles);
#if defined (__cplusplus)
}
#endif
#endif
