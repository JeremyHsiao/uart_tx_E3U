/******************************************************************************
;       Program  : Fun.c
;       Function : Implement Project Scenario Functions 
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2019 / 5 / 16
;       Author   : CYS
******************************************************************************/
//---------------------------- Include File ---------------------------------//
#include "PinIO.h"

//---------------------------- Declare External Var --------------------------//
Test_Button_TypeDef gttest_button;
Time_TypeDef gttime;

/******************************************************************************
* Name         :Fun_Init
* Called by    :Main
* Return code  :N/A
* Description  :Initial all Software Function parameter/status
******************************************************************************/
void Fun_Init(void)
{
	gttest_button.u8status = TB_STATUS_NOT_TRIGGER;
	gttest_button.u8SleepCnt = THRESHOLD_SLEEP_EN;
	gttest_button.u8SW308_TCON_BIST_status = TCON_BIST_DISABLE;
	gttest_button.u8SW307_PMIC_status = PMIC_PUT_LOW;
	gttest_button.u8SW306_ICTFC_status = ICTFC_STATUS_INIT;
	gttest_button.u8SW305 = 0;
	gttest_button.u8EB_PTC16_buzzer_volume_level = BUZZER_MIN_LEVEL;
    gtc154.tsleep_info.u8SleepFlage = SLEEPCTL_WAKEUP;
    GPIO_HIGH(LED_PORT, LED_PIN4);
    GPIO_HIGH(LED_PORT, LED_PIN5);
    GPIO_HIGH(LED_PORT, LED_PIN8);
    GPIO_HIGH(LED_PORT, LED_PIN9);

    tgtestflow_info.u8status = STATE_TESTFLOW_INIT;
    tgtestflow_info.u8SW_version_check = 0x00;
    tgtestflow_info.u8pnael_check = 0x00;
    tgtestflow_info.u8I2C_init_check = 0x00;

    gttime.u32year = 2019;
    gttime.u8month = 7;
    gttime.u8day = 11;
    gttime.u8hour = 14;
    gttime.u8minute = 57;
    gttime.u8second = 0;
}

/******************************************************************************
* Name         :Fun_Down_Counter
* Called by    :Need de-bounce Function
* Return code  :N/A
* Description  :de-bounce
******************************************************************************/
uint8_t Fun_Down_Counter(uint8_t *ulCountValues)
{
	uint8_t ucFlag = DISABLE;

	if(*ulCountValues)
	{
		*ulCountValues-=1;
		if(*ulCountValues == 0)
			ucFlag = ENABLE;
	}
	return ucFlag;
}

/******************************************************************************
* Name         :Fun_Delay_Millisecond
* Called by    :Need delay ms
* Return code  :N/A
* Description  :delay X ms
******************************************************************************/
void Fun_Delay_Millisecond(uint32_t ulTime)						
{
	uint32_t i,j;
	for(i=0;i<ulTime;i++)
		for(j=0;j<BASE_DELAY;j++);
}

/******************************************************************************
* Name         :Fun_SW308
* Called by    :Main
* Return code  :N/A
* Description  :Boutton SW308, TCON BIST
******************************************************************************/
void Fun_SW308(void)
{
	uint8_t u8ret;

	gttest_button.u8status = TB_STATUS_TRIGGER;

	if(gttest_button.u8SW308_TCON_BIST_status == TCON_BIST_DISABLE)
	{
		gttest_button.u8udata[0] = TCON_BIST_ENABLE;
		u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), FMID_TCON_BIST, gttest_button.u8udata, 1);

		if(u8ret == CSD_FINISH)
		{
			gttest_button.u8SW308_TCON_BIST_status = TCON_BIST_ENABLE;
			GPIO_LOW(LED_PORT, LED_PIN8);
		}
	}
	else if(gttest_button.u8SW308_TCON_BIST_status == TCON_BIST_ENABLE)
	{
		gttest_button.u8udata[0] = TCON_BIST_DISABLE;
		u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), FMID_TCON_BIST, gttest_button.u8udata, 1);

		if(u8ret == CSD_FINISH)
		{
			gttest_button.u8SW308_TCON_BIST_status = TCON_BIST_DISABLE;
			GPIO_HIGH(LED_PORT, LED_PIN8);
		}
	}

	gttest_button.u8status = TB_STATUS_FINISH;
}

