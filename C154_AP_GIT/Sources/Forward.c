/******************************************************************************
;       Program  : Forward.c
;       Function : Forward procotol form PC
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2019 / 05 / 21
;       Author   : CYS
******************************************************************************/
#include "PinIO.h"

//---------------------------- Declare Global Variable ----------------------//
Forward_TypeDef gtforward_info;
C154_TypeDef gtc154;
uint8_t gu8transmit_data[256];
uint8_t u8temp[256];
uint8_t u8crc_temp[256];

/******************************************************************************
* Name         :Forward_State_Machine
* Called by    :Main
* Return code  :N/A
* Description  :Process for processing UART packets
******************************************************************************/
void(*const Forward_State_Machine[MAX_FORWARD_ST_NO])(void) =
{ Forward_Service_Initial, Forward_Service_Waitting, Forward_Service_Process, Forward_Service_Error};


/******************************************************************************
* Name         :Forward_DataPickUp
* Called by    :N/A
* Return code  :true/false
* Description  :Packeting Response UART packets
******************************************************************************/
bool Forward_DataPickUp(uint8_t u8message_id,uint8_t *pdata, uint8_t u8data_length, uint8_t frame_type)
{
	uint16_t temp = 0;
    uint16_t u16index = 0;
    uint8_t u8counter, u8_crc_count=0;
    uint32_t u32crc_temp;

    Forward_ClearBuffer(gu8transmit_data,sizeof(gu8transmit_data)/sizeof(uint8_t));

    gu8transmit_data[u16index] = ACK;
    u16index += 1;

    u8crc_temp[u8_crc_count] = u8message_id;
    u8_crc_count += 1;

    if((u8message_id == 0x3C) || (u8message_id == 0x3D) || (u8message_id == 0x3E))
    {
    	if((frame_type == SINGLE_FRAME) || (frame_type == MULTI_FRAME))
    	{
    		temp = (uint16_t)(frame_type << 8) + u8data_length;
    		gu8transmit_data[u16index] = (uint8_t)((temp & 0xff00)>>8);
    		u16index += 1;
    		gu8transmit_data[u16index] = (uint8_t)((temp & 0x00ff));
    		u16index += 1;

    		u8crc_temp[u8_crc_count] = (uint8_t)((temp & 0xff00)>>8);
    		u8_crc_count += 1;
    		u8crc_temp[u8_crc_count] = (uint8_t)((temp & 0x00ff));
    		u8_crc_count += 1;
    	}
    	else
    	{
    		gu8transmit_data[u16index] = frame_type;
    		u16index += 1;

    		u8crc_temp[u8_crc_count] = frame_type;
    		u8_crc_count += 1;
    	}
    }

    for(u8counter = 0; u8counter < u8data_length; u8counter++)
    {
    	gu8transmit_data[u16index] = pdata[u8counter];
    	u16index += 1;

    	u8crc_temp[u8_crc_count] = pdata[u8counter];
    	u8_crc_count += 1;


    }

	u32crc_temp = CRC32_Calculation(u8crc_temp, u8_crc_count);

	gu8transmit_data[u16index]= (uint8_t)((u32crc_temp & 0xff000000)>>24);
	u16index += 1;
	gu8transmit_data[u16index]= (uint8_t)((u32crc_temp & 0x00ff0000)>>16);
	u16index += 1;
	gu8transmit_data[u16index]= (uint8_t)((u32crc_temp & 0x0000ff00)>>8);
	u16index += 1;
	gu8transmit_data[u16index]= (uint8_t)((u32crc_temp & 0x000000ff));
	u16index += 1;

	gtforward_info.tforward_package.u8readback_length = u16index;

    return true;
}

