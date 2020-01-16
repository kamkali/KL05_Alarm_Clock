#include "MKL05Z4.h"
#include "leds.h"
#include "uart.h"
#include "buzzer.h"
#include "extra.h"
#include "alarm.h"
#include "rtc.h"
#include "datetime.h"

char msg = ' ';
char fullMsg[255];
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
}

void RTC_IRQHandler(void){
	//ringTheAlarm();
}


int main(void){
	date_time_t date;
	uint32_t rtc_time = 0;
	uint32_t epoch_time;
	
	initLed();
	buzzerInit();
	uart_init();
	rtc_init();
	
	uart_sendStr("Hello!\r\n Please input current date and hour is given format: \r\n<day>/<month>/<year(last 2 dec)>:<hour>/<min>/<sec>\r\n");
	while (!full_msg_flag);
	
//	uart_sendStr(fullMsg);
	
	if ( parse_str_to_date(fullMsg, &date) == 0 )
		uart_sendStr("Wrong format!!!\r\n");
	
	date_time_uart_send_str(&date);
	
	epoch_time = date_time_to_epoch(&date);
	
	rtc_write(epoch_time);
	
	rtc_time = rtc_read();
	while(1){
		if (rtc_read() > rtc_time){
			rtc_time = rtc_read();
			epoch_to_date_time(&date, rtc_read());
			date_time_uart_send_str(&date);
		}

		
		//uart_sendCh(rtc_time + 48);
		if(fullMsg[0] == 'g'){
			uart_sendCh(rtc_time);
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
