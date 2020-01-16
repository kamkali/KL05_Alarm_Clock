#ifndef __DATETIME_H__
#define __DATETIME_H__

#include "MKL05Z4.h"


typedef struct
{
    uint16_t second; // 0-59
    uint16_t minute; // 0-59
    uint16_t hour;   // 0-23
    uint16_t day;    // 1-31
    uint16_t month;  // 1-12
    uint16_t year;   // 0-99 (representing 2000-2099)
}date_time_t;

static unsigned short days[4][12] =
        {
                {   0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335},
                { 366, 397, 425, 456, 486, 517, 547, 578, 609, 639, 670, 700},
                { 731, 762, 790, 821, 851, 882, 912, 943, 974,1004,1035,1065},
                {1096,1127,1155,1186,1216,1247,1277,1308,1339,1369,1400,1430},
        };
				
				
unsigned int date_time_to_epoch(date_time_t* date_time);
				
void epoch_to_date_time(date_time_t* date_time, unsigned int epoch);

uint8_t parse_str_to_date(char *str, date_time_t *date_buf);


#endif /* __DATETIME_H__*/
				