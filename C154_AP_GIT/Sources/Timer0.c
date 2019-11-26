/******************************************************************************
;       Program  : Timer0.c
;       Function : Use LPIT Channel 0 to Gerernal Vistual Timer
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2018 / 1 / 7
;       Author   : Fenderson Lu
;       Describe : 
******************************************************************************/
//---------------------------- Include File ---------------------------------//
#include "PinIO.H"
//---------------------------- Declare External Var -------------------------//
static uint32_t uVirtualTimer,uRealTimer;
static uint32_t uVTimer1msCount;

/******************************************************************************
* Name         :LPIT_ISR
* Called by    :INT_SYS_InstallHandler for LPIT0_IRQn
* Return code  :N/A
* Description  :Function of Timer Interrupt, count uRealTimer
******************************************************************************/
void LPIT_ISR(void)
{
	uRealTimer++;
	LPIT_DRV_ClearInterruptFlagTimerChannels(INST_LPIT0, (1 << LPIT_CH));				// Clear LPIT channel flag
}

/******************************************************************************
* Name         :Timer0_Init
* Called by    :Main
* Return code  :N/A
* Description  :Initial lpit0 driver
******************************************************************************/
void Timer0_Init(void)
{ 	
	LPIT_DRV_Init(INST_LPIT0, &lpit0_InitConfig);								// Set ISR interrupt period time = 1000 (1ms)
	LPIT_DRV_InitChannel(INST_LPIT0, LPIT_CH, &lpit0_ChnConfig0);				// Initialize LPIT_CH0 and configure it as a periodic counter, which is used to generate an interrupt every second.
	INT_SYS_InstallHandler(LPIT0_IRQn, &LPIT_ISR, (isr_t *)0);				 	// Install LPIT_ISR as LPIT_CH0 interrupt handler
	LPIT_DRV_StartTimerChannels(INST_LPIT0, (1 << LPIT_CH));					// Start LPIT0_CH0 counter

    uVirtualTimer = uRealTimer = uVTimer1msCount = 0;
}

/******************************************************************************
* Name         :Timer0_Virtual
* Called by    :Main
* Return code  :N/A
* Description  :Perform the corresponding function according to the count time
******************************************************************************/
void Timer0_Virtual(void)
{
	if(uVirtualTimer != uRealTimer)
	{ 
		uVirtualTimer++;
		uVTimer1msCount++;

		if(uVTimer1msCount%TIME_1S==0)
		{
			Function_1s();
			uVTimer1msCount=0;
		}
		else
		{
			if(uVTimer1msCount%TIME_5MS==0)
			{
				Function_5ms();
			}

			if(uVTimer1msCount%TIME_50MS==0)
			{
				Function_50ms();
			}
		}
	}
}

/******************************************************************************
* Name         :Timer0_Virtual_Clear
* Called by    :Main
* Return code  :N/A
* Description  :Reset uRealTimer
******************************************************************************/
void Timer0_Virtual_Clear(void) 
{
	uVirtualTimer = uRealTimer;
}

/******************************************************************************
* Name         :Timer0_RealTime_Get
* Called by    :N/A
* Return code  :N/A
* Description  :Return uRealTimer
******************************************************************************/
uint16_t Timer0_RealTime_Get(void) 
{
	return(uRealTimer);
}

