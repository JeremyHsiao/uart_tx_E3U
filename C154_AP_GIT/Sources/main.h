/******************************************************************************
;       Program  : Main.h
;       Function : Declare Main Function & Variable
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2018 / 12 / 28
;       Author   : Fenderson Lu
******************************************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__
//---------------------------- Define Constant ------------------------------//
#define	APROM_USED_SIZE	0x4000		// ROM 16KB=0x4000, Flash 64KB=0x10000
#define	SPACE						16

#define	FW_VER						"V3.03.01"

//---------------------------- Support Function -----------------------------//
#define	ENABLE						1
#define	DISABLE						0

#define	THRESHOLD_SLEEP_EN			3
//---------------------------- Declare Function -----------------------------// 
void Function_5ms(void);
void Function_50ms(void);
void Function_1s(void);

#endif

