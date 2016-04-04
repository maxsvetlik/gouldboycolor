/* This file is intended to verify correctness of the 
 * hybrid CPU emulated by z80.h
 *
 * Primarily this will enable an interactive session
 * with a user that can specify state and verify any 
 * op.
 *
 * However, later on, written test cases may be used.
 */

#include "visualization.h"
#include "z80.h"
#include "cpu_test.h"

#include <stdlib.h>
#include <stdio.h>

/*uses scanf to retrieve an integer from the user
 * in an interactive session*/
unsigned short int getint(){
    char buf[BUFFSIZE];
    char *p;
    unsigned short int i;
    //check if there is a dangling newline.
    //consume if so, else putit back.
    char check;
    if((check = fgetc(stdin)) != '\n')
        ungetc(check,stdin);  

    if (fgets(buf, sizeof(buf), stdin) != NULL)
        i = strtol(buf, &p, 10);
    
    return i;
}

unsigned char get_state_hex(){
    //unsigned char result;
    //char buff[16];
    unsigned int res;
    scanf("%x", &res);
    //fscanf(result,"%x\n",&buff);
    return (unsigned char) (res & 0xFF);
}
unsigned char get_state_input(){
    unsigned char result = getchar();
    if(result == '\n')
        result = getchar();
    printf("\n");
    return result;
}
char get_menu_input(){
    printf("Enter a number: ");
    char result = getchar();
    if(result == '\n')
        result = getchar();
    printf("\n");
    return result;
}
/*Prints a range in the memory space*/
void print_mem(int base_addr, int num_bytes){
    int i;
    for(i = 0; i < num_bytes; i+=1){
        printf("Mem[%x] : %5s %x\n", i+base_addr, " ", mem[i+base_addr]);
    }
}

/*prints the direct cpu state*/
void print_state(){
    int i;
    printf("***START STATE***\n");
    printf("PC: %x\n", pc);
    printf("sp: %x\n", sp);
    printf("f_z: %x\n", f_z);
    printf("f_s: %x\n", f_s);
    printf("f_hc: %x\n", f_hc);
    printf("f_c: %x\n", f_c);
    for(i = 0; i < GP_REGS; i+=1)
        printf("Reg[%x]: %5s %x\n", i, " ", reg[i]);
    printf("***END STATE***\n\n");
}
void print_global_menu(){
    printf("0 - Exit\n");
    printf("1 - Set State\n");
    printf("2 - Set Memory\n");
    printf("3 - Read State\n");
    printf("4 - Read Memory\n");
    printf("8 - Call bulk cycle\n");
    printf("9 - Call cycle\n");
}
void print_set_state_menu(){
    printf("0 - Back\n");
    printf("1 - Set GP Registers\n");
    printf("2 - Set Flags\n");
    printf("3 - Set Stack Pointer\n");
    printf("4 - Set Program Counter\n");
}
/* This handles the in/output of the 'SET MEM' submenu
 */
void handle_set_mem(){
    char input;
    unsigned int base_addr;
    printf("NOTE: INPUT DATUM IS EXPECTED TO BE IN HEXIDECIMAL FORM. \n");
    printf("INPUT ADDRESS IN DECIMAL\n");
    while(input != 'q'){
        printf("Please enter address: ");
        base_addr = (unsigned) getint();
        if(base_addr > MEM_SIZE){
            printf("ERROR: Memory address outside of possible range.\n");
        } else{
            printf("Old value for mem[%x]: %x or %x\n", base_addr, mem[base_addr], mem[base_addr]);
            printf("New value: ");
            mem[base_addr] = get_state_hex();
            printf("Enter (c) to continue or (q) to quit: ");
            input = get_menu_input(); 
        }
    }
}
/* This handles the in/output of the 'SET STATE' submenu
 */
void handle_set_reg(){
    char input;
    int i;
    print_set_state_menu();
    input = get_menu_input();
    switch(input){
        case('0'):                          break;
        case('1'):
            for(i = 0; i < GP_REGS; i++){
                printf("Current val for Reg[%x]  :  %x\n", i, reg[i]);
                printf("Enter new value: ");
                reg[i] = (get_state_input() - '0');
            }                               break;
        case('2'):
            printf("Not implemented\n.");   break;
        case('3'): 
            printf("Current SP  :  %x\n", sp);
            printf("Enter new value: ");
            sp = (unsigned) getint();       break;
        case('4'):
            printf("Current PC  :  %x\n", pc);
            printf("Enter a new value: ");
            pc = (unsigned) getint();       break;
        default:
            printf("Unsupported option.\n");
    }
}
void bulkcycle(){
    printf("Enter a number: ");
    int result = getint();
    if(result == '\n')
         result = getint();
    printf("\n");
    int i;
    for(i = 0; i < result; i+=1){
        if(cycle())
            exit(-1);
        if(!(i % 1000))
            draw_tile(mem);
    }
}
void interactive_session(){
    char input;
    int val1;
    int val2;
    while(input != '0'){
        print_global_menu();
        input = get_menu_input();
        switch(input){
            case('0'): printf("Exiting.\n");  break;
            case('1'): handle_set_reg();    break;
            case('2'): handle_set_mem();    break;
            case('3'): print_state();       break;
            case('4'): 
                printf("Enter the base address: ");
                val1 = getint();
                printf("Enter the number of bytes to print: ");
                val2 = getint();
                print_mem(val1, val2);
                break;
            case('8'): bulkcycle();         break;
            case('9'): if(cycle()) exit(-1);             break;
            default: printf("Unsupported command, please try again.\n"); 
        }
    }
}
