/*
***************************************************************************************************
* Project  : I2C Master Bit Bang Driver
* Filename : I2C_Master_Bit_Bang_Driver.h
*
* Created : 01.05.2017
* Author  : M. Schuepbach
*
* Description : This is the header file for I2C_Master_Bit_Bang_Driver.c
*
***************************************************************************************************
*/

#ifndef I2C_MASTER_BIT_BANG_DRIVER_H_
#define I2C_MASTER_BIT_BANG_DRIVER_H_


/*
***************************************************************************************************
*                                         USER DEFINES
***************************************************************************************************
*/
#define F_CPU   1000000UL           /* F_osc=8MHz & CKDIV=8 -> 8MHz / 8 = 1MHz */

#define I2C_SCL         4           /* SCL Bit */
#define I2C_SCL_PORT    PORTA       /* SCL Port */
#define I2C_SCL_DDR     DDRA
#define I2C_SCL_PIN     PINA

#define I2C_SDA         6           /* SDA Bit */
#define I2C_SDA_PORT    PORTA       /* SDA Port */
#define I2C_SDA_DDR     DDRA
#define I2C_SDA_PIN     PINA

/* End of configuration options. Do not change followings without care.                          */
/*************************************************************************************************/


/*
***************************************************************************************************
*                                       DEFINES AND MACROS
***************************************************************************************************
*/


#define I2C_DATA_HI()                                                                             \
I2C_SDA_DDR &= ~(1 << I2C_SDA);     /* SDA as input, pull-up will pull SDA high */

#define I2C_DATA_LO()                                                                             \
I2C_SDA_DDR |= (1 << I2C_SDA);      /* SDA as output */                                           \
I2C_SDA_PORT &= ~(1 << I2C_SDA);    /* Pull SDA low  */

#define I2C_CLOCK_HI()                                                                            \
I2C_SCL_DDR &= ~(1 << I2C_SCL);     /* SCL as input, pull-up will pull SCL high */

#define I2C_CLOCK_LO()                                                                            \
I2C_SCL_DDR |= (1 << I2C_SCL);      /* SCL as output */                                           \
I2C_SCL_PORT &= ~(1 << I2C_SCL);    /* Pull SCL low  */

#define I2C_READ    0x01
#define I2C_WRITE   0x00

#define I2C_ACK     0       /* Acknowledge     */
#define I2C_NACK    1       /* Not Acknowledge */


/*
***************************************************************************************************
*                                           INCLUDES
***************************************************************************************************
*/
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>


/*
***************************************************************************************************
*                                   GLOBAL VARIABLES AND ARRAYS
***************************************************************************************************
*/
bool I2C_started;


/*
***************************************************************************************************
*                                       FUNCTION PROTOTYPES
***************************************************************************************************
*/
void init_ATtiny841_board(void);
void I2C_init(void);
bool read_SCL(void);
bool read_SDA(void);
void set_SCL(void);
void clear_SCL(void);
void set_SDA(void);
void clear_SDA(void);
void arbitration_lost(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_write_bit(bool bit);
bool I2C_read_bit(void);
bool I2C_write_byte(bool send_start, bool send_stop, unsigned char byte);
unsigned char I2C_read_byte(bool nack, bool send_stop);
bool I2C_write(unsigned char slave_address, unsigned char slave_register, unsigned char data_to_write);
unsigned char I2C_read(unsigned char slave_address, unsigned char slave_register);
bool I2C_write_16bit_addr(unsigned char slave_address, unsigned char slave_high_register, unsigned char slave_low_register, unsigned char data_to_write);
unsigned char I2C_read_16bit_addr(unsigned char slave_address, unsigned char slave_high_register, unsigned char slave_low_register);
unsigned char BCD_to_decimal(unsigned char bcd);



#endif /* I2C_MASTER_BIT_BANG_DRIVER_H_ */