/******************************************************************************
* Name         :Forward_DataProcess
* Called by    :Forward Service Process
* Return code  :N/A
* Description  :Execute TXB MSG command function
******************************************************************************/
void Forward_DataProcess(void)
{
	uint8_t u8ack = ACK;

	switch(gtforward_info.tforward_package.u8message_id)
	{
		case FMID_SLEEPCTL:
			if(gtforward_info.tforward_package.u8device_address == (DEV_ADDRESS + WRITE))
			{
				if(gtforward_info.tforward_package.u16data_length == LEN_MID_SLEEPCTL)
				{
					if(gtforward_info.tforward_package.u8data[BYTE_MSGID_DATASTART] == SLEEPCTL_WAKEUP)
					{
						gtc154.tsleep_info.u8SleepFlage = SLEEPCTL_WAKEUP;
						Uart1_Data_Send(&u8ack, 1);
					}
					else if(gtforward_info.tforward_package.u8data[BYTE_MSGID_DATASTART] == SLEEPCTL_SLEEP)
					{
						gtc154.tsleep_info.u8SleepFlage = SLEEPCTL_SLEEP;
						Uart1_Data_Send(&u8ack, 1);
					}
				}
			}
		break;

		default:
		break;
    }
}

/******************************************************************************
* Name         :Forward_DataPickUp
* Called by    :Forward Service Waitting
* Return code  :true/false
* Description  :Parsing request UART packets or Forward RXB MSG ID command
******************************************************************************/
bool Forward_DataPreprocess(void)
{
	uint32_t u32temp = 0;
    uint16_t u16counter = 0;
    uint16_t u16index = 0;
    uint8_t u8i;
    uint8_t u8counter = 0;
    uint8_t u8ack = ACK;

    u16index = 0;
    if(gtUart1_rx.ucRxBuffer[u16counter] == SYNC)
    {
    	gtforward_info.tforward_package.u8sync = gtUart1_rx.ucRxBuffer[u16counter + BYTE_CSD_SYNC];
    	gtforward_info.tforward_package.u8device_address = gtUart1_rx.ucRxBuffer[u16counter + BYTE_CSD_DEV_ADDR];
    	gtforward_info.tforward_package.u8message_id = gtUart1_rx.ucRxBuffer[u16counter + BYTE_CSD_MSG_ID];

    	switch(gtforward_info.tforward_package.u8message_id)
    	{
    	  	case FMID_SLEEPCTL:
    	  		if(gtforward_info.tforward_package.u8device_address == (DEV_ADDRESS + WRITE))
    	  		{
    	  		   	u8crc_temp[u8counter] = gtforward_info.tforward_package.u8message_id;
    	  		   	u8counter += 1;

    	  		   	gtforward_info.tforward_package.u8frame_type = SINGLE_FRAME;
    	  		   	gtforward_info.tforward_package.u16data_length = (gtUart1_rx.ucRxBuffer[u16counter + BYTE_CSD_NUMBEROFBYTE] - 4);
	  		   		gtforward_info.tforward_package.u16frame_count = 0;

	  		   		u16index = (BYTE_CSD_NUMBEROFBYTE + 1);
	  		   		for(u8i = 0; u8i < LEN_MAX_UARTDATA; u8i++)
	  		   		{
	  		        	gtforward_info.tforward_package.u8data[u8i] = gtUart1_rx.ucRxBuffer[u16index];
	  		        	u16index += 1;

	  		        	u8crc_temp[u8counter] = gtforward_info.tforward_package.u8data[u8i];
	  		        	u8counter += 1;

	  		        	gtforward_info.tforward_package.u16frame_count += 1;
	  		        	if(gtforward_info.tforward_package.u16frame_count == gtforward_info.tforward_package.u16data_length)
	  		        	{
	  		        		u8i = LEN_MAX_UARTDATA;
	  		        	}
	  		        }

    	  		   	gtforward_info.tforward_package.u32crc32 = 0;

    	  		   	for(u8i = 0 ; u8i < 4; u8i++)
    	  		   	{
    	  		   		gtforward_info.tforward_package.u32crc32 <<= 8;
    	  		   		gtforward_info.tforward_package.u32crc32 |= gtUart1_rx.ucRxBuffer[u16index++];
    	  		   	}

    	  		   	u32temp = CRC32_Calculation(u8crc_temp, u8counter);

    	  		   	if(u32temp == gtforward_info.tforward_package.u32crc32)
    	  		   	{
    	  		   		return true;
    	  		   	}
    	  		   	else
    	  		   	{
    	  		   		return false;
    	  		   	}
    	  		}
    	  		else
    	  		{
    	  			gtforward_info.tforward_package.u16data_length = (gtUart1_rx.ucRxBuffer[u16counter + BYTE_CSD_NUMBEROFBYTE] - 4);
    	  			return true;
    	  		}
    	   	break;

    	  	case MID_REQUEST_UDS:
    	    case MID_RESPONSE_UDS:
    	    case MID_DIAGNOSIS_UDS:
    	    case MID_PANEL_POWER:
    	    case MID_BACKLIGHT_DUTY:
    	    case MID_TEMPERATURE:
    	    case MID_VERSION:
    	    case MID_RESPONSE_DTC:
    	    case MID_REQUEST_DTC:
    	    case MID_SN_BARCODE:
    	    case MID_VOLUME_ADJUST:
    	    case MID_BAT_VOLT:
    	    case MID_BL_DERATING:
    	    case MID_UPDATE_MCU:
    	    case FMID_BLU_BARCODE:
    	    case FMID_PS_BARCODE:
    	    case FMID_EEPROM_ERASE_ALL:
    	    case FMID_TCON_BIST:
    	    	Uart0_Data_Send(gtUart1_rx.ucRxBuffer, gtUart1_rx.uLength);
    	    	Uart1_Data_Send(&u8ack, 1);
    	    	return true;
    	    break;

    	    default:
    	    	{
    	    		const uint8_t 	max_i2c_write_byte = (8+1), // slave_address_byte included
    	    						max_i2c_read_byte = 4,  // excluding slave_address_byte
    	    						END_BYTE = 0x20;
    	    		if(gtforward_info.tforward_package.u8device_address == (DEV_ADDRESS + WRITE))
    	    		{
    	    			// I2C write
            	    	if((gtforward_info.tforward_package.u8message_id<=max_i2c_write_byte) &&
            	    		(END_BYTE==gtUart1_rx.ucRxBuffer[BYTE_CSD_BYTESTART+gtforward_info.tforward_package.u8message_id]))
            	    	{
            	    		status_t i2c_status;
            	    		i2c_status = I2c0_Master_Array_Write(
            	    						(gtforward_info.tforward_package.u8message_id),
											(gtUart1_rx.ucRxBuffer+BYTE_CSD_BYTESTART));
                   	    	if(i2c_status==STATUS_SUCCESS)
                   	    	{
                   	    		Uart1_Data_Send(&u8ack, 1);
                     	    	return true;
                   	    	}
            	    	}
    	    		}
    	    		else if(gtforward_info.tforward_package.u8device_address == (DEV_ADDRESS + READ))
    	    		{
    	    			// I2C Read
    	    			uint8_t read_data[max_i2c_read_byte+1];
            	    	if((gtforward_info.tforward_package.u8message_id<=max_i2c_read_byte) &&
            	    		(END_BYTE==gtUart1_rx.ucRxBuffer[BYTE_CSD_BYTESTART+1]))
            	    	{
            	    		status_t i2c_status;
            	    		uint8_t uSlaveAddr = gtUart1_rx.ucRxBuffer[BYTE_CSD_BYTESTART];
            	    		i2c_status = I2c0_Master_Array_Read(uSlaveAddr,
            	    				(gtforward_info.tforward_package.u8message_id),read_data+1);
                   	    	if(i2c_status==STATUS_SUCCESS)
                   	    	{
                   	    		read_data[0] = u8ack;
                  	    		Uart1_Data_Send(read_data, (gtforward_info.tforward_package.u8message_id)+1);
                       	    	return true;
                   	    	}
             	    	}
    	    		}
//        	    	if((gtforward_info.tforward_package.u8message_id>0)&&(gtforward_info.tforward_package.u8message_id<=8))
//        	    	{
//        	    		status_t i2c_status;
//        	    		uint8_t read_back;
//        	    		const uint8_t uSlaveAddr = 0x1B, ucRegister = 0x00;
//
//        	    		uint8_t  set_page0[] = { uSlaveAddr, ucRegister, 0x00 };
//        	    		uint8_t  set_page1[] = { uSlaveAddr, ucRegister, 0x01 };
//        	    		i2c_status = I2c0_Master_Array_Write(sizeof(set_page0),set_page0);
//        	    		read_back = I2c0_Master_Byte_Read(uSlaveAddr, (uint8_t*)&ucRegister);
//        	    		Uart1_Data_Send((uint8_t*)&read_back, 1);
//
//        	    		i2c_status = I2c0_Master_Array_Write(sizeof(set_page1),set_page1);
//        	    		read_back = I2c0_Master_Byte_Read(uSlaveAddr, (uint8_t*)&ucRegister);
//        	    		Uart1_Data_Send((uint8_t*)&read_back, 1);
//
//        	    		/*
//
//       	    			status_t i2c_status;
//        	    		uint8_t read_back;
//        	    		const uint8_t uSlaveAddr = 0x1B, ucRegister = 0x00;
//
//        	    		uint8_t  set_page0[] = { uSlaveAddr, ucRegister, 0x00 };
//        	    		uint8_t  set_page1[] = { uSlaveAddr, ucRegister, 0x01 };
//        	    		i2c_status = I2c0_Master_Array_Write(sizeof(set_page0),set_page0);
//        	    		read_back = I2c0_Master_Byte_Read(uSlaveAddr, (uint8_t*)&ucRegister);
//        	    		Uart1_Data_Send((uint8_t*)&read_back, 1);
//
//        	    		i2c_status = I2c0_Master_Array_Write(sizeof(set_page1),set_page1);
//        	    		read_back = I2c0_Master_Byte_Read(uSlaveAddr, (uint8_t*)&ucRegister);
//        	    		Uart1_Data_Send((uint8_t*)&read_back, 1);
//
//
//////
//        	    		i2c_status = I2c0_Master_Array_Write((gtforward_info.tforward_package.u8message_id),
//        	    												(gtUart1_rx.ucRxBuffer+BYTE_CSD_MSG_ID+2));
//        	    		Uart1_Data_Send((gtUart1_rx.ucRxBuffer+BYTE_CSD_MSG_ID+2), (gtforward_info.tforward_package.u8message_id));
//        	    		Uart1_Data_Send((uint8_t*)i2c_status, sizeof(uint16_t));
//    */
//        	    		return true;
//        	    	}
    	    	}
    	    break;
    	}
    }
    return false;
}

