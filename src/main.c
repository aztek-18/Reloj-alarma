#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config_fuses.h"
#include "i2c.h"
#include "lcd_display.h"
#include "dst_rtc.h"
#include "RPM.h"

// #define RTC_SET_ON_STARTUP
volatile bool bInterrupcionMenu         = 0;
volatile uint8_t counter_menu           = 0;
volatile uint16_t counter_encoder       = 0;
volatile uint16_t pulses_1s             = 0;
volatile bool flag_1s                   = 0;

void __interrupt() ISR(void);

int main()
{
    RTC_DateTime rtc;

    // define the time of the Oscilation to 4MHz
    OSCCON = 0b01100001;

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

    LCD_SetCursor(0, 8);
    LCD_Puts("...");
    __delay_ms(500);
    LCD_Clean();

    // Disables all the analog inputs and converts all the pins to digital
    ANSEL                       = 0x00;
    ANSELH                      = 0x00;

    // PORT D configuration
    TRISDbits.TRISD1            = 0;
    TRISDbits.TRISD5            = 0;
    PORTDbits.RD1               = 0;
    PORTDbits.RD5               = 0;
    
    // Configuración de botones como selectores
    TRISDbits.TRISD7            = 1;
    TRISDbits.TRISD6            = 1;

    // Habilitar la interrupción externa en el pin RB0
    TRISBbits.TRISB0            = 1;            // RB0 como entrada
    OPTION_REGbits.INTEDG       = 0;            // Flanco descendiente
    INTCONbits.INTF             = 0;            // bandera de interrupción
    INTCONbits.INTE             = 1;            // habilita la interrupción externa de RB0
    INTCONbits.GIE              = 1;            // habilita las interrupciones globales

    // Habilitar la interrupción externa en el pin RB4
    TRISBbits.TRISB4            = 1;            // RB4 como entrada
    IOCBbits.IOCB4              = 1;            // Indicar qué pin se usará como interruptor por cambio de flanco [RB4 - RB7]
    INTCONbits.RBIE             = 1;            // Habilita la configuración por cambio de flanco en el puerto B
    INTCONbits.RBIF             = 0;            // Limpiar la bandera de la interrupción en el puerto B

    // Configuración del Watchdog por software
    WDTCONbits.WDTPS            = 0b0011;       // ~2 s
    WDTCONbits.SWDTEN           = 1;            // Habilita WDT

    // Configuración del Timer1 para el cálculo de la velocidad (1s)
    T1CONbits.TMR1ON            = 0;            // Timer1 apagado mientras configuramos
    T1CONbits.TMR1CS            = 0;            // Fuente de reloj: FOSC/4
    T1CONbits.T1CKPS            = 0b11;         // Prescaler 1:8
    T1CONbits.T1OSCEN           = 0;            // Oscilador dedicado de Timer1 apagado
    T1CONbits.T1SYNC            = 0;            // Sincronización no importa usando reloj interno
    TMR1H                       = 0x0B;         // Cargar Timer1 para ~500 ms (HIGH)
    TMR1L                       = 0xDC;         // Cargar Timer1 para ~500 ms (LOW)
    PIR1bits.TMR1IF             = 0;            // Limpiar bandera
    PIE1bits.TMR1IE             = 1;            // Habilitar interrupción de Timer1    
    INTCONbits.PEIE             = 1;            // Habilitar interrupciones periféricas
    T1CONbits.TMR1ON            = 1;            // Encender Timer1

    // Inicializar comunicación con DS1302
    DS1302_Init();

#ifdef RTC_SET_ON_STARTUP
    // AJUSTA estos valores a tu fecha y hora.
    // Activa esta opción solo para ajustar el RTC.
    rtc.second = 0;
    rtc.minute = 21;
    rtc.hour   = 1;
    rtc.day    = 22;
    rtc.month  = 11;
    rtc.year   = 26;

    RTC_SetDateTime(rtc);
#endif //RTC_SET_ON_STARTUP
    PORTDbits.RD1 = 0;

    uint8_t alternate_counter   = 0;
    bool bAlternateMenu         = false;

    while(1)
    {
        // Activar el menu de configuracion
        if(bInterrupcionMenu)
        {
            __delay_ms(200);
            bool bandera_salida                 = false;
            uint8_t counter_menu_aux            = counter_menu;
            uint8_t counter_menu_scroll         = 0;
            uint8_t counter_menu_scroll_prev    = 1;
            static char option0[] = "1. Hora";
            static char option1[] = "2. Minutos";
            static char option2[] = "3. Dia";
            static char option3[] = "4. Mes";
            static char option4[] = "5. Year";
            static char option5[] = "6. Exit";
            
            char buffer[10];

            while(!bandera_salida)
            {
                PORTDbits.RD1 = !PORTDbits.RD1;
                __delay_ms(200);
                // detecta si hay un cambio en el botón de menú
                if(counter_menu_aux != counter_menu)
                {
                    __delay_ms(200);
                    counter_menu_aux = counter_menu;
                    
                    if(0 == counter_menu_scroll)
                    {
                        bool bandera_salida_menu_in         = false;
                        uint8_t hora                        = 30;
                        char buffer[10];
                        while(!bandera_salida_menu_in)
                        {
                            if(hora != rtc.hour)
                            {
                                if(30 == hora)
                                    hora = 0;
                                LCD_Clean();
                                LCD_SetCursor(0,4);
                                LCD_Puts("Hora:");
                                LCD_SetCursor(1,4);
                                sprintf(buffer, "%d", hora);
                                LCD_Puts(buffer);
                                
                                rtc.hour = hora;
                            }

                            if(PORTDbits.RD7)
                            {
                                __delay_ms(200);
                                hora++;
                                if(23 < hora)
                                    hora = 0;
                            }
                            if(PORTDbits.RD6)
                            {
                                __delay_ms(200);
                                if(0 == hora)
                                    hora = 23;
                                else
                                    hora--;
                            }

                            if(counter_menu_aux != counter_menu)
                            {
                                __delay_ms(200);
                                counter_menu_aux = counter_menu;
                                RTC_SetDateTime(rtc);
                                bandera_salida_menu_in = true;
                            }
                            CLRWDT();
                        }
                    }
                    else if(1 == counter_menu_scroll)
                    {
                        bool bandera_salida_menu_in         = false;
                        uint8_t minute                        = 70;
                        char buffer[10];
                        while(!bandera_salida_menu_in)
                        {
                            if(minute != rtc.minute)
                            {
                                if(70 == minute)
                                    minute = 0;
                                LCD_Clean();
                                LCD_SetCursor(0,4);
                                LCD_Puts("Minute:");
                                LCD_SetCursor(1,4);
                                sprintf(buffer, "%d", minute);
                                LCD_Puts(buffer);
                                
                                rtc.minute = minute;
                            }

                            if(PORTDbits.RD7)
                            {
                                __delay_ms(200);
                                minute++;
                                if(59 < minute)
                                    minute = 0;
                            }
                            if(PORTDbits.RD6)
                            {
                                __delay_ms(200);
                                if(0 == minute)
                                    minute = 59;
                                else
                                    minute--;
                            }

                            if(counter_menu_aux != counter_menu)
                            {
                                __delay_ms(200);
                                counter_menu_aux = counter_menu;
                                RTC_SetDateTime(rtc);
                                bandera_salida_menu_in = true;
                            }
                            CLRWDT();
                        }
                    }
                    else if(2 == counter_menu_scroll)
                    {
                        bool bandera_salida_menu_in         = false;
                        uint8_t day                        = 32;
                        char buffer[10];
                        while(!bandera_salida_menu_in)
                        {
                            if(day != rtc.day)
                            {
                                if(32 == day)
                                    day = 1;
                                LCD_Clean();
                                LCD_SetCursor(0,4);
                                LCD_Puts("Day:");
                                LCD_SetCursor(1,4);
                                sprintf(buffer, "%d", day);
                                LCD_Puts(buffer);
                                
                                rtc.day = day;
                            }

                            if(PORTDbits.RD7)
                            {
                                __delay_ms(200);
                                day++;
                                if(31 < day)
                                    day = 1;
                            }
                            if(PORTDbits.RD6)
                            {
                                __delay_ms(200);
                                if(1 == day)
                                    day = 31;
                                else
                                    day--;
                            }

                            if(counter_menu_aux != counter_menu)
                            {
                                __delay_ms(200);
                                counter_menu_aux = counter_menu;
                                RTC_SetDateTime(rtc);
                                bandera_salida_menu_in = true;
                            }
                            CLRWDT();
                        }
                    }
                    else if(3 == counter_menu_scroll)
                    {
                        bool bandera_salida_menu_in         = false;
                        uint8_t month                        = 13;
                        char buffer[10];
                        while(!bandera_salida_menu_in)
                        {
                            if(month != rtc.month)
                            {
                                if(13 == month)
                                    month = 1;
                                LCD_Clean();
                                LCD_SetCursor(0,4);
                                LCD_Puts("Month:");
                                LCD_SetCursor(1,4);
                                sprintf(buffer, "%d", month);
                                LCD_Puts(buffer);
                                
                                rtc.month = month;
                            }

                            if(PORTDbits.RD7)
                            {
                                __delay_ms(200);
                                month++;
                                if(12 < month)
                                    month = 1;
                            }
                            if(PORTDbits.RD6)
                            {
                                __delay_ms(200);
                                if(1 == month)
                                    month = 12;
                                else
                                    month--;
                            }

                            if(counter_menu_aux != counter_menu)
                            {
                                __delay_ms(200);
                                counter_menu_aux = counter_menu;
                                RTC_SetDateTime(rtc);
                                bandera_salida_menu_in = true;
                            }
                            CLRWDT();
                        }
                    }
                    else if(4 == counter_menu_scroll)
                    {
                        bool bandera_salida_menu_in         = false;
                        uint8_t year                        = 100;
                        char buffer[10];
                        while(!bandera_salida_menu_in)
                        {
                            if(year != rtc.year)
                            {
                                if(100 == year)
                                    year = 26;
                                LCD_Clean();
                                LCD_SetCursor(0,4);
                                LCD_Puts("Year:");
                                LCD_SetCursor(1,4);
                                sprintf(buffer, "%d", year);
                                LCD_Puts(buffer);
                                
                                rtc.year = year;
                            }

                            if(PORTDbits.RD7)
                            {
                                __delay_ms(200);
                                year++;
                                if(99 < year)
                                    year = 26;
                            }
                            if(PORTDbits.RD6)
                            {
                                __delay_ms(200);
                                if(26 == year)
                                    year = 99;
                                else
                                    year--;
                            }

                            if(counter_menu_aux != counter_menu)
                            {
                                __delay_ms(200);
                                counter_menu_aux = counter_menu;
                                RTC_SetDateTime(rtc);
                                bandera_salida_menu_in = true;
                            }
                            CLRWDT();
                        }
                    }
                    else if(5 == counter_menu_scroll)
                    {
                        bandera_salida          = true;
                        counter_menu            = 0;
                        counter_menu_scroll     = 0;
                    }
                }
                // detecta si hay un cambio en los botones de cambio RD7 y RD6
                if(PORTDbits.RD7)
                {
                    __delay_ms(200);
                    counter_menu_scroll++;
                    if(5 < counter_menu_scroll)
                        counter_menu_scroll = 0;
                }
                if(PORTDbits.RD6)
                {
                    __delay_ms(200);
                    if(0 == counter_menu_scroll)
                        counter_menu_scroll = 5;
                    else
                        counter_menu_scroll--;
                }

                if(counter_menu_scroll_prev != counter_menu_scroll)
                {
                    LCD_Clean();
                    LCD_SetCursor(0,0);
                    switch(counter_menu_scroll)
                    {
                        case 0:
                            LCD_Puts(option0);
                            LCD_SetCursor(0,12);
                            LCD_Puts("<<-");
                            LCD_SetCursor(1,0);
                            LCD_Puts(option1);
                            break;
                        case 1:
                            LCD_Puts(option1);
                            LCD_SetCursor(0,12);
                            LCD_Puts("<<-");
                            LCD_SetCursor(1,0);
                            LCD_Puts(option2);
                            break;
                        case 2:
                            LCD_Puts(option2);
                            LCD_SetCursor(0,12);
                            LCD_Puts("<<-");
                            LCD_SetCursor(1,0);
                            LCD_Puts(option3);
                            break;
                        case 3:
                            LCD_Puts(option3);
                            LCD_SetCursor(0,12);
                            LCD_Puts("<<-");
                            LCD_SetCursor(1,0);
                            LCD_Puts(option4);
                            break;
                        case 4:
                            LCD_Puts(option4);
                            LCD_SetCursor(0,12);
                            LCD_Puts("<<-");
                            LCD_SetCursor(1,0);
                            LCD_Puts(option5);
                            break;
                        case 5:
                            LCD_Puts(option5);
                            LCD_SetCursor(0,12);
                            LCD_Puts("<<-");
                            LCD_SetCursor(1,0);
                            LCD_Puts(option0);
                            break;
                        default:
                            LCD_Puts("Error. Exiting");
                            bandera_salida = true;
                            counter_menu = 0;
                    }
                    counter_menu_scroll_prev = counter_menu_scroll;
                }
                CLRWDT();    
            }
            bInterrupcionMenu = false;
            LCD_Clean();
            CLRWDT();
        }

        if(bAlternateMenu)
        {
            // instrucciones para mostrar tanto el RPM como el nivel de bateria (futuro commit)
            if(0 == alternate_counter)
            {
                if (!LCD_Clean())
                    return false;
            }
            
            if(!RPM_ShowMenu(&pulses_1s))
            {
                while(1)
                {
                    __delay_ms(100);
                    PORTDbits.RD1 = !PORTDbits.RD1;
                }
            }
            AlternateFlag(&alternate_counter, &bAlternateMenu);
        }
        else
        {
            if(0 == alternate_counter)
            {
                if (!LCD_Clean())
                    return false;
            }
            RTC_ReadDateTime(&rtc);
            if (!LCD_ShowDateTime(&rtc))
            {
                while(1)
                {
                    __delay_ms(100);
                    PORTDbits.RD1 = !PORTDbits.RD1;
                }
            }
            AlternateFlag(&alternate_counter, &bAlternateMenu);
        }

        PORTDbits.RD1 = !PORTDbits.RD1;
        __delay_ms(200);
        CLRWDT();
    }
    return 0;
}

void __interrupt() ISR(void)
{
    static uint8_t timer1_count = 0;
    // Interrupcion para el encoder (rueda de bici)
    if (INTCONbits.INTF)
    {
        uint8_t temp_portB = PORTB;
        INTCONbits.INTF = 0;
        counter_encoder++;
    }
    // interrupcion para el botón de SELECT (MENU)
    if (INTCONbits.RBIF)
    {
        uint8_t temp_portB = PORTB;
        INTCONbits.RBIF = 0;
        counter_menu++;
        if(200 <= counter_menu)
            counter_menu = 0;
        bInterrupcionMenu = true;
    }
    // Interrupción de Timer1
    if (PIR1bits.TMR1IF)
    {
        PIR1bits.TMR1IF = 0;

        // Recargar Timer1 para otros ~500 ms
        TMR1H = 0x0B;
        TMR1L = 0xDC;

        timer1_count++;

        if (timer1_count >= 2)
        {
            timer1_count = 0;
            flag_1s = 1;
            PORTDbits.RD5 = !PORTDbits.RD5;
            pulses_1s = counter_encoder;
            counter_encoder = 0;
        }
    }
}