
/* ===== DS1302: pines ===== */
#include "dst_rtc.h"


void DS1302_Init(void)
{
    RTC_CE_TRIS = 0;
    RTC_SCLK_TRIS = 0;
    RTC_IO_TRIS = 0;

    RTC_CE = 0;
    RTC_SCLK = 0;
    RTC_IO = 0;
}

static void DS1302_WriteByte(uint8_t data)
{
    uint8_t i;

    RTC_IO_TRIS = 0;

    for (i = 0; i < 8; i++)
    {
        RTC_SCLK = 0;
        RTC_IO = (data >> i) & 0x01;
        __delay_us(2);

        RTC_SCLK = 1;
        __delay_us(2);
    }

    RTC_SCLK = 0;
}


static uint8_t DS1302_ReadByte(void)
{
    uint8_t i;
    uint8_t data = 0;

    // El PIC libera la línea DAT para que el RTC transmita.
    RTC_IO_TRIS = 1;

    for (i = 0; i < 8; i++)
    {
        // Leer primero el bit que el RTC presenta.
        if (RTC_IO)
            data |= (uint8_t)(1u << i);

        // Preparar el siguiente bit.
        RTC_SCLK = 1;
        __delay_us(2);

        RTC_SCLK = 0;
        __delay_us(2);
    }

    // Recuperar el control de DAT.
    RTC_IO_TRIS = 0;

    return data;
}


static void DS1302_WriteRegister(uint8_t command,
                                 uint8_t data)
{
    RTC_CE = 0;
    RTC_SCLK = 0;
    RTC_CE = 1;

    DS1302_WriteByte(command); // Comando de escritura
    DS1302_WriteByte(data);

    RTC_CE = 0;
}

static uint8_t DS1302_ReadRegister(uint8_t command)
{
    uint8_t data;

    RTC_CE = 0;
    RTC_SCLK = 0;
    RTC_CE = 1;

    DS1302_WriteByte(command | 0x01); // Lectura

    data = DS1302_ReadByte();

    RTC_CE = 0;
    RTC_SCLK = 0;
    RTC_IO_TRIS = 0;

    return data;
}

static uint8_t RTC_DecToBCD(uint8_t value)
{
    return (uint8_t)(((value / 10) << 4) |
                     (value % 10));
}

static uint8_t RTC_BCDToDec(uint8_t value)
{
    return (uint8_t)(((value >> 4) * 10) +
                     (value & 0x0F));
}

void RTC_ReadDateTime(RTC_DateTime *rtc)
{
    rtc->second = RTC_BCDToDec(
        DS1302_ReadRegister(0x80 | 0x01) & 0x7F);

    rtc->minute = RTC_BCDToDec(
        DS1302_ReadRegister(0x82 | 0x01) & 0x7F);

    // Máscara para el formato de 24 horas
    rtc->hour = RTC_BCDToDec(
        DS1302_ReadRegister(0x84 | 0x01) & 0x3F);

    rtc->day = RTC_BCDToDec(
        DS1302_ReadRegister(0x86 | 0x01) & 0x3F);

    rtc->month = RTC_BCDToDec(
        DS1302_ReadRegister(0x88 | 0x01) & 0x1F);

    rtc->year = RTC_BCDToDec(
        DS1302_ReadRegister(0x8C | 0x01));
}

void RTC_SetDateTime(RTC_DateTime rtc)
{
    // Desactivar la protección de escritura
    DS1302_WriteRegister(0x8E, 0x00);

    // Escribir hora y fecha en formato BCD
    DS1302_WriteRegister(0x80, RTC_DecToBCD(rtc.second));
    DS1302_WriteRegister(0x82, RTC_DecToBCD(rtc.minute));
    DS1302_WriteRegister(0x84, RTC_DecToBCD(rtc.hour));
    DS1302_WriteRegister(0x86, RTC_DecToBCD(rtc.day));
    DS1302_WriteRegister(0x88, RTC_DecToBCD(rtc.month));

    // Día de la semana: 1 a 7
    DS1302_WriteRegister(0x8A, 0x01);

    // Se almacena el año con dos dígitos: 00 a 99
    DS1302_WriteRegister(0x8C, RTC_DecToBCD(rtc.year));

    // Protección de escritura activada
    DS1302_WriteRegister(0x8E, 0x80);
}


static void RTC_TwoDigits(char *dest, uint8_t value)
{
    dest[0] = (char)('0' + (value / 10));
    dest[1] = (char)('0' + (value % 10));
}

bool LCD_ShowDateTime(const RTC_DateTime *rtc)
{
    char dateLine[] = "Fecha: 00/00/00";
    char timeLine[] = "Hora: 00:00:00";

    RTC_TwoDigits(&dateLine[7], rtc->day);
    RTC_TwoDigits(&dateLine[10], rtc->month);
    RTC_TwoDigits(&dateLine[13], rtc->year);

    RTC_TwoDigits(&timeLine[6], rtc->hour);
    RTC_TwoDigits(&timeLine[9], rtc->minute);
    RTC_TwoDigits(&timeLine[12], rtc->second);

    /*
     * Primera línea
     */
    if (!PCF_Start())
        return false;

    if (!LCD_SendByte_Bus(0x80, false))
    {
        (void)PCF_Stop();
        return false;
    }

    if (!LCD_Puts_Bus(dateLine))
    {
        (void)PCF_Stop();
        return false;
    }

    if (!PCF_Stop())
        return false;

    /*
     * Segunda línea
     */
    if (!PCF_Start())
        return false;

    if (!LCD_SendByte_Bus(0xC0, false))
    {
        (void)PCF_Stop();
        return false;
    }

    if (!LCD_Puts_Bus(timeLine))
    {
        (void)PCF_Stop();
        return false;
    }

    if (!PCF_Stop())
        return false;

    return true;
}