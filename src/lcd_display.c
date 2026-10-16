#include "lcd_display.h"


bool PCF_Write(uint8_t value)
{
    if (!i2c_start())
        return false;

    if (!i2c_write_address(lcd_addr))
    {
        (void)i2c_stop();
        return false;
    }

    if (!i2c_write_byte(value))
    {
        (void)i2c_stop();
        return false;
    }

    return i2c_stop();
}

bool LCD_Write4Bits(uint8_t nibble, bool rs)
{
    uint8_t value;

    value = (uint8_t)((nibble & 0x0F) << 4);
    value |= LCD_BL;

    if (rs)
        value |= LCD_RS;

    // Pulso de habilitación E
    if (!PCF_Write(value | LCD_EN))
        return false;

    __delay_us(1);

    if (!PCF_Write(value))
        return false;

    __delay_us(50);
    return true;
}

bool LCD_SendByte(uint8_t value, bool rs)
{
    if (!LCD_Write4Bits(value >> 4, rs))
        return false;

    if (!LCD_Write4Bits(value & 0x0F, rs))
        return false;

    return true;
}

bool LCD_Command(uint8_t command)
{
    if (!LCD_SendByte(command, false))
        return false;

    // Clear y Return Home requieren más tiempo
    if (command == 0x01 || command == 0x02)
        __delay_ms(2);

    return true;
}

bool LCD_Char(char character)
{
    return LCD_SendByte((uint8_t)character, true);
}

bool LCD_Puts(const char *text)
{
    while (*text != '\0')
    {
        if (!LCD_Char(*text++))
            return false;
    }

    return true;
}

bool LCD_SetCursor(uint8_t row, uint8_t column)
{
    uint8_t address;

    if (column > 15 || row > 1)
        return false;

    // Direcciones DDRAM para LCD 16x2
    address = (row == 0) ? column : (uint8_t)(0x40 + column);

    return LCD_Command(0x80 | address);
}

bool LCD_Init()
{
    __delay_ms(50);

    if (!PCF_Write(LCD_BL))
        return false;

    // Secuencia inicial recomendada para modo de 4 bits
    if (!LCD_Write4Bits(0x03, false))
        return false;
    __delay_ms(5);

    if (!LCD_Write4Bits(0x03, false))
        return false;
    __delay_us(150);

    if (!LCD_Write4Bits(0x03, false))
        return false;
    __delay_us(150);

    if (!LCD_Write4Bits(0x02, false))
        return false;

    // 4 bits, 2 líneas, fuente 5x8
    if (!LCD_Command(0x28))
        return false;

    // Display encendido, cursor apagado
    if (!LCD_Command(0x0C))
        return false;

    // Avanzar cursor después de cada carácter
    if (!LCD_Command(0x06))
        return false;

    // Limpiar pantalla
    if (!LCD_Command(0x01))
        return false;

    return true;
}