/* The video memory mapping definitions, registers, etc.
 *
 * REGISTERS:
 * LCDC - LCD control. R/W. Looks like:
 *  Bit 7 - LCD Display Enable             (0=Off, 1=On)
 *  Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
 *  Bit 5 - Window Display Enable          (0=Off, 1=On)
 *  Bit 4 - BG & Window Tile Data Select   (0=8800-97FF, 1=8000-8FFF)
 *  Bit 3 - BG Tile Map Display Select     (0=9800-9BFF, 1=9C00-9FFF)
 *  Bit 2 - OBJ (Sprite) Size              (0=8x8, 1=8x16)
 *  Bit 1 - OBJ (Sprite) Display Enable    (0=Off, 1=On)
 *  Bit 0 - BG Display (for CGB see below) (0=Off, 1=On)

 * STAT - LCD status. R/W Looks like:
    Bit 6 - LYC=LY Coincidence Interrupt (1=Enable) (Read/Write)
    Bit 5 - Mode 2 OAM Interrupt         (1=Enable) (Read/Write)
    Bit 4 - Mode 1 V-Blank Interrupt     (1=Enable) (Read/Write)
    Bit 3 - Mode 0 H-Blank Interrupt     (1=Enable) (Read/Write)
    Bit 2 - Coincidence Flag  (0:LYC<>LY, 1:LYC=LY) (Read Only)
    Bit 1-0 - Mode Flag       (Mode 0-3, see below) (Read Only)
           0: During H-Blank
           1: During V-Blank
           2: During Searching OAM
           3: During Transferring Data to LCD Driver

* Consult the documentation for Mode explaination.
 */

#define LCDC    0xFF40  //
#define SCY     0xFF42  //LCD Scroll y-coordinate
#define SCX     0xFF43  //LCD Scroll x-coordinate
#define LY      0xFF44  //LCDC Y-coordinate
#define LYC     0xFF45  //LY compare
#define WY      0xFF4A  //Window y position
#define WX      0xFF4B  //Window x position minus 7 (yeah really)

/* LCD Palettes */
    /*Monochrome:
     Bit 7-6 - Shade for Color Number 3
     Bit 5-4 - Shade for Color Number 2
     Bit 3-2 - Shade for Color Number 1
     Bit 1-0 - Shade for Color Number 0
     Non-CGB mode only */
#define WHITE   0
#define LGRAY   1
#define DGRAY   2
#define BLACK   3
#define BGP     0xFF47  //BG pallete data, rw, assigns gray shades
#define OBP0    0xFF48  //Object palette 0. same as BGP BUT lower two bits not used since 00 -> transparent
#define 0BP1    0xFF49  //Object palette 1. read above about transparency.

    /*Color palettes*/

/*DMA TRANSFERS*/
#define DMA     0xFF46



