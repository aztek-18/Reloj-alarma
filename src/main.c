#include "config_fuses.h"
#include <xc.h>

void setup()
{
    OSCCON = 0b01100001;

    ANSEL = 0x00;
    ANSELH = 0x00;

    TRISD = 0x00;
    PORTD = 0XFF;
}

int main()
{
    setup();
    while(1)
    {
        PORTD = 0XFF;
        __delay_ms(500);
        PORTD = 0X00;
        __delay_ms(500);
    }
    return 0;
}