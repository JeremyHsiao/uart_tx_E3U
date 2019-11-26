/******************************************************************************
;       Program  : PinIO.c
;       Function : GPIO Configure and Setting 
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2019 / 05 / 16
;       Author   : CYS
******************************************************************************/
//---------------------------- Include File ---------------------------------//
#include "PinIO.H"		

/******************************************************************************
* Name         :PinIO_Init
* Called by    :Main
* Return code  :N/A
* Description  :Initial Pin_mux driver
******************************************************************************/
void PinIO_Init(void)
{   
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);			// Initialize pins
}



