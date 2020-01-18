#include "rtc.h"
#include "MKL05Z4.h"
#include "uart.h"
#include "datetime.h"

#define CLEAR_ALARM_FLAG 4294967295


void rtc_init(void){
	SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;

	//OSCERCLK ~32kHz
	SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK;
	SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(0);
	
	RTC->TSR = 1;
	
	//Time Compensation Register
	// disable LR
	RTC->LR &= ~RTC_LR_LRL_MASK;
	RTC->TCR = RTC_TCR_CIR(0) | RTC_TCR_TCR(0);

	// enable LR
	RTC->LR |= RTC_LR_TCL_MASK;
	RTC->LR |= RTC_LR_LRL_MASK;
	
	// enable alarm interrupt
	RTC->IER = RTC_IER_TAIE_MASK;

	// enable rtc
	RTC->SR |= RTC_SR_TCE_MASK;
		
	NVIC_ClearPendingIRQ(RTC_IRQn);
	NVIC_SetPriority(RTC_IRQn, 10);
	NVIC_EnableIRQ(RTC_IRQn);
}

void rtc_set_alarm(uint32_t time){
	if (time != CLEAR_ALARM_FLAG){
		date_time_t date;
		uart_sendStr("\r\nAlarm has been set with time: ");
		epoch_to_date_time(&date, time);
		date_time_uart_send_str(&date);
	}
	
	RTC->TAR = time;
}

uint32_t rtc_read(void) {
	return RTC->TSR;
}

void rtc_write(uint32_t t) {
	// disable rtc
	RTC->SR &= ~RTC_SR_TCE_MASK;

	if (t == 0)
	t = 1;
	
	RTC->TSR = t;

	RTC->SR |= RTC_SR_TCE_MASK;
}
