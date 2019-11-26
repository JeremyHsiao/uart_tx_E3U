/******************************************************************************
;       Program  : PinIO.h
;       Function : Define IO Pin Function
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2019 / 05 / 16
******************************************************************************/
#ifndef _PinIO_H
#define _PinIO_H
//---------------------------- Include File ---------------------------------//
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "Cpu.h"
#include "pin_mux.h"
#include "Clock0.h"
#include "Timer0.h"
#include "Uart0.h"
#include "Uart1.h"
#include "I2c0.h"

#include "main.h"
#include "System.h"
#include "Fun.h"

#include "Crc32.h"
#include "Init.h"
#include "mesgframe.h"

#include "testflow.h"

#include "CSD_frame.h"
#include "Forward.h"
#include "Uds.h"

//---------------------------- Delcare Content  ----------------------------//
#define		INPUT				0
#define		OUTPUT				1

#define		LEVEL_HIGH			1
#define		LEVEL_LOW			0

#define		BIT0				0
#define		BIT1				1
#define		BIT2				2
#define		BIT3				3
#define		BIT4				4
#define		BIT5				5
#define		BIT6				6
#define		BIT7				7
#define		BIT8				8
#define		BIT9				9
#define		BIT10				10
#define		BIT11				11
#define		BIT12				12
#define		BIT13				13
#define		BIT14				14
#define		BIT15				15
#define		BIT16				16
//---------------------------- I/O Pin Definition ---------------------------//	
#define	LED_PORT		PTE
#define	LED_PIN4  		BIT4
#define	LED_PIN5  		BIT5
#define	LED_PIN8  		BIT8
#define	LED_PIN9  		BIT9

#define	WAKEUP_PORT					PTD
#define	WAKEUP_PIN16  				BIT16

#define	TESTBUTTON_PORT				PTD
#define	TESTBUTTON_PIN15_SW308		BIT15
#define	TESTBUTTON_PIN5_SW307		BIT5
#define	TESTBUTTON_PIN3_SW306		BIT3
#define	TESTBUTTON_PIN2_SW305		BIT2

#define	BUZZERBUTTON_PORT		PTC
#define	BUZZERBUTTON_PIN15		BIT15

#define	ADC3100_PORT			PTD
#define	ADC3100_PIN0_RESET		BIT0

#define	SERDES_PORT				PTB
#define	SERDES_PIN13_PWDNB		BIT13

//---------------------------- Declare Function Replace Name -----------------//
#define	GPIO_HIGH(port, pin)			PINS_DRV_SetPins(port, (1 << pin))
#define	GPIO_LOW(port, pin)			PINS_DRV_ClearPins(port, (1 << pin))
#define	GPIO_TOGGLE(port, pin)		PINS_DRV_TogglePins(port, (1 << pin))
#define	GPIO_READ(port, pin)			((PINS_DRV_ReadPins(port) & (1 << pin)) >> pin) 
#define	GPIO_DIRECTION_SET(port, pin, direction)		PINS_DRV_SetPinsDirection(port, (direction << pin))	
//---------------------------- Declare Function -----------------------------// 
void PinIO_Init(void);
void PinIO_System_Power(uint8_t ucSwitch);
void PinIO_Panel_Power_Check(void);
#endif


