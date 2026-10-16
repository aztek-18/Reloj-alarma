#include <xc.h>
#include <stdint.h>
#include "config_fuses.h"
#include "i2c.h"

#define DISPLAY_ADDR 0x27

void setup()
{
    // define the time of the Oscilation to 4MHz
    OSCCON = 0b01100001;

    // Disables all the analog inputs and converts all the pins to digital
    ANSEL = 0x00;
    ANSELH = 0x00;

    // PORT D configuration
    TRISD = 0x00;
    PORTD = 0XFF;

    // Configuration of the i2c
    i2c_init();
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