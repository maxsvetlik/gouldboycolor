/* Time manager for the gouldboy color
 *
 * Notes on Timer control. The internal byte layout of TAC is as such: 
 *  Bit 2    - Timer Stop  (0=Stop, 1=Start)
 *  Bits 1-0 - Input Clock Select
             00:   4096 Hz    (~4194 Hz SGB)
             01: 262144 Hz  (~268400 Hz SGB)
             10:  65536 Hz   (~67110 Hz SGB)
             11:  16384 Hz   (~16780 Hz SGB)
 *
 */

#define BASEFREQ 262144

//registers
#define DIV     0xFF04  //divider register.
#define TIMA    0xFF05  //Timer counter
#define TMA     0xFF06  //Timer Modulor. Holds value that gets loaded to TIME when overflowed
#define TAC     0xFF07  //Timer Control. Sets the timer mode

unsigned int cyc_div(unsigned int cpu_freq);
void tick(unsigned char cycles_executed);
