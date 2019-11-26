/******************************************************************************
;       Program  : mesgframe.h
;       Function : CSD UART Mesg Frame
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2018 / 05 / 16
;       Author   : CYS
******************************************************************************/
#ifndef _MULTIFRAME_H_
#define _MULTIFRAME_H_

#define SINGLE_FRAME_FORMAT   		(uint8_t)0
#define MULTI_FRAME_FORMAT    		(uint8_t)1
#define NON_CSD_FORMAT    			(uint8_t)2

#define CSD_FINISH 					(uint8_t)0
#define CSD_TIMEOUT 				(uint8_t)1
#define CSD_LENGTH_ERROR 			(uint8_t)2
#define CSD_DEV_ADDR_ERROR 			(uint8_t)3
#define CSD_MULTI_LOOP 				(uint8_t)4
#define CSD_CRC_ERROR 				(uint8_t)5
#define CSD_NULL 					(uint8_t)6

uint8_t CSD_MESGFRAME_WRITE_noUDS(uint8_t DEV_ADDR, uint8_t MSG_ID, uint8_t *data, uint32_t data_length);
uint8_t CSD_MESGFRAME_READ_noUDS(uint8_t DEV_ADDR, uint8_t MSG_ID, uint32_t data_length);

typedef struct
{
    uint8_t u8buffer[256];
    uint16_t u16length;
}CSD_Frame_Typedef;

extern CSD_Frame_Typedef CSD_Frame_response;

#endif




