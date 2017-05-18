/*
***************************************************************************************************
* Project  : I2C Master Bit Bang Driver
* Filename : I2C_Master_Bit_Bang_Driver.c
*
* Created : 01.05.2017
* Author  : M. Schuepbach
*
* Description : This is a I2C Master Bit Bang Driver. Tested with ATtiny841 and RTC DS1307.
*               Standard-mode (100kHz) only.
*
* Source : "https://en.wikipedia.org/wiki/I%C2%B2C#Example_of_bit-banging_the_I.C2.B2C_master_protocol"
*
***************************************************************************************************
*/


/*
***************************************************************************************************
*                                           INCLUDES
***************************************************************************************************
*/
#include "I2C_Master_Bit_Bang_Driver.h"


/*
***************************************************************************************************
*                                   GLOBAL VARIABLES AND ARRAYS
***************************************************************************************************
*/
bool I2C_started = false;


/*
***************************************************************************************************
*                                             FUNCTIONS
***************************************************************************************************
*/
void I2C_init(void)
{
    set_SCL();
    set_SDA();
    
    _delay_us(10);
}

bool read_SCL(void)
{
    I2C_SCL_DDR &= ~(1 << I2C_SCL);             /* Set SCL as input */
    return ((I2C_SCL_PIN >> I2C_SCL) & 0x01);   /* Read pin and return the result */
}

bool read_SDA(void)
{
    I2C_SDA_DDR &= ~(1 << I2C_SDA);             /* Set SDA as input */
    return ((I2C_SDA_PIN >> I2C_SDA) & 0x01);   /* Read pin and return the result */
}

void set_SCL(void)
{
    I2C_CLOCK_HI();
}

void clear_SCL(void)
{
    I2C_CLOCK_LO();
}

void set_SDA(void)
{
    I2C_DATA_HI();
}

void clear_SDA(void)
{
    I2C_DATA_LO();
}

void arbitration_lost(void)
{
    /* Not quite sure what to do here... */
    I2C_stop();
    _delay_ms(1);
}

/*
***************************************************************************************************
* Description : Do a I2C start condition.
***************************************************************************************************
*/
void I2C_start(void)
{
    if (I2C_started) {                  /* If I2C has started, do a restart condition */
        set_SDA();
        _delay_us(1);
        set_SCL();
        while (read_SCL() == 0);        /* Clock stretching */
        
        _delay_us(5);
    }
    
    if (read_SDA() == 0) {
        arbitration_lost();
    }
    
    clear_SDA();                        /* SCL is high, set SDA from 1 to 0 to start */
    _delay_us(1);
    clear_SCL();
    I2C_started = true;
}

/*
***************************************************************************************************
* Description : Do a I2C stop condition.
***************************************************************************************************
*/
void I2C_stop(void)
{
    clear_SDA();
    _delay_us(1);
    set_SCL();
    while (read_SCL() == 0);            /* Clock stretching */
    _delay_us(4);
    set_SDA();                          /* SCL is high, set SDA from 0 to 1 to stop */
    _delay_us(1);
    
    if (read_SDA() == 0) {
        arbitration_lost();
    }
    
    //clear_SCL();                      /* Mistake? Does not work if not commented out*/
    I2C_started = false;
}

/*
***************************************************************************************************
* Description : Write a bit to the I2C Bus.
*
* Arguments   : BOOL    TRUE:  Logic high
*                       FALSE: Logic low
*
* Returns     : none
*
***************************************************************************************************
*/
void I2C_write_bit(bool bit)
{
    if (bit) {
        set_SDA();
        } else {
        clear_SDA();
    }
    
    _delay_us(1);
    set_SCL();
    _delay_us(4);
    
    while (read_SCL() == 0);            /* Clock stretching */
    
    if (bit && (read_SDA() == 0)) {
        arbitration_lost();
    }
    
    clear_SCL();
}

/*
***************************************************************************************************
* Description : Read a bit from the I2C Bus.
*
* Arguments   : none
*
* Returns     : BOOL    TRUE:  Logic high
*                       FALSE: Logic low
*
***************************************************************************************************
*/
bool I2C_read_bit(void)
{
    bool bit;
    
    
    set_SDA();
    _delay_us(4);
    set_SCL();
    
    while (read_SCL() == 0);                 /* Clock stretching */
    
    _delay_us(4);
    bit = read_SDA();
    clear_SCL();
    
    return bit;
}

/*
***************************************************************************************************
* Description : Write a byte to the I2C Bus.
*
* Arguments   : BOOL    TRUE:  Start condition
*                       FALSE: No start condition
*
*               BOOL    TRUE:  Stop condition
*                       FALSE: No stop condition
*
*               unsigned char: Byte to write
*
* Returns     : BOOL    TRUE:  Not Acknowledge
*                       FALSE: Acknowledge
*
***************************************************************************************************
*/
bool I2C_write_byte(bool send_start, bool send_stop, unsigned char byte)
{
    unsigned bit;
    bool     nack;
    
    
    if (send_start) {
        I2C_start();
    }
    
    for (bit = 0; bit < 8; ++bit) {
        I2C_write_bit((byte & 0x80) != 0);
        byte <<= 1;
    }
    
    nack = I2C_read_bit();
    
    if (send_stop) {
        I2C_stop();
    }
    
    return nack;
}

