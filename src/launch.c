/* Orchestrates the launching of everything including CPU, 
 * graphical interface, sound (maybe), etc.
 *
 * Author Maxwell J Svetlik
 */

#include "cpu_test.h"
#include "error.h"
#include "visualization.h"
#include "z80.h"

int main(int argc, char** argv){
    
    cpu_init();
    init_visualization();
    interactive_session();
    return 0;

}
