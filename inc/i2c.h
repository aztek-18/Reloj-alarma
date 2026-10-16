/*
    The I2C information is in page number 80 of the datasheet for PIC16F887A.

    FOrmula for calculating Fdat

        F_DAT = Fosc/(4(SSPADD +1));

*/

#include <stdbool.h>
#include <stdint.h>
#include <xc.h>   

#define I2C_BAUD        100000UL
#define I2C_TIMEOUT      50000UL

/*
 * @brief Inicializes the module specifically for the PIC16F887A and runs at 100KHz.
          This meks it run as master and is necessary to run it every time we want to use i2c.
 * @param void
 * @return void 
*/
void i2c_init(void);

/*
 * @brief wait for idle. Not used during normal programming.
 * @param void
 * @return true ot false 
*/
static bool i2c_wait_idle(void);

/*
 * @brief wait for complete. Not used during normal programming.
 * @param void
 * @return true ot false 
*/
static bool i2c_wait_complete(void);

/*
 * @brief Generates the START condition
 * @return true ot false [success - failed]
*/
bool i2c_start(void);

/*
 * @brief Stops the operation
 * @return true ot false  [sucess - failed]
*/
bool i2c_stop(void);

/*
 * @brief Sends a byte of information and acknowledge the ACK.
 * @param unit8_t data - 7 bit data to be transmited
 * @return true ot false [success - failed]
*/
bool i2c_write_byte(uint8_t data);

/*
 * @brief Sends a direction in 7 bits (read only)
 * @param uint8_t adress - 7 bit direction of the device/sensor
 * @return true ot false  [success - failed]
*/
bool i2c_write_address(uint8_t address);