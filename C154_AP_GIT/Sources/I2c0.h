/******************************************************************************
;       Program  : I2c0.h
;       Function : Declare I2C0 Master Function
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2018 / 01 / 18
;       Author   : Fenderson Lu
******************************************************************************/
#ifndef __I2c0_H__
#define __I2c0_H__
//---------------------------- Define Constant --------------------------------//
#define	SLA_ADDR_EEPROM					0x50			// Shift is 0xA0

#define	LEN_A_BYTE								1

#define	SIZE_I2C0_RX			20
#define	SIZE_A_BYTE_REG						1
#define	SIZE_EEPROM_REG						2
#define	SIZE_EEPROM_DATA					1
#define	SIZE_EEPROM_TX						(SIZE_EEPROM_REG + SIZE_EEPROM_DATA)
//---------------------------- Declare Function -------------------------------//
void I2c0_Init(void);
uint8_t I2c0_Master_Byte_Read(uint16_t uSlaveAddr, uint8_t *ucRegister);
uint8_t I2c0_BuffRx_Get(uint8_t ucIndex);
#endif


