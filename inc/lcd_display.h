/*
    This library is for using the lcd 16x2 connected to the i2c module.
*/

#include "i2c.h"
#define lcd_addr 0x27
// Bits del adaptador PCF8574 (asignación habitual)
#define LCD_RS 0x01
#define LCD_RW 0x02
#define LCD_EN 0x04
#define LCD_BL 0x08

bool LCD_Init(void);
bool LCD_SetCursor(uint8_t row, uint8_t column);
bool LCD_Puts(const char *text);