/******************************************************************************
* Name         :Fun_SW307
* Called by    :Main
* Return code  :N/A
* Description  :Boutton SW307, PMIC
******************************************************************************/
void Fun_SW307(void)
{
	uint8_t u8ret;

	gttest_button.u8status = TB_STATUS_TRIGGER;

	if(gttest_button.u8SW307_PMIC_status == PMIC_PUT_LOW)
	{
		gttest_button.u8udata[0] = PMIC_PUT_HIGH;
		u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), FMID_PMIC_WPN, gttest_button.u8udata, 1);

		if(u8ret == CSD_FINISH)
		{
			gttest_button.u8SW307_PMIC_status = PMIC_PUT_HIGH;
			GPIO_LOW(LED_PORT, LED_PIN9);
		}
	}
	else if(gttest_button.u8SW307_PMIC_status == PMIC_PUT_HIGH)
	{
		gttest_button.u8udata[0] = PMIC_PUT_LOW;
		u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), FMID_PMIC_WPN, gttest_button.u8udata, 1);

		if(u8ret == CSD_FINISH)
		{
			gttest_button.u8SW307_PMIC_status = PMIC_PUT_LOW;
			GPIO_HIGH(LED_PORT, LED_PIN9);
		}
	}

	gttest_button.u8status = TB_STATUS_FINISH;
}

/******************************************************************************
* Name         :Fun_SW306
* Called by    :Main
* Return code  :N/A
* Description  :Boutton SW306, ICFTC
******************************************************************************/
void Fun_SW306(void)
{
	uint8_t u8ret;

	gttest_button.u8status = TB_STATUS_TRIGGER;

	gttest_button.u8udata[0] = ICTFC_PRIORITY_HIGH_ENABLE;
	u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), FMID_BAT_SENSE_PRIORITY_SWITCH, gttest_button.u8udata, 1);

	if(u8ret == CSD_FINISH)
	{
		gttest_button.u8udata[0] = ICTFC_PRIORITY_HIGH_ENABLE;
		u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), FMID_BAT_SENSE, gttest_button.u8udata, 1);

		if(u8ret == CSD_FINISH)
		{
			u8ret = CSD_MESGFRAME_READ_noUDS((DEV_ADDRESS+READ), FMID_ICT_FCT, 1);

			if(u8ret == CSD_FINISH)
			{
				GPIO_HIGH(LED_PORT, LED_PIN4);
				GPIO_HIGH(LED_PORT, LED_PIN5);
				for(uint8_t u8toggle_loop=0; u8toggle_loop<5; u8toggle_loop++)
				{
					GPIO_TOGGLE(LED_PORT, LED_PIN4);
					GPIO_TOGGLE(LED_PORT, LED_PIN5);
					Fun_Delay_Millisecond(200);
				}

				gttest_button.u8SW306_ICTFC_status = ICTFC_STATUS_FINISH;
				if(CSD_Frame_response.u8buffer[0] == 0x00)
				{
					GPIO_HIGH(LED_PORT, LED_PIN4);
					GPIO_LOW(LED_PORT, LED_PIN5);
				}
				else if(CSD_Frame_response.u8buffer[0] == 0x01)
				{
					GPIO_HIGH(LED_PORT, LED_PIN4);
					GPIO_HIGH(LED_PORT, LED_PIN5);
				}
				else if(CSD_Frame_response.u8buffer[0] == 0x02)
				{
					GPIO_LOW(LED_PORT, LED_PIN4);
					GPIO_LOW(LED_PORT, LED_PIN5);
				}
				else if(CSD_Frame_response.u8buffer[0] == 0x03)
				{
					GPIO_LOW(LED_PORT, LED_PIN4);
					GPIO_HIGH(LED_PORT, LED_PIN5);
				}

				Fun_Delay_Millisecond(2000);

				GPIO_HIGH(LED_PORT, LED_PIN4);
				GPIO_HIGH(LED_PORT, LED_PIN5);
				for(uint8_t u8toggle_loop=0; u8toggle_loop<5; u8toggle_loop++)
				{
					GPIO_TOGGLE(LED_PORT, LED_PIN4);
					GPIO_TOGGLE(LED_PORT, LED_PIN5);
					Fun_Delay_Millisecond(200);
				}
				GPIO_HIGH(LED_PORT, LED_PIN4);
				GPIO_HIGH(LED_PORT, LED_PIN5);
			}
		}
	}

	gttest_button.u8udata[0] = ICTFC_PRIORITY_HIGH_DISABLE;
	u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), FMID_BAT_SENSE_PRIORITY_SWITCH, gttest_button.u8udata, 1);

	if(u8ret == CSD_FINISH)
	{
		gttest_button.u8SW306_ICTFC_status = ICTFC_STATUS_INIT;
	}

	gttest_button.u8status = TB_STATUS_FINISH;
}

