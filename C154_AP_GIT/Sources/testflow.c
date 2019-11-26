#define _TESTFLOW_C_
#include "PinIO.H"

Testflow_State S_Flow;
STEPS_IN_State S_Step;

uint8_t en = 0;
uint8_t result = 0;
uint32_t count = 0;

uint8_t Panel_power[1];
uint8_t Backlight_duty[1];

uint8_t u8ret;
Test_Flow_TypeDef tgtestflow_info;

/******************************************************************************
* Name         :F_Test_Flow
* Called by    :Main
* Return code  :N/A
* Description  :TXB initialization process for RXB at power-on
******************************************************************************/
void F_Test_Flow()
{
	if((gtc154.tsleep_info.u8SleepFlage == SLEEPCTL_WAKEUP) && (!GPIO_READ(WAKEUP_PORT, WAKEUP_PIN16)))
	{
		S_Flow = WAKEUP;
		S_Step = SRUN;
		GPIO_HIGH(LED_PORT, LED_PIN4);
		GPIO_HIGH(LED_PORT, LED_PIN5);
		GPIO_HIGH(LED_PORT, LED_PIN8);
		GPIO_HIGH(LED_PORT, LED_PIN9);

	    tgtestflow_info.u8status = STATE_TESTFLOW_INIT;
	    tgtestflow_info.u8SW_version_check = 0x00;
	    tgtestflow_info.u8pnael_check = 0x00;
	    tgtestflow_info.u8I2C_init_check = 0x00;
	}
	else if((gtc154.tsleep_info.u8SleepFlage == SLEEPCTL_SLEEP) && (GPIO_READ(WAKEUP_PORT, WAKEUP_PIN16)))
	{
		S_Flow = TESTFLOW_END;
		S_Step = SRUN;

		GPIO_LOW(LED_PORT, LED_PIN4);
		GPIO_LOW(LED_PORT, LED_PIN5);
		GPIO_LOW(LED_PORT, LED_PIN8);
		GPIO_LOW(LED_PORT, LED_PIN9);

		GPIO_LOW(ADC3100_PORT, ADC3100_PIN0_RESET);
		GPIO_LOW(SERDES_PORT, SERDES_PIN13_PWDNB);

		GPIO_LOW(WAKEUP_PORT, WAKEUP_PIN16);
	}

	// State Machine of Test_Flow
	switch(S_Flow)
	{
		// wake up CSD
		case WAKEUP:
			switch(S_Step)
			{
				// First time in the state, excute some commands
				case SRUN:
					GPIO_HIGH(WAKEUP_PORT, WAKEUP_PIN16);
					S_Step = SWAIT;
					break;

				// Wait 500 ms
				case SWAIT:
					Fun_Delay_Millisecond(500);
					S_Step = SCHECK;
					break;

				// Go to next flow
				case SCHECK:
					GPIO_LOW(LED_PORT, LED_PIN8);

					tgtestflow_info.u8I2C_init_check = 0x00;
					S_Flow = SET_I2C;
					S_Step = SRUN;
					break;
				default:
					break;
			};
			break;

		// Setup SERDES, Buzzer
		case SET_I2C:
			switch(S_Step)
			{
				// First time in the state, excute some commands
				case SRUN:
					GPIO_HIGH(SERDES_PORT, SERDES_PIN13_PWDNB);
					Fun_Delay_Millisecond(25);
					GPIO_LOW(SERDES_PORT, SERDES_PIN13_PWDNB);
					Fun_Delay_Millisecond(25);
					GPIO_HIGH(SERDES_PORT, SERDES_PIN13_PWDNB);
					Fun_Delay_Millisecond(25);
					Init_I2C_SERDES();

					en=Init_I2C_SERDES_Check();
					if(en==0x01)
					{
						tgtestflow_info.u8I2C_init_check += 0x01;
					}

					Fun_Delay_Millisecond(50);

					GPIO_HIGH(ADC3100_PORT, ADC3100_PIN0_RESET);
					Fun_Delay_Millisecond(25);
					GPIO_LOW(ADC3100_PORT, ADC3100_PIN0_RESET);
					Fun_Delay_Millisecond(25);
					GPIO_HIGH(ADC3100_PORT, ADC3100_PIN0_RESET);
					Fun_Delay_Millisecond(25);
					Init_I2C_Buzzer_ASC3100();

					en=Init_I2C_Buzzer_ASC3100_Check();
					if(en==0x01)
					{
						tgtestflow_info.u8I2C_init_check += 0x01;
					}

					if(tgtestflow_info.u8I2C_init_check == 0x00)
					{
						GPIO_LOW(LED_PORT, LED_PIN9);
					}

					tgtestflow_info.u8pnael_check = 0x00;
					en=0x01;
					S_Flow = SET_UART_Panel_ON;
					S_Step = SRUN;
					break;

				default:
					break;
			};
			break;

		// SET_UART_Panel_ON
		case SET_UART_Panel_ON:
			switch(S_Step)
			{
				// first time in the state, excute some commands
				case SRUN:
					if(en == 0x01)
					{
						Panel_power[0] = 0x01;
						count = sizeof(Panel_power)/sizeof(Panel_power[0]);
						u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), MID_PANEL_POWER, Panel_power, count);
						S_Step = SCHECK;
					}
					else if(en == 0x02)
					{
						count = sizeof(Panel_power)/sizeof(Panel_power[0]);
						u8ret = CSD_MESGFRAME_READ_noUDS((DEV_ADDRESS+READ), MID_PANEL_POWER, count);
						S_Step = SCHECK;
					}
					break;

				// Go to next flow
				case SCHECK:
					if(u8ret == CSD_FINISH)
					{
						if(en == 0x01)
						{
							en += 0x01;
							S_Step = SRUN;
						}
						else
						{
							if(CSD_Frame_response.u8buffer[0] != Panel_power[0])
							{
								tgtestflow_info.u8pnael_check += 0x01;
							}

							tgtestflow_info.u8SW_version_check = 0x00;
							en=0x01;
							S_Flow = GET_UART_Version;
							S_Step = SRUN;
						}
					}
					else
					{
						S_Step = SRUN;
					}
					break;

				default:
					break;
			};
			break;

		case GET_UART_Version:
		// Run Steps
			switch(S_Step)
			{
				// first time in the state, excute some commands
				case SRUN:
					count = 0x18;
					u8ret = CSD_MESGFRAME_READ_noUDS((DEV_ADDRESS+READ), MID_VERSION, count);
					S_Step = SCHECK;
					break;

				// Go to next flow
				case SCHECK:
					if(u8ret == CSD_FINISH)
					{
						if((CSD_Frame_response.u8buffer[0]!=0x56) ||
							(CSD_Frame_response.u8buffer[1]!=0x30) ||
							(CSD_Frame_response.u8buffer[2]!=0x32) ||
							(CSD_Frame_response.u8buffer[3]!=0x2e) ||
							(CSD_Frame_response.u8buffer[4]!=0x42) ||
							(CSD_Frame_response.u8buffer[5]!=0x33))
						{
							tgtestflow_info.u8SW_version_check += 0x01;
						}

						if(tgtestflow_info.u8SW_version_check == 0x00)
						{
							GPIO_LOW(LED_PORT, LED_PIN4);
						}

						en=0x01;
						S_Flow = SET_UART_Backlight_32;
						S_Step = SRUN;

						Fun_Delay_Millisecond(300);		//for backlight duty ready, RX seeting 1.5s form power on
					}
					else
					{
						S_Step = SRUN;
					}
					break;

				default:
					break;
			};
			break;

		// SET_UART_Backlight_50
		case SET_UART_Backlight_32:
			switch(S_Step)
			{
				// first time in the state, excute some commands
				case SRUN:
					if(en == 0x01)
					{
						Backlight_duty[0] = 0x32;
						count = sizeof(Backlight_duty)/sizeof(Backlight_duty[0]);
						u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), MID_BACKLIGHT_DUTY, Backlight_duty, count);
						S_Step = SCHECK;
					}
					else if(en == 0x02)
					{
						count = sizeof(Backlight_duty)/sizeof(Backlight_duty[0]);
						u8ret = CSD_MESGFRAME_READ_noUDS((DEV_ADDRESS+READ), MID_BACKLIGHT_DUTY, count);
						S_Step = SCHECK;
					}
					break;

				// Go to next flow
				case SCHECK:
					if(u8ret == CSD_FINISH)
					{
						if(en==0x01)
						{
							en += 0x01;
							S_Step = SRUN;
						}
						else if(en==0x02)
						{
							if(CSD_Frame_response.u8buffer[0] != Backlight_duty[0])
							{
								tgtestflow_info.u8pnael_check += 1;
							}

							en=0x01;
							S_Flow = SET_UART_Backlight_64;
							S_Step = SRUN;
						}
					}
					else
					{
						S_Step = SRUN;
					}
					break;

				default:
					break;

			};
			break;

		// SET_UART_Backlight_100
		case SET_UART_Backlight_64:
			switch(S_Step)
			{
				// first time in the state, excute some commands
				case SRUN:
					if(en == 0x01)
					{
						Backlight_duty[0] = 0x64;
						count = sizeof(Backlight_duty)/sizeof(Backlight_duty[0]);
						u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), MID_BACKLIGHT_DUTY, Backlight_duty, count);
						S_Step = SCHECK;
					}
					else if(en == 0x02)
					{
						count = sizeof(Backlight_duty)/sizeof(Backlight_duty[0]);
						u8ret = CSD_MESGFRAME_READ_noUDS((DEV_ADDRESS+READ), MID_BACKLIGHT_DUTY, count);
						S_Step = SCHECK;
					}
					break;

				// Go to next flow
				case SCHECK:
					if(u8ret == CSD_FINISH)
					{
						if(en==0x01)
						{
							en += 0x01;
							S_Step = SRUN;
						}
						else if(en==0x02)
						{
							if(CSD_Frame_response.u8buffer[0] != Backlight_duty[0])
							{
								tgtestflow_info.u8pnael_check += 1;
							}

							if(tgtestflow_info.u8pnael_check == 0x00)
							{
								GPIO_LOW(LED_PORT, LED_PIN5);
							}

							if((tgtestflow_info.u8SW_version_check  == 0x00) && (tgtestflow_info.u8pnael_check  == 0x00) && (tgtestflow_info.u8I2C_init_check  == 0x00))
							{
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

								uint8_t ADC_TxBuff[2] = { 0x00, 0x00};
								ADC_TxBuff[0]= 0x52;
								ADC_TxBuff[1]= 0x00;
								LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C0, SLA_ADDR_ADC3100, false);
								LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C0, ADC_TxBuff, 2, true, OSIF_WAIT_FOREVER);
							}
							else
							{
								tgtestflow_info.u8status = STATE_TESTFLOW_FAIL;

								Backlight_duty[0] = 0x00;
								count = sizeof(Backlight_duty)/sizeof(Backlight_duty[0]);
								u8ret = CSD_MESGFRAME_WRITE_noUDS((DEV_ADDRESS+WRITE), MID_BACKLIGHT_DUTY, Backlight_duty, count);
							}

							en=0x01;
							S_Flow = TESTFLOW_END;
							S_Step = SRUN;
						}
					}
					else
					{
						S_Step = SRUN;
					}
					break;

				default:
				break;

			};
			break;

		case TESTFLOW_END:
			break;

		default:
			S_Flow = WAKEUP; 
			S_Step = SRUN;
		break;
	}
}	
