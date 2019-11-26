/******************************************************************************
;       Program  : Timer0.h
;       Function : Declare LPIT Channel 0 Function
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2018 / 1 / 7
;       Author   : Fenderson Lu
******************************************************************************/
#ifndef __TIMER0_H__
#define __TIMER0_H__
//---------------------------- Define Constant --------------------------------//
#define 	INT_TIME					1000                // Tick timer in 1000us period

#define 	TIME_5MS				(int)(5000/INT_TIME)
#define 	TIME_50MS				(int)(50000/INT_TIME)
#define 	TIME_1S					(int)(1000000/INT_TIME)

#define LPIT_CH        0							// LPIT channel used
//---------------------------- Declare Function -------------------------------//
void Timer0_Init(void);
void Timer0_Virtual(void);
void Timer0_Virtual_Clear(void);
uint16_t Timer0_RealTime_Get(void);

#endif

