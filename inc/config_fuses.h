// OSCILATION TIME
#define _XTAL_FREQ 4000000 // Frecuencia del oscilador interno (4 MHz típico)

// CONFIG1

#pragma config FOSC = INTRC_NOCLKOUT        // Oscilador Interno
#pragma config WDTE = OFF                   // Watchdog Timer Desactivado
#pragma config PWRTE = OFF                  // Power-up Timer Desactivado
#pragma config MCLRE = OFF                  // RA5/ MCLR como entrada digital
#pragma config CP = OFF                     // Proteccion de Programa Desactivado
#pragma config CPD = OFF                    // Proteccion de Datos Desactivado
#pragma config BOREN = ON                   // Brown-out Detect Activado
#pragma config IESO = OFF                   // 
#pragma config FCMEN = OFF                  // 
#pragma config LVP = OFF                    // Programacion en Alta Tension

// CONFIG2

#pragma config BOR4V = BOR40V               // 
#pragma config WRT = OFF                    //
