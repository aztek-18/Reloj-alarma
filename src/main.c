#include <xc.h>
#include <stdint.h>
#include "config_fuses.h"
#include "i2c.h"
#include "lcd_display.h"

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

    // Start of the LCD (i2c)
    if(!LCD_Init())
    {
        while(1)
        {
            PORTDbits.RD1 = !PORTDbits.RD1;
            __delay_ms(100);
        }
    }

    PORTDbits.RD1 = 1;

    // Mostrar mensaje
    LCD_SetCursor(0, 0);
    LCD_Puts("Hola, PIC16F887");

    LCD_SetCursor(1, 0);
    LCD_Puts("I2C funcionando");
}

int main()
{
    setup();

    while(1)
    {
        PORTDbits.RD1 = 1;
        __delay_ms(500);
        PORTDbits.RD1 = 0;
        __delay_ms(500);
    }
    return 0;
}