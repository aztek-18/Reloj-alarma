#include "config_fuses.h"
#include <xc.h>

void setup()
{
    // Puerto B como salidas e iniciandolo como 1s.
    TRISB = 0x00;
    PORTB = 0XFF;

    return;
}

int main(void) {
    setup();
    
    while(1) 
    {
        PORTB = !PORTB;
        __delay_ms(1000); // Retardo de 500ms
    }
    return 0;
}   