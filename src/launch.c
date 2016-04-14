/* Orchestrates the launching of everything including CPU, 
 * graphical interface, sound (maybe), etc.
 *
 * Author Maxwell J Svetlik
 */

#include "cpu_test.h"
#include "error.h"
#include "visualization.h"
#include "z80.h"
#include <signal.h>

//ctr-c handler
static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}

int main(int argc, char** argv){
    signal(SIGINT, intHandler);    
    init_visualization();
    cpu_init();
    interactive_session();
    return 0;
}
