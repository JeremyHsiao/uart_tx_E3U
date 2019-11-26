/******************************************************************************
;       Program  : I2c0.c
;       Function : I2C Master Access The Device 
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2018 / 01 / 18
;       Author   : Fenderson Lu
;       Describe : PTA2 = I2C0_SDA
;                  PTA3 = I2C0_SCL
******************************************************************************/
//---------------------------- Include File ---------------------------------//
#include "PinIO.H"
#include "I2c0.h"
//---------------------------- Declare External Var --------------------------//
lpi2c_master_state_t LpI2c0MasterState;				
uint8_t I2c0RxBuff[SIZE_I2C0_RX] = {0};

/******************************************************************************
* Name         :I2c0_Init
* Called by    :Main
* Return code  :N/A
* Description  :Initial I2C driver
******************************************************************************/
void I2c0_Init(void)
{
	LPI2C_DRV_MasterInit(INST_LPI2C0, &lpi2c0_MasterConfig0,&LpI2c0MasterState);		// 400KHz
}

/******************************************************************************
* Name         :I2c0_Master_Byte_Read
* Called by    :N/A
* Return code  :I2c0RxBuff[0]
* Description  :Use I2C Read 1 Byte data from uSlaveAddr
******************************************************************************/
uint8_t I2c0_Master_Byte_Read(uint16_t uSlaveAddr, uint8_t *ucRegister)				// Reg = 1-byte
{
	LPI2C_DRV_MasterSetSlaveAddr(INST_LPI2C0, uSlaveAddr, false);
	LPI2C_DRV_MasterSendDataBlocking(INST_LPI2C0, ucRegister, SIZE_A_BYTE_REG, true, OSIF_WAIT_FOREVER);		// Send a packet of data to the bus slave		
	LPI2C_DRV_MasterReceiveDataBlocking(INST_LPI2C0, &I2c0RxBuff[0], LEN_A_BYTE, true, OSIF_WAIT_FOREVER);		// Request data from the bus slave
	return I2c0RxBuff[0];
}

/******************************************************************************
* Name         :I2c0_BuffRx_Get
* Called by    :N/A
* Return code  :N/A
* Description  :return I2c0RxBuff
******************************************************************************/
uint8_t I2c0_BuffRx_Get(uint8_t ucIndex)
{
	return I2c0RxBuff[ucIndex];
}



