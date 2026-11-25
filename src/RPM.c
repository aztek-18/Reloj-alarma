#include "RPM.h"

void AlternateFlag(uint8_t* counter, bool* flag)
{
    bool aux_flag = *flag;
    (*counter)++;
    if(NUM_CYCLES == *counter)
    {
        *counter = 0;
        *flag = !aux_flag;
    }
}

static uint16_t CalcVel(uint16_t pulses)
{
    uint32_t velocidad;
    velocidad = (uint32_t)pulses
              * DIAM_RUEDA_MM
              * 31416UL
              * 36UL;

    velocidad /= ((uint32_t)NUM_PULSES * 1000000UL);

    return (uint16_t)velocidad;
}

bool RPM_ShowMenu(uint16_t *counter_encoder)
{
    char velLine[] = "    00.00 km/h";

    uint16_t velocidad;
    uint8_t entero;
    uint8_t decimal;

    velocidad = CalcVel(*counter_encoder);

    entero = velocidad / 100;
    decimal = velocidad % 100;

    velLine[4] = '0' + (entero / 10);
    velLine[5] = '0' + (entero % 10);

    velLine[7] = '0' + (decimal / 10);
    velLine[8] = '0' + (decimal % 10);

    if (!PCF_Start())
        return false;

    if (!LCD_SendByte_Bus(0x80, false))
    {
        PCF_Stop();
        return false;
    }

    if (!LCD_Puts_Bus(velLine))
    {
        PCF_Stop();
        return false;
    }

    if (!PCF_Stop())
        return false;

    return true;
}