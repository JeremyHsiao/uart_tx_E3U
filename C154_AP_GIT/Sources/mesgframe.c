/******************************************************************************
;       Program  : mesgframe.c
;       Function : CSD UART Mesg Frame
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2018 / 05 / 16
;       Author   : CYS
******************************************************************************/
#include "PinIO.h"

static uint16_t read_data_count=0;
static uint16_t pre_read_data_count=0;

static uint16_t read_data_length=0;
static uint16_t pre_read_data_length=0;

CSD_Frame_Typedef CSD_Frame_response;

uint8_t SEND_FRAME[320];
uint8_t CRC_TEMP[320];

/******************************************************************************
* Name         :CSD_MESGFRAME_WRITE_noUDS
* Called by    :N/A
* Return code  :N/A
* Description  :Packeting write request UART packet on no-UDS
******************************************************************************/
uint8_t CSD_MESGFRAME_WRITE_noUDS(uint8_t DEV_ADDR, uint8_t MSG_ID, uint8_t *data, uint32_t data_length)
{
    uint8_t u8csd_sync = 0x79;
    uint8_t u8dev_addr = 0x6C;
    uint32_t u32send_frame_length = 0;
    uint32_t u32send_data_length = 0;
    uint32_t u32count = 0;
    uint32_t u32crc=0, u32crc_count=0;

    /* CSD Master Frame define DEV_ADDR is 0x6C */
    if(DEV_ADDR == 0x6C)
    {
         /* Divided into single frame or multi frame according to the input length */
        if(data_length<=249)
        {
            u32send_frame_length = 0;
            u32send_data_length=data_length;

            /* Store the first three bytes of the CSD frame*/
            SEND_FRAME[u32send_frame_length]=u8csd_sync;
            u32send_frame_length += 1;

            SEND_FRAME[u32send_frame_length]=u8dev_addr;
            u32send_frame_length += 1;

            SEND_FRAME[u32send_frame_length]=MSG_ID;
            u32send_frame_length += 1;

            /* Store the data needed to calculate the CRC */
            u32crc_count=0;

            CRC_TEMP[u32crc_count]=SEND_FRAME[2];
            u32crc_count+=1;

            /* When MSGID is 0x3c (UDS), NUMBER OF TPYE is fixed to 0xFF,
             * and other conditions are FrameType(value is "0", 4 BIT)+DataLength(12 BIT)+data(N Byte)+CRC(4 Byte) */
            if(MSG_ID == 0x3C)
            {
            	return CSD_NULL;
            }
            else
            {
                SEND_FRAME[u32send_frame_length]=u32send_data_length + 4;
                u32send_frame_length += 1;
            }

            /* Store the Data of the CSD frame*/
            for(u32count=0 ; u32count<u32send_data_length ; u32count++)
            {
                SEND_FRAME[u32send_frame_length]=data[u32count];
                u32send_frame_length += 1;

                CRC_TEMP[u32crc_count]=data[u32count];
                u32crc_count+=1;
            }

            /* Calculate the CRC */
            u32crc= CRC32_Calculation(CRC_TEMP, u32crc_count);
            SEND_FRAME[u32send_frame_length]= (uint8_t)((u32crc & 0xff000000)>>24);
            u32send_frame_length += 1;

            SEND_FRAME[u32send_frame_length]= (uint8_t)((u32crc & 0x00ff0000)>>16);
            u32send_frame_length += 1;

            SEND_FRAME[u32send_frame_length]= (uint8_t)((u32crc & 0x0000ff00)>>8);
            u32send_frame_length += 1;

            SEND_FRAME[u32send_frame_length]= (uint8_t)((u32crc & 0x000000ff));
            u32send_frame_length += 1;

            /* Use UART Send on S32K144 */
            Fun_Delay_Millisecond(10);

            for(int i=0; i<gtUart0_rx.uLength; i++)
            {
            	gtUart0_rx.ucRxBuffer[i]=0x00;
          	}
        	gtUart0_rx.uLength=0;

        	Uart0_Data_Send(&SEND_FRAME[0], u32send_frame_length);

        	Fun_Delay_Millisecond(40);

        	if(gtUart0_rx.bDataFlag == true)
        	{
        		gtUart0_rx.bDataFlag = false;

        		if (gtUart0_rx.ucRxBuffer[0] == 0xC3)
        		{
        			return CSD_FINISH;
        		}
        		else
        		{
        			return CSD_TIMEOUT;
        		}
        	}
        	else
        	{
        		return CSD_TIMEOUT;
        	}
        }
        else
        {
            return CSD_LENGTH_ERROR;
        }
    }
    else
    {
        return CSD_DEV_ADDR_ERROR;
    }

    return CSD_NULL;
}

