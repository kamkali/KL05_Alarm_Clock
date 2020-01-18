#include "MKL05Z4.h"
#include "leds.h"
#include "uart.h"
#include "buzzer.h"
#include "extra.h"
#include "alarm.h"
#include "rtc.h"
#include "datetime.h"
#include "menu.h"

#define BUFFER_SIZE  			255
#define SNOOZE_LENGTH			15							// drzemka 15 s
#define SNOOZE_REPEATS 		3								// ilosc powtorzen drzemki
#define CLEAR_ALARM_FLAG 	4294967295			// max uint32 do czyszczenia flagi
#define IDLE							0



char fullMsg[BUFFER_SIZE];
uint8_t full_msg_flag = 0;
uint8_t j = 0;
uint32_t alarm_time;


void UART0_IRQHandler(void){
	NVIC_ClearPendingIRQ(UART0_IRQn);
	char msg;
	static uint8_t drzemka_counter = 0;
	static uint8_t i = 0;
	full_msg_flag = 0;

	msg = uart_getChar();
	fullMsg[i] = msg;
	i++;
	if (msg == '\r'){
		full_msg_flag = 1;
		j = i;
		i = 0;
	}
		if ( msg == 'a' && (RTC->SR & RTC_SR_TAF_MASK) ){
			uart_sendStr("\n\rStopping alarm.\n\r");
			rtc_set_alarm(CLEAR_ALARM_FLAG);
			alarm_time = 0;
			stopTheAlarm();
			show_menu(alarm_time);
		}
		if ( msg == 'd' && (RTC->SR & RTC_SR_TAF_MASK) ){
			if (drzemka_counter < SNOOZE_REPEATS){
				
				alarm_time = rtc_read() + SNOOZE_LENGTH;
				rtc_set_alarm(alarm_time);
				stopTheAlarm();
				
				drzemka_counter++;
				show_menu(alarm_time);
				
			} else{
				uart_sendStr("\n\rNO MORE SNOOZE ALOWED! WAKE UP!!!\n\r");
				uart_sendStr("Press <a> to stop the alarm");
				
				alarm_time = 0;
				drzemka_counter = 0;
			}
		}
}


void RTC_IRQHandler(void){
	uart_sendStr("\n\rALARM!!! Press <a> to stop the alarm or <d> to set snooze\n\r");
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
	
	uart_sendStr("Hello!\n\r Please input current date and hour in given format: \n");
	uart_sendStr("<day>/<month>/<year(last 2 dec)> <hour>:<min>:<sec>\r");
	uart_sendStr("(Example: 17/01/20 16/20:30)\n\r");
	full_msg_flag = 0;
	while (!full_msg_flag);
		
	if ( parse_str_to_date(fullMsg, &date) == 0 )
		uart_sendStr("Wrong format!!!\n\r");
	
	uart_sendStr("\n\r");
	uart_sendStr("Current date: ");
	date_time_uart_send_str(&date);
	uart_sendStr("\n\r");
	show_menu(alarm_time);
	
	epoch_time = date_time_to_epoch(&date);
	
	rtc_write(epoch_time);
	
	rtc_time = rtc_read();
	while(1){
		
		if (fullMsg[0] == 't'){
			if (rtc_read() > rtc_time){
				rtc_time = rtc_read();
				epoch_to_date_time(&date, rtc_read());
				
				uart_sendStr("Current date: ");
				date_time_uart_send_str(&date);
				uart_sendStr("Press <m> for menu\r");
			}
		}
		
		if (fullMsg[0] == 'm'){
			uart_sendStr("\n\r");
			show_menu(alarm_time);
			fullMsg[0] = IDLE;
		}

		if (fullMsg[0] == 's'){
			uart_sendStr("\n\rPlease input alarm date and hour in given format: \n\r<day>/<month>/<year(last 2 dec)>:<hour>/<min>/<sec>\n\r");
			full_msg_flag = 0;
			while (!full_msg_flag);

			if ( parse_str_to_date(fullMsg, &alarm_date) == 0 ){
				uart_sendStr("Wrong format!!!\n\r");
			} else {
				alarm_epoch = date_time_to_epoch(&alarm_date);

				if (rtc_read() >= alarm_epoch){
					uart_sendStr("Wrong date!\n\r");
					fullMsg[0] = IDLE;
				} else {
					alarm_time = alarm_epoch;
					rtc_set_alarm(alarm_epoch);
				}
			}
			show_menu(alarm_time);
			fullMsg[0] = IDLE;
		}
		
		if (fullMsg[0] == 'r'){
			uart_sendStr("Deleting alarm.\n\r");
			rtc_set_alarm(CLEAR_ALARM_FLAG);
			alarm_time = 0;
			show_menu(alarm_time);
			fullMsg[0] = IDLE;
		}
		
		if (fullMsg[0] == 'c'){
			if (alarm_time == 0){
				uart_sendStr("\n\rAlarm is not set.\n\r");
			} else {
				date_time_t alarm_date;
				uart_sendStr("\n\rAlarm is set on: ");
				epoch_to_date_time(&alarm_date, alarm_time);
				date_time_uart_send_str(&alarm_date);
			}
			show_menu(alarm_time);
			fullMsg[0] = IDLE;
		}
		
	}
}
