#include "datetime.h"
#include "uart.h"

unsigned int date_time_to_epoch(date_time_t* date_time)
{
    unsigned int second = date_time->second;  // 0-59
    unsigned int minute = date_time->minute;  // 0-59
    unsigned int hour   = date_time->hour;    // 0-23
    unsigned int day    = date_time->day-1;   // 0-30
    unsigned int month  = date_time->month-1; // 0-11
    unsigned int year   = date_time->year;    // 0-99
    return (((year/4*(365*4+1)+days[year%4][month]+day)*24+hour)*60+minute)*60+second;
}


void epoch_to_date_time(date_time_t* date_time, unsigned int epoch)
{
	date_time->second = epoch%60;
	epoch /= 60;
	date_time->minute = epoch%60;
	epoch /= 60;
	date_time->hour   = epoch%24;
	epoch /= 24;

    unsigned int years = epoch/(365*4+1)*4; epoch %= 365*4+1;

    unsigned int year;
    for (year=3; year>0; year--)
    {
        if (epoch >= days[year][0])
            break;
    }

    unsigned int month;
    for (month=11; month>0; month--)
    {
        if (epoch >= days[year][month])
            break;
    }

    date_time->year  = years+year;
    date_time->month = month+1;
    date_time->day   = epoch-days[year][month]+1;
}

static char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

void date_time_uart_send_str(date_time_t* date_time) {
	char buffer[255];

	uart_sendStr("Current date: ");
	itoa(date_time->year, buffer, 10);
	uart_sendStr(buffer);
	uart_sendCh('/');
	itoa(date_time->month, buffer, 10);
	uart_sendStr(buffer);
	uart_sendCh('/');
	itoa(date_time->day, buffer, 10);
	uart_sendStr(buffer);
	uart_sendCh(' ');
	itoa(date_time->hour, buffer, 10);
	uart_sendStr(buffer);
	uart_sendCh(':');
	itoa(date_time->minute, buffer, 10);
	uart_sendStr(buffer);
	uart_sendCh(':');
	itoa(date_time->second, buffer, 10);
	uart_sendStr(buffer);
	uart_sendCh('\r');
}

static uint8_t find_number(uint8_t ch){
    switch (ch)
    {
        case 48u:	return 0;
        case 49u:	return 1;
        case 50u:	return 2;
        case 51u:	return 3;
        case 52u:	return 4;
        case 53u:	return 5;
        case 54u:	return 6;
        case 55u:	return 7;
        case 56u:	return 8;
        case 57u:	return 9;

        default:
            return 101;
    }
}

uint8_t parse_str_to_date(char *str, date_time_t * date_buf){
    //parses str "<day>/<month>/<year(last 2 dec)>:<hour>/<min>/<sec> to date_time_t struct
    uint16_t day = 0;
    uint16_t month = 0;
    uint16_t year = 0;
    uint16_t hour = 0;
    uint16_t min = 0;
    uint16_t sec = 0;

    day = find_number(str[0]) * 10;
		day += find_number(str[1]);
	
    if (day > 31)
        return 0;
		
    month = find_number(str[3]) * 10;
		month += find_number(str[4]);
		
    if (month > 12)
        return 0;
		
    year = find_number(str[6]) * 10;
		year += find_number(str[7]);
		
    if (year > 99)
        return 0;

    hour = find_number(str[9]) * 10;
		hour += find_number(str[10]);
		
    if (hour > 23)
        return 0;
		
    min = find_number(str[12]) * 10;
		min	+= find_number(str[13]);
		
    if (min > 59)
        return 0;
		
    sec = find_number(str[15]) * 10;
		sec += find_number(str[16]);
		
    if (sec > 59)
        return 0;

    date_buf->day = day;
    date_buf->month = month;
    date_buf->year = year;
    date_buf->hour = hour;
    date_buf->minute = min;
    date_buf->second = sec;

    return 1;
}
