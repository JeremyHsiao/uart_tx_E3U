/*****************************************************************************
;       Program  : Uart0.h
;       Function : Declare LPUART0 Function & Variable
;       Chip     : NXP S32K144
;       Clock    : Internal 48MHz
;       Date     : 2018 / 12 / 28
;       Author   : Frank Chen
*****************************************************************************/
#ifndef __Uart0_H__
#define __Uart0_H__
//----------------------- Define Constant -------------------------------------//
#define SIZE_UART0_RX      512
//----------------------- Define Structure -------------------------------------//
typedef struct
{
    uint8_t ucRxBuffer[SIZE_UART0_RX];
    uint16_t uLength;
    bool bDataFlag;
}Uart0_Rx_Typedef;
//----------------------- Declare Extern Function ------------------------------//
extern Uart0_Rx_Typedef gtUart0_rx;
//----------------------- Declare Function ------------------------------------// 
void Uart0_Init(void);
void Uart0_Data_Send(uint8_t *puData, uint16_t uLen);
void Uart0_Byte_Get(uint8_t *puRxBuffer, uint16_t *puLen);
#endif
