#include "menu.h"
#include "uart.h"


void show_menu(uint32_t is_alarm_set){
	uart_sendStr("Press <s> to set time for alarm\r\n");
	uart_sendStr("Press <t> to show current date and time\r\n");
	
	if (is_alarm_set != 0){
		uart_sendStr("Press <c> to check alarm date\r\n");
		uart_sendStr("Press <r> to delete existing alarm\r\n");
	}
	
}
