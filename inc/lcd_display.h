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

/*
 * @brief Inicializes the LCD display.
 * @return true false [success failed]
*/
bool LCD_Init(void);

/*
 * @brief Sets the initial position of the LCD write
 * @param uint8_t row - indicates the initial row (0 or 1)
          uint8_t column - indicates the column (0 - 15)
 * @return true false [success failed]
*/
bool LCD_SetCursor(uint8_t row, uint8_t column);

/*
 * @brief Sends the information to be printed in the LCD.
 * @param char* text (do not send strings above 15 characters!)
 * @return true false [success failed]
*/
bool LCD_Puts(const char *text);

/*
 * @brief Cleans the LCD
 * @return true false [success failed]
*/
bool LCD_Clean(void);