/******************************************************************************
* Name         :Forward_ClearBuffer
* Called by    :After the UART packet event is processed or before the response is sent
* Return code  :N/A
* Description  :Clear buffer of UART packets
******************************************************************************/
void Forward_ClearBuffer(uint8_t *pbuffer, uint16_t u16length)
{
    uint16_t u16counter;
    for(u16counter = 0; u16counter < u16length; u16counter++)
        pbuffer[u16counter] = 0;
}

/******************************************************************************
* Name         :Forward_ClearPackage
* Called by    :After the UART packet event is processed
* Return code  :N/A
* Description  :Clear buffer of UART packets
******************************************************************************/
void Forward_ClearPackage(void)
{
	gtforward_info.tforward_package.u8sync = 0;
	gtforward_info.tforward_package.u8device_address = 0;
	gtforward_info.tforward_package.u8message_id = 0;
	gtforward_info.tforward_package.u8frame_type = 0;
	gtforward_info.tforward_package.u32crc32 = 0;
	Forward_ClearBuffer(gtforward_info.tforward_package.u8data, sizeof(gtforward_info.tforward_package.u8data)/sizeof(uint8_t));
}

/******************************************************************************
* Name         :Forward_ClearDTC
* Called by    :After the UART packet event is processed
* Return code  :N/A
* Description  :Clear buffer of UART packets
******************************************************************************/
void Forward_ClearDTC(void)
{
	Forward_ClearBuffer(gtforward_info.uds_dtc, sizeof(gtforward_info.uds_dtc)/sizeof(uint8_t));
}

