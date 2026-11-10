#include <xc.h>
#include "lcd_display.h"

#define RTC_CE   PORTAbits.RA0
#define RTC_SCLK PORTAbits.RA1
#define RTC_IO   PORTAbits.RA2

#define RTC_CE_TRIS   TRISAbits.TRISA0
#define RTC_SCLK_TRIS TRISAbits.TRISA1
#define RTC_IO_TRIS   TRISAbits.TRISA2

typedef struct
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} RTC_DateTime;

void DS1302_Init(void);

void RTC_ReadDateTime(RTC_DateTime *rtc);

void RTC_SetDateTime(RTC_DateTime rtc);

bool LCD_ShowDateTime(const RTC_DateTime *rtc);