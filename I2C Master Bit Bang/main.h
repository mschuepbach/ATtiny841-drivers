/*
***************************************************************************************************
* Project:  I2C Master Bit Bang Driver
* Filename: main.h
*
* Created: 01.05.2017
* Author:  M. Schuepbach
*
* Description: This is the header file for main.c
*
***************************************************************************************************
*/


#ifndef MAIN_H_
#define MAIN_H_


/*
***************************************************************************************************
**                                         USER DEFINES
***************************************************************************************************
*/
#define F_CPU   1000000UL           /* F_osc=8MHz & CKDIV=8 -> 8MHz / 8 = 1MHz */

#define RTC_DS1307_ADDRESS      (0x68 << 1)     /* 7-bit address of RTC converted to 8-bit */
#define EEPROM_AT24C32_ADDRESS   0xA0           /* 8-bit address of EEPROM */

/* End of configuration options. Change followings only if you know what you are doing.          */
/* ********************************************************************************************* */


/*
***************************************************************************************************
**                                   SYSTEM DEFINES AND MACROS
***************************************************************************************************
*/
// Add system defines here


/*
***************************************************************************************************
**                                           INCLUDES
***************************************************************************************************
*/
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>


/*
***************************************************************************************************
**                                  GLOBAL VARIABLES AND ARRAYS
***************************************************************************************************
*/
// Add global variables or arrays here and use extern


/*
***************************************************************************************************
**                                      FUNCTION PROTOTYPES
***************************************************************************************************
*/
void ATtiny841_board_init(void);


#endif /* MAIN_H_ */
