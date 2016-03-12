#define BUFFSIZE    32
#define DEBUG       1
unsigned short int getint();
unsigned char get_state_hex();
unsigned char get_state_input();
char get_menu_input();
void print_mem(int base, int num);
void print_state();
void print_global_menu();
void print_set_state_menu();
void handle_set_mem();
void handle_set_reg();
void interactive_session();
