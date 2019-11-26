/******************************************************************************
;       Program  : serdes.c
;       Function : Inital serdes function
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2018 / 05 / 16
;       Author   : CYS
******************************************************************************/
#include "PinIO.H"
#include "Init_serdes.h"
#include "Init_buzzer_adc3100.h"

uint8_t SERDES_TxBuff[3] = { 0x00, 0x00, 0x00};
uint8_t SERDES_RxBuff[3] = { 0x00, 0x00, 0x00};
uint8_t ADC_TxBuff[2] = { 0x00, 0x00};
uint8_t ADC_RxBuff[2] = { 0x00, 0x00};

/******************************************************************************
* Name         :Init_I2C_Buzzer_ASC3100
* Called by    :Need initial ADC3100 IC
* Return code  :N/A
* Description  :initial ADC3100 IC from ADC3100_init[]
******************************************************************************/
void Init_I2C_Buzzer_ASC3100()
{
	uint32_t count,index,temp;

	count = sizeof(ADC3100_init)/sizeof(ADC3100_init[0]);
	for (index=0;index<count;index++)
	{
		temp = ADC3100_init[index];
		ADC_TxBuff[0]= (uint8_t)((temp & 0xff00)>>8);
		ADC_TxBuff[1]= (uint8_t)((temp & 0x00ff));
		LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C0, SLA_ADDR_ADC3100, false);
		LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C0, ADC_TxBuff, 2, true, OSIF_WAIT_FOREVER);

		if((ADC_TxBuff[0]==0x01) && (ADC_TxBuff[1]==0x01))
		{
			Fun_Delay_Millisecond(100);
		}
	}
}

/******************************************************************************
* Name         :Init_I2C_Buzzer_ASC3100_Check
* Called by    :Need check ADC3100 IC setting
* Return code  :0x00(pass)/0x01(fail)
* Description  :Read ADC3100 IC Register
******************************************************************************/
uint8_t Init_I2C_Buzzer_ASC3100_Check()
{
	uint32_t count,index,temp;
	uint8_t write_error=0x00;

	// get TX code
	count = sizeof(ADC3100_init)/sizeof(ADC3100_init[0]);
	for (index=0; index<count; index++)
	{
		temp = ADC3100_init[index];

		ADC_TxBuff[0]= (uint8_t)((temp & 0xff00)>>8);
		ADC_TxBuff[1]= (uint8_t)((temp & 0x00ff));

		if(ADC_TxBuff[0] == 0x00)
		{
			LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C0, SLA_ADDR_ADC3100, false);
			LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C0, ADC_TxBuff, 2, true, OSIF_WAIT_FOREVER);
		}
		else
		{
			ADC_RxBuff[0]= (uint8_t)((temp & 0xff00)>>8);
			ADC_RxBuff[1]= 0;

			LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C0, SLA_ADDR_ADC3100, false);
			LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C0, ADC_RxBuff, 1, true, OSIF_WAIT_FOREVER);
			LPI2C_DRV_MasterReceiveDataBlocking(INST_LPI2C0, &ADC_RxBuff[1], 1, true, OSIF_WAIT_FOREVER);

			if(!((ADC_TxBuff[0]==0x01) && (ADC_TxBuff[1]==0x01)))
			{
				if(ADC_RxBuff[1]!=ADC_TxBuff[1])
				{
					write_error=0x01;
				}
			}
		}
	}

	if(write_error==0x00)
	{
		return 0x00;
	}
	else
	{
		return 0x01;
	}
}

/******************************************************************************
* Name         :Init_I2C_SERDES
* Called by    :Need initial SERDES-96751/96752 IC
* Return code  :N/A
* Description  :initial SERDES-96751/96752 IC from SERDES_init[]
******************************************************************************/
void Init_I2C_SERDES()
{
	uint32_t count,index,temp;
	uint8_t I2C_address;

	count = sizeof(SERDES_init)/sizeof(SERDES_init[0]);
	for (index=0;index<count;index++)
	{	
		temp = SERDES_init[index];

		I2C_address= (uint8_t)((temp & 0xff000000)>>24);

		SERDES_TxBuff[0]= (uint8_t)((temp & 0x00ff0000)>>16);
		SERDES_TxBuff[1]= (uint8_t)((temp & 0x0000ff00)>>8);
		SERDES_TxBuff[2]= (uint8_t)((temp & 0x000000ff));

		LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C0, I2C_address, false);
		LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C0, SERDES_TxBuff, 3, true, OSIF_WAIT_FOREVER);

		if((SERDES_TxBuff[0]==0x00) && (SERDES_TxBuff[1]==0x10) && (SERDES_TxBuff[2]==0x30))
		{
			Fun_Delay_Millisecond(150);
		}
	}
}

/******************************************************************************
* Name         :Init_I2C_SERDES
* Called by    :Need check SERDES-96751/96752 IC setting
* Return code  :0x00(pass)/0x01(fail)
* Description  :Read SERDES-96751/96752 IC Register
******************************************************************************/
uint8_t Init_I2C_SERDES_Check()
{
	uint32_t count,index,temp;
	uint8_t write_error=0x00;
	uint8_t I2C_address;

	count = sizeof(SERDES_init)/sizeof(SERDES_init[0]);
	for (index=0;index<count;index++)
	{
		temp = SERDES_init[index];

		I2C_address= (uint8_t)((temp & 0xff000000)>>24);

		SERDES_TxBuff[0]= (uint8_t)((temp & 0x00ff0000)>>16);
		SERDES_TxBuff[1]= (uint8_t)((temp & 0x0000ff00)>>8);
		SERDES_TxBuff[2]= (uint8_t)((temp & 0x000000ff));

		/* Ignore SERDES GPIO & reset at deserializer Check */
		if(((SERDES_TxBuff[0]==0x02) && (SERDES_TxBuff[1]==0x12)) ||
		   ((SERDES_TxBuff[0]==0x02) && (SERDES_TxBuff[1]==0x14)) ||
		   ((SERDES_TxBuff[0]==0x02) && (SERDES_TxBuff[1]==0x1E)) ||
		   ((SERDES_TxBuff[0]==0x00) && (SERDES_TxBuff[1]==0x10)))
		{
			SERDES_RxBuff[0]= 0;
			SERDES_RxBuff[1]= 0;
			SERDES_RxBuff[2]= 0;
		}
		else
		{
			SERDES_RxBuff[0]= (uint8_t)((temp & 0x00ff0000)>>16);
			SERDES_RxBuff[1]= (uint8_t)((temp & 0x0000ff00)>>8);
			SERDES_RxBuff[2]= 0;

			LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C0, I2C_address, false);
			LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C0, SERDES_RxBuff, 2, true, OSIF_WAIT_FOREVER);
			LPI2C_DRV_MasterReceiveDataBlocking(INST_LPI2C0, &SERDES_RxBuff[2], 1, true, OSIF_WAIT_FOREVER);

			if(SERDES_RxBuff[2]!=SERDES_TxBuff[2])
			{
				write_error=0x01;
			}
		}
	}

	if(write_error==0x00)
	{
		return 0x00;
	}
	else
	{
		return 0x01;
	}
}






