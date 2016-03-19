/* Handles all hardware interrupts
 *
 * Timers are typically stored in memory.
 * There are two main registers that handles this: 
 *      IME specifies if interrupts are enabled
 *      IF specifies the interrupts requested by devices
 *
 * IF's map looks like this
 * 0xFF0F  IFflag 
 * Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
 * Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
 * Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
 * Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
 * Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)
 *
 * This also specifies the interrupt priority in the case of collisions.
 * In order for an interrupt to be executed, the IME flag must be set, and the requested interrupt (set via IF) must exist in the IE flag, at location FFFF.
 *
 *
 * Author Maxwell J Svetlik
 */
#define IFFLAG 0xFF0F
#define IEFLAG 0xFFFF

/*the interrupt vector addresses:
    corresponds to the priority defined above:
    VBLANK, LCDSTAT, TIMER, SERIAL, JOYPAD
*/
static const short int interrupt_table[] = {0x0040, 0x0048, 0x0050, 0x0058,0x0060};
unsigned char f_ime;    //interrupt master enable flag

void enable_interrupts();
void disable_interrupts();
void interrupt_handler();

