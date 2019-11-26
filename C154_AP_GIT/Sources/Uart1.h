/******************************************************************************
;       Program  : Uart1.h
;       Function : Declare LPUart1 Function & Variable
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2019 / 01 / 25
;       Author   : Fenderson Lu
******************************************************************************/
#ifndef __Uart1_H__
#define __Uart1_H__
//---------------------------- Declare Support Function ----------------------//
//---------------------------- Define Constant ------------------------------//
#define	SIZE_UART1_RX      512
//---------------------------- Define Structure ------------------------------//
typedef struct
{
    uint8_t ucRxBuffer[SIZE_UART1_RX];
    uint16_t uLength;
    bool bDataFlag;
}Uart1_Rx_Typedef;
//--------------------------------Declare Debug Message -----------------------------------------//

//---------------------------- Declare Extern Function -----------------------//
extern Uart1_Rx_Typedef gtUart1_rx;
//---------------------------- Declare Function -----------------------------// 
void Uart1_Init(void);
void Uart1_Data_Send(uint8_t *puData, uint16_t uLen);
void Uart1_Byte_Get(uint8_t *puRxBuffer, uint16_t *puLen);
#endif
