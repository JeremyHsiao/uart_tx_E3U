/******************************************************************************
;       Program  : CSD_Frame.h
;       Function : CSD frame procotol define
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2019 / 05 / 21
;       Author   : CYS
******************************************************************************/
#ifndef __CSDFRAME_H__
#define __CSDFRAME_H__
//---------------------------- Define Constant ------------------------------//
//Frame Format Define
#define SINGLE_FRAME                (uint8_t)0x00
#define MULTI_FRAME                 (uint8_t)0x10
#define CONSECUTIVE_FRAME           (uint8_t)0x20
#define SYNC                        (uint8_t)0x79
#define DEV_ADDRESS                 (uint8_t)0x6C
#define ACK                         (uint8_t)0xC3
#define READ                        (uint8_t)0x01
#define WRITE                       (uint8_t)0x00

//Byte Define
#define	BYTE_MSGID_DATASTART			(uint8_t)0
#define BYTE_CSD_SYNC               	(uint8_t)0
#define BYTE_CSD_DEV_ADDR       		(uint8_t)1
#define BYTE_CSD_MSG_ID          		(uint8_t)2
#define BYTE_CSD_NUMBEROFBYTE 			(uint8_t)3

//Length Define
#define LEN_MAX_UARTDATA 			251

#endif


