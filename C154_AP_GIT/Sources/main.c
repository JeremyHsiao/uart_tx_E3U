/******************************************************************************
;       Program  : main.c
;       Function : The Main Program
;       Chip     : NXP FS32K118
;       Clock    : Internal 48MHz
;       Date     : 2018 / 01 / 18
;       Author   : Fenderson Lu
******************************************************************************/
//---------------------------- Include File ---------------------------------//
#include "PinIO.H"

/******************************************************************************
* Name         :main
* Called by    :N/A
* Return code  :N/A
* Description  :Initial all driver and execute super loop
******************************************************************************/
int main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/
	Clock0_Init();
	Timer0_Init();
	PinIO_Init();
	Uart0_Init();
	Forward_Service_Initial();
	I2c0_Init();
	Fun_Init();

	Timer0_Virtual_Clear();
	while(1)
	{
		Timer0_Virtual();
	}
}

/******************************************************************************
* Name         :Function_5ms
* Called by    :Timer0_Virtual
* Return code  :N/A
* Description  :Executed every 5ms
******************************************************************************/
void Function_5ms(void)
{
	Forward_State_Machine[gtforward_info.u8state]();		// Check Forward Interrupt Event
	F_Test_Flow();
}

/******************************************************************************
* Name         :Function_50ms
* Called by    :Timer0_Virtual
* Return code  :N/A
* Description  :Executed every 50ms
******************************************************************************/
void Function_50ms(void)
{
	uint8_t button_value;

    button_value =  (GPIO_READ(BUZZERBUTTON_PORT, BUZZERBUTTON_PIN15)<<4) +
    		        (GPIO_READ(TESTBUTTON_PORT, TESTBUTTON_PIN2_SW305)<<3) +
					(GPIO_READ(TESTBUTTON_PORT, TESTBUTTON_PIN3_SW306)<<2) +
					(GPIO_READ(TESTBUTTON_PORT, TESTBUTTON_PIN5_SW307)<<1) +
					(GPIO_READ(TESTBUTTON_PORT, TESTBUTTON_PIN15_SW308));

	if(tgtestflow_info.u8status == STATE_TESTFLOW_SUCCES)
	{
		if((button_value == 0x1E) && (gttest_button.u8status == TB_STATUS_NOT_TRIGGER))
		{
			if(Fun_Down_Counter(&gttest_button.u8SleepCnt))
	        {
				Fun_SW308();
				gttest_button.u8SleepCnt = THRESHOLD_SLEEP_EN;
	        }
		}
		else if((button_value == 0x1D) && (gttest_button.u8status == TB_STATUS_NOT_TRIGGER))
		{
			if(Fun_Down_Counter(&gttest_button.u8SleepCnt))
	        {
				Fun_SW307();
				gttest_button.u8SleepCnt = THRESHOLD_SLEEP_EN;
	        }
		}
		else if((button_value == 0x1B) && (gttest_button.u8status == TB_STATUS_NOT_TRIGGER))
		{
			if(Fun_Down_Counter(&gttest_button.u8SleepCnt))
	        {
				Fun_SW306();
				gttest_button.u8SleepCnt = THRESHOLD_SLEEP_EN;
	        }
		}
		else if((button_value == 0x17) && (gttest_button.u8status == TB_STATUS_NOT_TRIGGER))
		{
			if(Fun_Down_Counter(&gttest_button.u8SleepCnt))
	        {
				Fun_SW305();
				gttest_button.u8SleepCnt = THRESHOLD_SLEEP_EN;
	        }
		}
		else if((button_value == 0x13) && (gttest_button.u8status == TB_STATUS_NOT_TRIGGER))
		{
			if(Fun_Down_Counter(&gttest_button.u8SleepCnt))
	        {
				Fun_Complex_SW305_SW306();
				gttest_button.u8SleepCnt = THRESHOLD_SLEEP_EN;
	        }
		}
		else if((button_value == 0x0F) && (gttest_button.u8status == TB_STATUS_NOT_TRIGGER))
		{
			if(Fun_Down_Counter(&gttest_button.u8SleepCnt))
	        {
				Fun_EB_Buzzer();
				gttest_button.u8SleepCnt = THRESHOLD_SLEEP_EN;
	        }
		}
		else if(button_value == 0x1F)
		{
			if(Fun_Down_Counter(&gttest_button.u8SleepCnt))
	        {
				gttest_button.u8status = TB_STATUS_NOT_TRIGGER;
				gttest_button.u8SleepCnt = THRESHOLD_SLEEP_EN;
	        }
		}
	}
	else if(tgtestflow_info.u8status == STATE_TESTFLOW_FAIL)
	{
		if((button_value == 0x1E) && (gttest_button.u8status == TB_STATUS_NOT_TRIGGER))
		{
			if(Fun_Down_Counter(&gttest_button.u8SleepCnt))
	        {
				Fun_Remove_Fun_Limit();
				gttest_button.u8SleepCnt = THRESHOLD_SLEEP_EN;
	        }
		}
		else if(button_value == 0x1F)
		{
			if(Fun_Down_Counter(&gttest_button.u8SleepCnt))
	        {
				gttest_button.u8status = TB_STATUS_NOT_TRIGGER;
				gttest_button.u8SleepCnt = THRESHOLD_SLEEP_EN;
	        }
		}
	}
}

/******************************************************************************
* Name         :Function_1s
* Called by    :Timer0_Virtual
* Return code  :N/A
* Description  :Executed every 1s
******************************************************************************/
void Function_1s(void)
{
	Fun_World_Period_Count();
}


