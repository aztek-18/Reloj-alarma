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

static void RTC_TwoDigits(char *dest, uint8_t value)
{
    dest[0] = (char)('0' + (value / 10));
    dest[1] = (char)('0' + (value % 10));
}

bool RPM_ShowMenu(void)
{
    char velLine[] = "    00.00 km/h";
    char batStatus[] = "    99% per";

    RTC_TwoDigits(&velLine[4], 10);
    RTC_TwoDigits(&velLine[7], 50);
    RTC_TwoDigits(&batStatus[4], 99);

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

    if (!LCD_Puts_Bus(velLine))
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

    if (!LCD_Puts_Bus(batStatus))
    {
        (void)PCF_Stop();
        return false;
    }

    if (!PCF_Stop())
        return false;

    return true;
}

