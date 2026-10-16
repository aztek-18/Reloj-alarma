// This is exclusively for the pic 16f887a.
#include "i2c.h"
#define PIC16F887

#ifdef PIC16F887
void i2c_init()
{
    // Configures the i2c in the pins C3 [SCL] & C4 [SDA]
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;

    // Configuración I2C maestro a 100 kHz con FOSC = 4 MHz
    SSPCON = 0b00101000;  // MSSP habilitado, I2C Master
    SSPCON2 = 0x00;
    SSPSTAT = 0b10000000; // SMP=1: slew rate deshabilitado

    SSPADD = 9;

    // Limpiar banderas de estado
    PIR1bits.SSPIF = 0;
    PIR2bits.BCLIF = 0;
}

bool i2c_scan_address(uint8_t* found_address)
{
    uint8_t address;
    bool ack;

    for (address = 0x08; address <= 0x77; address++)
    {
        if (!i2c_start())
            return false;

        ack = i2c_write_address(address);

        if (!i2c_stop())
            return false;

        if (ack)
        {
            *found_address = address;
            return true;
        }
    }

    return false;
}
// Envía un byte y devuelve true si recibe ACK.
bool i2c_write_byte(uint8_t data)
{
    if (!i2c_wait_idle())
        return false;

    PIR1bits.SSPIF = 0;
    SSPBUF = data;

    if (!i2c_wait_complete())
        return false;

    return (SSPCON2bits.ACKSTAT == 0);
}

// Recibe una dirección de 7 bits, no desplazada.
bool i2c_write_address(uint8_t address)
{
    if (address > 0x7F)
        return false;

    return i2c_write_byte((uint8_t)(address << 1) | 0);
}

static bool i2c_wait_idle(void)
{
    uint32_t timeout = I2C_TIMEOUT;

    while ((SSPCON2 & 0x1F) ||
           SSPSTATbits.R_W ||
           SSPSTATbits.BF)
    {
        if (--timeout == 0)
            return false;
    }

    return true;
}

static bool i2c_wait_complete(void)
{
    uint32_t timeout = I2C_TIMEOUT;

    while (!PIR1bits.SSPIF)
    {
        if (--timeout == 0)
            return false;
    }

    PIR1bits.SSPIF = 0;
    return true;
}

bool i2c_start(void)
{
    if (!i2c_wait_idle())
        return false;

    PIR1bits.SSPIF = 0;
    SSPCON2bits.SEN = 1;

    return i2c_wait_complete();
}

bool i2c_stop(void)
{
    if (!i2c_wait_idle())
        return false;

    PIR1bits.SSPIF = 0;
    SSPCON2bits.PEN = 1;

    return i2c_wait_complete();
}

#endif // PIC16F887