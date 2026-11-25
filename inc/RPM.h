#include <stdbool.h>
#include <stdint.h>
#include <xc.h>  
#include "lcd_display.h"

#define NUM_CYCLES      12
#define NUM_PULSES      10      // puede cambiar de acuerdo a la cantidad de imanes que se coloquen
#define DIAM_RUEDA_MM   698.5      // Cambiar de acuerdo a la distancia de los imanes

void AlternateFlag(uint8_t* counter, bool* flag);
bool RPM_ShowMenu(uint16_t* counter_encoder);