/*
***************************************************************************************************
* Description : Read a byte from the I2C Bus.
*
* Arguments   : BOOL    TRUE:  Send Not Acknowledge Bit
*                       FALSE: Send Acknowledge Bit
*
*               BOOL    TRUE:  Stop condition
*                       FALSE: No stop condition
*
* Returns     : unsigned char: Byte from slave
*
***************************************************************************************************
*/
unsigned char I2C_read_byte(bool nack, bool send_stop)
{
    unsigned char byte = 0;
    unsigned char bit;
    
    
    for (bit = 0; bit < 8; ++bit) {
        byte = (byte << 1) | I2C_read_bit();
    }
    
    I2C_write_bit(nack);
    
    if (send_stop) {
        I2C_stop();
    }
    
    return byte;
}

/*
***************************************************************************************************
* Description : Write a byte to the I2C slave with a 8 bit register.
*
* Arguments   : unsigned char: Slave address
*
*               unsigned char: Slave register
*
*               unsigned char: Byte to write
*
* Returns     : BOOL    TRUE:  Write successful
*                       FALSE: Write not successful
*
***************************************************************************************************
*/
bool I2C_write(unsigned char slave_address, unsigned char slave_register, unsigned char data_to_write)
{
    if (I2C_write_byte(true, false, (slave_address | I2C_WRITE)) == I2C_ACK ) {     /* Start condition, slave address, write bit */
        if (I2C_write_byte(false, false, slave_register) == I2C_ACK ) {             /* Slave register */
            if (I2C_write_byte(false, true, data_to_write) == I2C_ACK ) {           /* Data, stop condition */
                return true;
            }
        }
    }
    return false;
}

/*
***************************************************************************************************
* Description : Read a byte from the I2C slave with a 8 bit register.
*
* Arguments   : unsigned char: Slave address
*
*               unsigned char: Slave register
*
* Returns     : unsigned char: Byte from slave or 0xFF if read was not successful
*
***************************************************************************************************
*/
unsigned char I2C_read(unsigned char slave_address, unsigned char slave_register)
{
    if (I2C_write_byte(true, false, (slave_address | I2C_WRITE)) == I2C_ACK ) {         /* Start condition, slave address, write bit */
        if (I2C_write_byte(false, false, slave_register) == I2C_ACK ) {                 /* Slave register */
            if (I2C_write_byte(true, false, (slave_address | I2C_READ)) == I2C_ACK ) {  /* Start condition, slave address, read bit */
                return I2C_read_byte(true, true);                                       /* Read data, send NACK, stop condition */
            }
        }
    }
    return 0xFF;
}

/*
***************************************************************************************************
* Description : Write a byte to the I2C slave with a 16 bit register.
*
* Arguments   : unsigned char: Slave address
*
*               unsigned char: First 8 bit of slave register
*
*               unsigned char: Second 8 bit of slave register
*
*               unsigned char: Byte to write
*
* Returns     : BOOL    TRUE:  Write successful
*                       FALSE: Write not successful
*
***************************************************************************************************
*/
bool I2C_write_16bit_addr(unsigned char slave_address, unsigned char slave_high_register, unsigned char slave_low_register, unsigned char data_to_write)
{
    if (I2C_write_byte(true, false, (slave_address | I2C_WRITE)) == I2C_ACK ) {     /* Start condition, slave address, write bit */
        if (I2C_write_byte(false, false, slave_high_register) == I2C_ACK ) {        /* First 8 bit of slave register */
            if (I2C_write_byte(false, false, slave_low_register) == I2C_ACK ) {     /* Second 8 bit of slave register */
                if (I2C_write_byte(false, true, data_to_write) == I2C_ACK ) {       /* Data, stop condition */
                    return true;
                }
            }                
        }
    }
    return false;
}

/*
***************************************************************************************************
* Description : Read a byte from the I2C slave with a 16 bit register.
*
* Arguments   : unsigned char: Slave Address
*
*               unsigned char: First 8 bit of slave register
*
*               unsigned char: Second 8 bit of slave register
*
* Returns     : unsigned char: Byte from slave or 0xFF if read was not successful
*
***************************************************************************************************
*/
unsigned char I2C_read_16bit_addr(unsigned char slave_address, unsigned char slave_high_register, unsigned char slave_low_register)
{
    if (I2C_write_byte(true, false, (slave_address | I2C_WRITE)) == I2C_ACK ) {             /* Start condition, slave address, write bit */
        if (I2C_write_byte(false, false, slave_high_register) == I2C_ACK ) {                /* First 8 bit of slave register */
            if (I2C_write_byte(false, false, slave_low_register) == I2C_ACK ) {             /* Second 8 bit of slave register */
                if (I2C_write_byte(true, false, (slave_address | I2C_READ)) == I2C_ACK ) {  /* Start condition, slave address, read bit */
                    return I2C_read_byte(true, true);                                       /* Read data, send NACK, stop condition */
                }
            }                
        }
    }
    return 0xFF;
}

/*
***************************************************************************************************
* Description : Convert BCD to decimal
*
* Arguments   : unsigned char: BCD code
*
* Returns     : unsigned char: decimal number (0...99)
*
***************************************************************************************************
*/
unsigned char BCD_to_decimal(unsigned char bcd)
{
    return (bcd & 0x0F) + ((bcd >> 4) * 10);
}
