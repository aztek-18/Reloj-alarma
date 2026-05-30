/*
Este proyecto es para probar si puedo compilar un archivo .c.
También es para ver si puedo programar directamente desde este visualizador de 
texto un proyecto que no había concluido.
*/

// Configuración de Bits para MPLAB X IDE con XC8
#pragma config FOSC = INTOSCIO // Oscilador Interno
#pragma config WDTE = OFF      // Watchdog Timer Desactivado
#pragma config PWRTE = OFF     // Power-up Timer Desactivado
#pragma config MCLRE = OFF     // RA5/MCLR como entrada digital
#pragma config BOREN = ON      // Brown-out Detect Activado
#pragma config LVP = OFF       // Programación en Alta Tensión
#pragma config CPD = OFF       // Protección de Datos Desactivada
#pragma config CP = OFF        // Protección de Programa Desactivada

#include <xc.h>
#include <PIC16F887.h>

#define _XTAL_FREQ 4000000 // Frecuencia del oscilador interno (4 MHz típico)

int main(void) {
    TRISB = 0x00; // Configura el Puerto B como salidas
    
    while(1) {
        PORTB ^= 0xFF; // Invierte el estado de todos los bits del Puerto B
        __delay_ms(500); // Retardo de 500ms
    }
    return 0;
}   