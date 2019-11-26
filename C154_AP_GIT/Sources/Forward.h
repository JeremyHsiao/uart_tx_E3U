/******************************************************************************
;       Program  : Forward.h
;       Function : Forward procotol form PC
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2019 / 05 / 21
;       Author   : CYS
******************************************************************************/
#ifndef __FORWARD_H__
#define __FORWARD_H__
//---------------------------- Define Constant ------------------------------//
#define	LEN_MID_SLEEPCTL			1
#define MAX_FORWARD_ST_NO               (uint8_t)4                          // The massage function has 4 states.

//State Define
#define STATE_FORWARD_INT               STATE_INIT
#define STATE_FORWARD_WAIT              (uint8_t)0x01
#define STATE_FORWARD_PROCUSS           (uint8_t)0x02
#define STATE_FORWARD_ERROR             (uint8_t)0x03

//Event Define
#define EVENT_UART1_NONE           	(uint8_t)0xFF
#define EVENT_UART1_FIRST           (uint8_t)0x01
#define EVENT_UART1_DATA            (uint8_t)0x02

//RXB Sleep Control Status Define
#define SLEEPCTL_WAKEUP            	(uint8_t)0x01
#define SLEEPCTL_SLEEP           	(uint8_t)0x00

//---------------------------- Declare Type Structure -------------------------//
typedef struct
{
    uint8_t u8sync;
    uint8_t u8device_address;
    uint8_t u8message_id;
    int16_t u16data_length;
    int16_t u16frame_count;
    uint8_t u8frame_type;
    uint8_t u8data[256];
    uint32_t u32crc32;
    int8_t u8readback_length;
}Forward_Package_TypeDef;

typedef struct
{
	Forward_Package_TypeDef tforward_package;
    uint8_t uds_dtc[6];
    uint8_t u8state;
    uint8_t u8UART_event;
    uint16_t u16Service_event;
    uint16_t u16DID_event;
    uint8_t u8key[2];
}Forward_TypeDef;

//---------------------------- Declare Extern  Constant -----------------------//
extern void(*const Forward_State_Machine[MAX_FORWARD_ST_NO])(void);
extern Forward_TypeDef gtforward_info;
extern C154_TypeDef gtc154;

//---------------------------- Declare Function -----------------------------//
bool Forward_DataPickUp(uint8_t u8message_id,uint8_t *pdata, uint8_t u8data_length, uint8_t frame_type);
bool UDS_CheckCRC(void);
void UDS_DataProcess(void);
bool UDS_DataPreprocess(void);
void Forward_ClearBuffer(uint8_t *pbuffer, uint16_t u16length);
void Forward_ClearPackage(void);
void Forward_ClearDTC(void);
void Forward_Parameter_Initial(void);
void Forward_Service_Initial(void);
void Forward_Service_Waitting(void);
void Forward_Service_Process(void);
void Forward_Service_Error(void);
#endif