/******************************************************************************
* Name         :CSD_MESGFRAME_WRITE_noUDS
* Called by    :N/A
* Return code  :N/A
* Description  :Packeting read request UART packet on no-UDS
******************************************************************************/
uint8_t CSD_MESGFRAME_READ_noUDS(uint8_t DEV_ADDR, uint8_t MSG_ID, uint32_t data_length)
{
    uint8_t u8csd_sync = 0x79;
    uint8_t u8dev_addr = 0x6D;
    uint32_t u32send_frame_length = 0;
    uint32_t u32send_data_length = 0;

    uint32_t u32count=0;
    uint32_t u32crc=0, CRC_count=0;

    /* CSD Slave Frame define DEV_ADDR is 0x6C */
    if(DEV_ADDR == 0x6D)
    {
        u32send_frame_length = 0;

        SEND_FRAME[u32send_frame_length]=u8csd_sync;
        u32send_frame_length += 1;

        SEND_FRAME[u32send_frame_length]=u8dev_addr;
        u32send_frame_length += 1;

        SEND_FRAME[u32send_frame_length]=MSG_ID;
        u32send_frame_length += 1;

        if(MSG_ID == 0x3D)
        {
            return CSD_NULL;
        }
        else
        {
            read_data_length = data_length;

            if(pre_read_data_length != 0)
            {
            	if(pre_read_data_length != read_data_length)
            	{
            		return CSD_NULL;
            	}
            	else
            	{
            		read_data_count=pre_read_data_count;
            		read_data_length=pre_read_data_length;
            	}
            }

            if(data_length<=249)
            {
                u32send_data_length=data_length;
            }
            else if(data_length<=4095)
            {
                if(read_data_count == 0)
                {
                    u32send_data_length=249;
                }
                else
                {
                    u32send_data_length=read_data_length-read_data_count;
                    if(u32send_data_length>250)
                    {
                        u32send_data_length=250;
                    }
                }
            }

            SEND_FRAME[u32send_frame_length]=(u32send_data_length+4);
            u32send_frame_length += 1;
        }

        Fun_Delay_Millisecond(10);

        for(int i=0; i<gtUart0_rx.uLength; i++)
        {
        	gtUart0_rx.ucRxBuffer[i]=0x00;
        }
        gtUart0_rx.uLength=0;

        Uart0_Data_Send(&SEND_FRAME[0], u32send_frame_length);

        Fun_Delay_Millisecond(40);

        if(gtUart0_rx.bDataFlag == true)
        {
        	gtUart0_rx.bDataFlag = false;

        	if(gtUart0_rx.ucRxBuffer[0] == 0xC3)
        	{
            	CRC_count = 0;
                CRC_TEMP[CRC_count] = MSG_ID;
                CRC_count+=1;

                CSD_Frame_response.u16length = 0;
                read_data_count = 0;
                for(u32count=1; u32count<gtUart0_rx.uLength; u32count++)
                {
                    if(u32count == (gtUart0_rx.uLength - 4))
                    {
                        u32crc= CRC32_Calculation(CRC_TEMP, CRC_count);

                        if((gtUart0_rx.ucRxBuffer[u32count + 0] == (uint8_t)((u32crc & 0xff000000)>>24)) &&
                           (gtUart0_rx.ucRxBuffer[u32count + 1] == (uint8_t)((u32crc & 0x00ff0000)>>16)) &&
                           (gtUart0_rx.ucRxBuffer[u32count + 2] == (uint8_t)((u32crc & 0x0000ff00)>>8))  &&
                           (gtUart0_rx.ucRxBuffer[u32count + 3] == (uint8_t)((u32crc & 0x000000ff))))
                        {
                            if(read_data_count == read_data_length)
                            {
                            	pre_read_data_count = 0;
                            	pre_read_data_length = 0;
                                read_data_count=0;
                                read_data_length=0;
                                return CSD_FINISH;
                            }
                            else
                            {
                                pre_read_data_count=read_data_count;
                                pre_read_data_length=read_data_length;
                                read_data_count=0;
                                read_data_length=0;
                                return CSD_MULTI_LOOP;
                            }
                        }
                        else
                        {
                        	pre_read_data_count = 0;
                        	pre_read_data_length = 0;
                            read_data_count=0;
                            read_data_length=0;
                        	return CSD_CRC_ERROR;
                        }
                    }
                    else
                    {
                        CSD_Frame_response.u8buffer[CSD_Frame_response.u16length] = gtUart0_rx.ucRxBuffer[u32count];
                        CSD_Frame_response.u16length += 1;
                        read_data_count += 1;

                        CRC_TEMP[CRC_count] = gtUart0_rx.ucRxBuffer[u32count];
                        CRC_count+=1;
                    }
                }
        	}
        	else
        	{
                return CSD_TIMEOUT;
        	}
        }
        else
        {
            return CSD_TIMEOUT;
        }
    }
    else
    {
        return CSD_DEV_ADDR_ERROR;
    }

    return CSD_NULL;
}






