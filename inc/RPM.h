#include <stdbool.h>
#include <stdint.h>
#include <xc.h>  
#include "lcd_display.h"

#define NUM_CYCLES 12

void AlternateFlag(uint8_t* counter, bool* flag);
bool RPM_ShowMenu(void);