#include "MKL05Z4.h"
#include "leds.h"
#include "uart.h"
#include "buzzer.h"
#include "extra.h"
#include "alarm.h"
#include "rtc.h"
#include "datetime.h"
#include "menu.h"

#define BUFFER_SIZE  255


char msg = ' ';
char fullMsg[BUFFER_SIZE];
uint8_t full_msg_flag = 0;
uint16_t i = 0;
uint16_t j = 0;


void UART0_IRQHandler(void){
	full_msg_flag = 0;

	NVIC_ClearPendingIRQ(UART0_IRQn);
	msg = uart_getChar();
	fullMsg[i] = msg;
	i++;
	if (msg == '\r'){
		full_msg_flag = 1;
		j = i;
		i = 0;
	}
	if (msg == 'a')
		stopTheAlarm();
}


void RTC_IRQHandler(void){
	uart_sendStr("ALARM!!! Press <a> to stop the alarm\n\r");
	ringTheAlarm();
}


int main(void){
	date_time_t date;
	date_time_t alarm_date;
	uint32_t rtc_time = 0;
	uint32_t epoch_time;
	uint32_t alarm_epoch;
	
	initLed();
	buzzerInit();
	uart_init();
	rtc_init();
	
	uart_sendStr("Hello!\n\r Please input current date and hour in given format: \n\r<day>/<month>/<year(last 2 dec)>:<hour>/<min>/<sec>\n\r");
	full_msg_flag = 0;
	while (!full_msg_flag);
		
	if ( parse_str_to_date(fullMsg, &date) == 0 )
		uart_sendStr("Wrong format!!!\n\r");
	
	uart_sendStr("\n\r");
	date_time_uart_send_str(&date);
	uart_sendStr("\n\r");
	show_menu();
	
	epoch_time = date_time_to_epoch(&date);
	
	rtc_write(epoch_time);
	
	rtc_time = rtc_read();
	while(1){
		if (fullMsg[0] == 't'){
			if (rtc_read() > rtc_time){
			rtc_time = rtc_read();
			epoch_to_date_time(&date, rtc_read());
			date_time_uart_send_str(&date);
			}
		}

		if (fullMsg[0] == 's'){
			uart_sendStr("\n\rPlease input alarm date and hour in given format: \n\r<day>/<month>/<year(last 2 dec)>:<hour>/<min>/<sec>\n\r");
			full_msg_flag = 0;
			while (!full_msg_flag);

			if ( parse_str_to_date(fullMsg, &alarm_date) == 0 )
				uart_sendStr("Wrong format!!!\r\n");
			
			alarm_epoch = date_time_to_epoch(&alarm_date);
			
			if (rtc_read() >= alarm_epoch){
				uart_sendStr("Wrong date!");
			} else {
				rtc_set_alarm(alarm_epoch);
				uart_sendStr("Alarm has been set!");
			}
		
			
		}
		
		if (fullMsg[0] == 'a'){
			stopTheAlarm();
		}
		
		/*
		else if (msg != ' '){
			for (int k = 0; k < j; k++){
				uart_sendCh(fullMsg[k]);
			}
		}
		*/
	}
}
