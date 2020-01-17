#include "menu.h"
#include "uart.h"


void show_menu(void){
	uart_sendStr("Press <s> to set time for alarm \r\n");
	uart_sendStr("Press <t> to show current date and time\r\n");
	//uart_sendStr("Press");
	
}