/******************************************************************************
* Name         :Forward_Parameter_Initial
* Called by    :Forward Service Initial
* Return code  :N/A
* Description  :Forward parameter initial
******************************************************************************/
void Forward_Parameter_Initial(void)
{
	Forward_ClearDTC();
    Forward_ClearPackage();
}

/******************************************************************************
* Name         :Forward_Service_Initial
* Called by    :Forward State Machine
* Return code  :N/A
* Description  :Forward service initial
******************************************************************************/
void Forward_Service_Initial(void)
{
	Forward_Parameter_Initial();
    Uart1_Init();
    gtforward_info.u8state = STATE_FORWARD_WAIT;
    gtforward_info.u8UART_event = EVENT_UART1_FIRST;
}

/******************************************************************************
* Name         :Forward_Service_Waitting
* Called by    :Forward State Machine
* Return code  :N/A
* Description  :Wait parsing UART packet
******************************************************************************/
void Forward_Service_Waitting(void)
{
    if(gtUart1_rx.bDataFlag == true)
    {
    	gtforward_info.u8UART_event = EVENT_UART1_DATA;
    }

    switch(gtforward_info.u8UART_event)
    {
        case EVENT_UART1_FIRST:
        	Forward_ClearBuffer(gtUart1_rx.ucRxBuffer, sizeof(gtUart1_rx.ucRxBuffer)/sizeof(uint8_t));
        	gtUart1_rx.uLength = 0;
        	gtforward_info.u8UART_event = EVENT_UART1_NONE;
            break;
        case EVENT_UART1_DATA:
            if(Forward_DataPreprocess() == true)
            {
            	gtforward_info.u8state = STATE_FORWARD_PROCUSS;
            	gtforward_info.u8UART_event = EVENT_UART1_FIRST;
            }
            else
            {
            	gtforward_info.u8UART_event = EVENT_UART1_NONE;
            }
            Forward_ClearBuffer(gtUart1_rx.ucRxBuffer, sizeof(gtUart1_rx.ucRxBuffer)/sizeof(uint8_t));
            gtUart1_rx.uLength = 0;
            gtUart1_rx.bDataFlag = false;
            break;
        case EVENT_UART1_NONE:
        default:
            break;
    }
}

/******************************************************************************
* Name         :Forward_Service_Waitting
* Called by    :Forward State Machine
* Return code  :N/A
* Description  :Handle Forward service
******************************************************************************/
void Forward_Service_Process(void)
{

    switch(gtforward_info.u8UART_event)
    {
        case EVENT_UART1_FIRST:
        	Forward_DataProcess();
        	Forward_ClearPackage();
            gtforward_info.u8state = STATE_FORWARD_WAIT;
            gtforward_info.u8UART_event = EVENT_UART1_FIRST;
            break;
        case EVENT_UART1_NONE:
        default:
            break;
    }
}

/******************************************************************************
* Name         :Forward_Service_Error
* Called by    :Forward State Machine
* Return code  :N/A
* Description  :Handle Forward service error
******************************************************************************/
void Forward_Service_Error(void)
{
    switch(gtforward_info.u8UART_event)
    {
        case EVENT_UART1_FIRST:
            break;
        default:
            break;
    }
}