/******************************************************************************
* Name         :Fun_SW305
* Called by    :Main
* Return code  :N/A
* Description  :Boutton SW305, Conrtol CSD Sleep/Wake up
******************************************************************************/
void Fun_SW305(void)
{
	gttest_button.u8status = TB_STATUS_TRIGGER;

	if(gtc154.tsleep_info.u8SleepFlage == SLEEPCTL_WAKEUP)
	{
		gtc154.tsleep_info.u8SleepFlage = SLEEPCTL_SLEEP;
	}
	else if(gtc154.tsleep_info.u8SleepFlage == SLEEPCTL_SLEEP)
	{
		gtc154.tsleep_info.u8SleepFlage = SLEEPCTL_WAKEUP;
	}

	gttest_button.u8status = TB_STATUS_FINISH;
}

/******************************************************************************
* Name         :Fun_Complex_SW305_SW306
* Called by    :Main
* Return code  :N/A
* Description  :Complex Boutton SW305 & SW305, Re-setting SERDES
******************************************************************************/
void Fun_Complex_SW305_SW306(void)
{
	gttest_button.u8status = TB_STATUS_TRIGGER;

	uint8_t en=0;

	GPIO_HIGH(SERDES_PORT, SERDES_PIN13_PWDNB);
	Fun_Delay_Millisecond(25);
	GPIO_LOW(SERDES_PORT, SERDES_PIN13_PWDNB);
	Fun_Delay_Millisecond(25);
	GPIO_HIGH(SERDES_PORT, SERDES_PIN13_PWDNB);
	Fun_Delay_Millisecond(25);
	Init_I2C_SERDES();

	en=Init_I2C_SERDES_Check();
	if(en==0x00)
	{
	    GPIO_HIGH(LED_PORT, LED_PIN4);
	    GPIO_HIGH(LED_PORT, LED_PIN5);
	    GPIO_HIGH(LED_PORT, LED_PIN8);
	    GPIO_HIGH(LED_PORT, LED_PIN9);

		for(uint8_t u8toggle_loop=0; u8toggle_loop<5; u8toggle_loop++)
		{
			GPIO_TOGGLE(LED_PORT, LED_PIN4);
			GPIO_TOGGLE(LED_PORT, LED_PIN5);
			GPIO_TOGGLE(LED_PORT, LED_PIN8);
			GPIO_TOGGLE(LED_PORT, LED_PIN9);
			Fun_Delay_Millisecond(400);
		}

	    GPIO_HIGH(LED_PORT, LED_PIN4);
	    GPIO_HIGH(LED_PORT, LED_PIN5);
	    GPIO_HIGH(LED_PORT, LED_PIN8);
	    GPIO_HIGH(LED_PORT, LED_PIN9);
	}

	gttest_button.u8status = TB_STATUS_FINISH;
}

/******************************************************************************
* Name         :Fun_EB_Buzzer
* Called by    :Main
* Return code  :N/A
* Description  :Boutton PTD16, Conrtol CSD Buzzer
******************************************************************************/
void Fun_EB_Buzzer(void)
{
	uint8_t u8ret;
	//uint8_t ADC_TxBuff[2] = { 0x00, 0x00};

	uint8_t u8buzzer_ctrl_count;

	gttest_button.u8status = TB_STATUS_TRIGGER;

	u8buzzer_ctrl_count = 0;

	while(u8buzzer_ctrl_count<6)
	{
		/* use MSG ID control RXB buzzer volume*/
		gttest_button.u8EB_PTC16_buzzer_volume_level = BUZZER_MAX_LEVEL;
		gttest_button.u8udata[0] = gttest_button.u8EB_PTC16_buzzer_volume_level;
		u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), MID_VOLUME_ADJUST, gttest_button.u8udata, 1);

		if(u8ret == CSD_FINISH)
		{
			u8buzzer_ctrl_count += 1;
		}

		/* use I2C control TXB buzzer source*/
		/*ADC_TxBuff[0]= 0x52;
		ADC_TxBuff[1]= 0x00;
		LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C0, SLA_ADDR_ADC3100, false);
		LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C0, ADC_TxBuff, 2, true, OSIF_WAIT_FOREVER);
		u8buzzer_ctrl_count += 1;*/

		Fun_Delay_Millisecond(500);

		/* use MSG ID control RXB buzzer volume*/
		gttest_button.u8EB_PTC16_buzzer_volume_level = BUZZER_MIN_LEVEL;
		gttest_button.u8udata[0] = gttest_button.u8EB_PTC16_buzzer_volume_level;
		u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), MID_VOLUME_ADJUST, gttest_button.u8udata, 1);

		if(u8ret == CSD_FINISH)
		{
			u8buzzer_ctrl_count += 1;
		}

		/* use I2C control TXB buzzer source*/
		/*ADC_TxBuff[0]= 0x52;
		ADC_TxBuff[1]= 0x88;
		LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C0, SLA_ADDR_ADC3100, false);
		LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C0, ADC_TxBuff, 2, true, OSIF_WAIT_FOREVER);
		u8buzzer_ctrl_count += 1;*/

		Fun_Delay_Millisecond(500);
	}

	gttest_button.u8status = TB_STATUS_FINISH;
}

