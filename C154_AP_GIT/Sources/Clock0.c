/******************************************************************************
;       Program  : Clock0.c
;       Function : Clock Control 
;       Chip     : NXP FS32K118
;       Clock    : Internal 48MHz
;       Date     : 2018 / 12 / 28
;       Author   : Fenderson Lu
******************************************************************************/
//---------------------------- Include File ---------------------------------//
#include "PinIO.H"		
#include "Clock0.h"			
//---------------------------- Declare External Var -------------------------//
char cClockTemp[20];
uint32_t ulFrequency;

/******************************************************************************
* Name         :Clock0_Init
* Called by    :main.c
* Return code  :N/A
* Description  :initial clockMan1 driver
******************************************************************************/
void Clock0_Init(void)
{
	CLOCK_SYS_Init(g_clockManConfigsArr,   CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
}

/******************************************************************************
* Name         :Clock0_Show
* Called by    :N/A
* Return code  :N/A
* Description  :Show clock data
******************************************************************************/
void Clock0_Show(void)
{
	/* Get current core clock frequency */
	(void)CLOCK_SYS_GetFreq(CORE_CLK, &ulFrequency);
}

