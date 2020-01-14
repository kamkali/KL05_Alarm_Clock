#include "rtc.h"
#include "MKL05Z4.h"

void rtc_init(void){
	// enable RTC clock
	SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;

	// select RTC clock source
	SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK;
	//Zegar OSCERCLK ~32kHz
	SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(2);
	
	// Configure the TSR. default value: 1
	RTC->TSR = 1;
	
	// Configure Time Compensation Register to calibrate RTC accuracy

	// dissable LRL lock
	RTC->LR &= ~RTC_LR_LRL_MASK;
	// RTC->TCR: RTC_TCR_CIR_MASK,RTC_TCR_CIR(x)=0,RTC_TCR_TCR(x)=0  Default no correction
	RTC->TCR = RTC_TCR_CIR(0) | RTC_TCR_TCR(0);
	/*
        RTC_TCR_CIR(x) sets the compensation interval in seconds from 1 to 256.
        0x05 will apply the compensation once every 4 seconds.
        RTC_TCR_TCR(x) sets the Register Overflow
        0x80 Time Prescaler Register overflows every 32896 clock cycles. (+128)
        ... ... RTC runs slower
        0xFF Time Prescaler Register overflows every 32769 clock cycles.
        0x00 Time Prescaler Register overflows every 32768 clock cycles, Default.
        0x01 Time Prescaler Register overflows every 32767 clock cycles.
        ... ... RTC runs faster
        0x7F Time Prescaler Register overflows every 32641 clock cycles. (-128)
	*/
	// enable TCL lock
	RTC->LR |= RTC_LR_TCL_MASK;
	// enable LRL lock
	RTC->LR |= RTC_LR_LRL_MASK;

	// enable counter
	RTC->SR |= RTC_SR_TCE_MASK;
}

uint32_t rtc_read(void) {
	return RTC->TSR;
}

void rtc_write(uint32_t t) {
	// disable counter
	RTC->SR &= ~RTC_SR_TCE_MASK;

	// we do not write 0 into TSR
	// to avoid invalid time
	if (t == 0)
	t = 1;

	// write seconds
	RTC->TSR = t;

	// re-enable counter
	RTC->SR |= RTC_SR_TCE_MASK;
}