/******************************************************************************
* Name         :Fun_Remove_Fun_Limit
* Called by    :Main
* Return code  :N/A
* Description  :Complex Boutton SW308, Remove Fun Limit
******************************************************************************/
void Fun_Remove_Fun_Limit(void)
{
	uint8_t ADC_TxBuff[2] = { 0x00, 0x00};
	uint8_t u8ret;
	uint32_t count = 0;
	uint8_t Backlight_duty[1];

	GPIO_LOW(LED_PORT, LED_PIN4);
	GPIO_LOW(LED_PORT, LED_PIN5);
	GPIO_LOW(LED_PORT, LED_PIN8);
	GPIO_LOW(LED_PORT, LED_PIN9);

	gttest_button.u8udata[0] = gttest_button.u8EB_PTC16_buzzer_volume_level;
	u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), MID_VOLUME_ADJUST, gttest_button.u8udata, 1);

	u8ret=CSD_FINISH;
	if(u8ret == CSD_FINISH)
	{
		for(uint8_t u8toggle_loop=0; u8toggle_loop<5; u8toggle_loop++)
		{
			GPIO_TOGGLE(LED_PORT, LED_PIN4);
			GPIO_TOGGLE(LED_PORT, LED_PIN5);
			GPIO_TOGGLE(LED_PORT, LED_PIN8);
			GPIO_TOGGLE(LED_PORT, LED_PIN9);
			Fun_Delay_Millisecond(400);
		}
		tgtestflow_info.u8status = STATE_TESTFLOW_SUCCES;
	}

	ADC_TxBuff[0]= 0x52;
	ADC_TxBuff[1]= 0x00;
	LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C0, SLA_ADDR_ADC3100, false);
	LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C0, ADC_TxBuff, 2, true, OSIF_WAIT_FOREVER);

	Backlight_duty[0] = 0x64;
	count = sizeof(Backlight_duty)/sizeof(Backlight_duty[0]);
	u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), MID_BACKLIGHT_DUTY, Backlight_duty, count);
}

/******************************************************************************
* Name         :Fun_World_Period_Count
* Called by    :Main
* Return code  :N/A
* Description  :Counting Real-time
******************************************************************************/
void Fun_World_Period_Count(void)
{
	uint8_t u8leap_year = 0;
	uint8_t u8print = 0;

	gttime.u8second += 1;
	if(gttime.u8second == 60)
	{
		gttime.u8second = 0;
		gttime.u8minute += 1;
		if(gttime.u8minute == 60)
		{
			gttime.u8minute = 0;
			gttime.u8hour += 1;
			if(gttime.u8hour == 24)
			{
				gttime.u8hour = 0;
				gttime.u8day += 1;

				switch(gttime.u8month)
				{
					case 2:
						if((gttime.u32year%4) == 0)
						{
							if((gttime.u32year%100) == 0)
							{
								if((gttime.u32year%400) == 0)
								{
									if((gttime.u32year%4000) == 0)
									{
										u8leap_year = 0;
									}
									else
									{
										u8leap_year = 1;
									}
								}
								else
								{
									u8leap_year = 0;
								}
							}
							else
							{
								u8leap_year = 1;
							}
						}
						else
						{
							u8leap_year = 0;
						}

						if(u8leap_year == 1)
						{
							if(gttime.u8day == 30)
							{
								gttime.u8day = 1;
								gttime.u8month += 1;
							}
						}
						else
						{
							if(gttime.u8day == 29)
							{
								gttime.u8day = 1;
								gttime.u8month += 1;
							}
						}
						break;

					case 1:
					case 3:
					case 5:
					case 7:
					case 9:
					case 11:
						if(gttime.u8day == 31)
						{
							gttime.u8day = 1;
							gttime.u8month += 1;
						}
						break;

					case 4:
					case 6:
					case 8:
					case 10:
					case 12:
						if(gttime.u8day == 32)
						{
							gttime.u8day = 1;
							gttime.u8month += 1;
						}
						break;

					default:
						break;
				};

				if(gttime.u8month == 13)
				{
					gttime.u8month = 1;
					gttime.u32year += 1;
				}
			}
		}
	}

	if(gttime.u8second == 0)
	{
		u8print = gttime.u8month;
		Uart1_Data_Send(&u8print, 1);

		u8print = gttime.u8day;
		Uart1_Data_Send(&u8print, 1);

		u8print = gttime.u8hour;
		Uart1_Data_Send(&u8print, 1);

		u8print = gttime.u8minute;
		Uart1_Data_Send(&u8print, 1);

		u8print = gttime.u8second;
		Uart1_Data_Send(&u8print, 1);
	}
}




