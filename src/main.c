#include <xc.h>
#include <stdint.h>
#include "config_fuses.h"
#include "i2c.h"
#include "lcd_display.h"
#include "dst_rtc.h"

// #define RTC_SET_ON_STARTUP

int main()
{
    RTC_DateTime rtc;

    // define the time of the Oscilation to 4MHz
    OSCCON = 0b01100001;

    // Disables all the analog inputs and converts all the pins to digital
    ANSEL = 0x00;
    ANSELH = 0x00;

    // PORT D configuration
    TRISDbits.TRISD1 = 0;
    PORTDbits.RD1 = 0;

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
    __delay_ms(1000);
    LCD_Clean();
    // Inicializar comunicación con DS1302
    DS1302_Init();

#ifdef RTC_SET_ON_STARTUP
    // AJUSTA estos valores a tu fecha y hora.
    // Activa esta opción solo para ajustar el RTC.
    rtc.second = 0;
    rtc.minute = 6;
    rtc.hour   = 23;
    rtc.day    = 19;
    rtc.month  = 10;
    rtc.year   = 26;

    RTC_SetDateTime(rtc);
#endif //RTC_SET_ON_STARTUP
    PORTDbits.RD1 = 0;

    while(1)
    {
        RTC_ReadDateTime(&rtc);

        if (!LCD_ShowDateTime(&rtc))
        {
            // Error en LCD/I2C
            PORTDbits.RD1 = 1;

            while(1)
            {
                __delay_ms(100);
                PORTDbits.RD1 = !PORTDbits.RD1;
            }
        }

        PORTDbits.RD1 = !PORTDbits.RD1;

        __delay_ms(200);
    //     LCD_SetCursor(0, 0);
    // LCD_Puts("123456789012345");

    // LCD_SetCursor(1, 0);
    // LCD_Puts("12345678901234");

    PORTDbits.RD1 = !PORTDbits.RD1;

    __delay_ms(200);
    }

    return 0;
}