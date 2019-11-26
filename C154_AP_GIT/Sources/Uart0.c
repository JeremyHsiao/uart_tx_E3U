/*****************************************************************************
;       Program  : Uart0.c
;       Function : Communicate with SerDes by LPUART0  
;       Chip     : NXP S32K144
;       Clock    : Internal 48MHz
;       Date     : 2018 / 01 / 08
;       Author   : Frank Chen
;       Describe : PTC3 = UART_TX0
;                  PTC2 = UART_RX0
*****************************************************************************/
#include "PinIO.h"
//------------------- Declare Global Variable ----------------------------------//
uint16_t uTemp;
Uart0_Rx_Typedef gtUart0_rx;

/******************************************************************************
* Name         :LPUART0_RxTx_IRQHandler
* Called by    :INT_SYS_InstallHandler for LPUART1_RxTx_IRQn
* Return code  :N/A
* Description  :Function of UART0 Interrupt, receive data and clear status flag
******************************************************************************/
void LPUART0_RxTx_IRQHandler(void)
{
    // Transmit process
    if((LPUART0->STAT & LPUART_STAT_TDRE_MASK) == LPUART_STAT_TDRE_MASK)
    {

    }

    // Receive process
    if((LPUART0->STAT & LPUART_STAT_RDRF_MASK) == LPUART_STAT_RDRF_MASK)
    {
        Uart0_Byte_Get(gtUart0_rx.ucRxBuffer,&gtUart0_rx.uLength);				// Receive data
    }

    // Idle interrupt, data is received finish
    if((LPUART0->STAT & LPUART_STAT_IDLE_MASK) == LPUART_STAT_IDLE_MASK)
    {
        gtUart0_rx.bDataFlag = true;
        LPUART0->STAT |= LPUART_STAT_IDLE_MASK;
    }

    // Frame error flag clear
    if((LPUART0->STAT & LPUART_STAT_FE_MASK) == LPUART_STAT_FE_MASK)
    {
        LPUART0->STAT |= LPUART_STAT_FE_MASK ;
    }

    // Noise error flag clear
    if((LPUART0->STAT & LPUART_STAT_NF_MASK) == LPUART_STAT_NF_MASK)
    {
        LPUART0->STAT |= LPUART_STAT_NF_MASK;
    }

    // Overrun error flag clear
    if((LPUART0->STAT & LPUART_STAT_OR_MASK) == LPUART_STAT_OR_MASK)
    {
        LPUART0->STAT |= LPUART_STAT_OR_MASK;
    }

    // Parity error flag clear
    if((LPUART0->STAT & LPUART_STAT_PF_MASK) == LPUART_STAT_PF_MASK)
    {
        LPUART0->STAT |= LPUART_STAT_PF_MASK;
    }
}

/******************************************************************************
* Name         :Uart0_Init
* Called by    :Main
* Return code  :N/A
* Description  :Initial lpuart0 driver
******************************************************************************/
void Uart0_Init(void)
{
    LPUART_DRV_Init(INST_LPUART0, &lpuart0_State, &lpuart0_InitConfig0);    						// Initialize LPUART instance
    LPUART0->FIFO |= (LPUART_FIFO_RXFE_MASK | LPUART_FIFO_TXFE_MASK);			// Enable Rx and Tx FIFOs
    LPUART0->CTRL |= LPUART_CTRL_RE_MASK;										// Enable Rx and Tx functions
    LPUART0->CTRL |= LPUART_CTRL_RIE_MASK;                                  								// Enable Rx interrupt
    LPUART0->CTRL |= LPUART_CTRL_IDLECFG(3);
    LPUART0->CTRL |= LPUART_CTRL_ILIE_MASK;                                 								// Enable IDLE interrupt
    INT_SYS_InstallHandler(LPUART0_RxTx_IRQn, &LPUART0_RxTx_IRQHandler, (isr_t *)0);
    INT_SYS_EnableIRQ(LPUART0_RxTx_IRQn);
}

/******************************************************************************
* Name         :Uart0_Data_Send
* Called by    :Need use UART0 send data
* Return code  :N/A
* Description  :send puData form UART0
******************************************************************************/
void Uart0_Data_Send(uint8_t *puData, uint16_t uLen)
{
    LPUART_DRV_SendDataPolling(INST_LPUART0, &puData[0], uLen);
}

/******************************************************************************
* Name         :Uart0_Byte_Get
* Called by    :LPUART0_RxTx_IRQHandler
* Return code  :N/A
* Description  :Save LPUART0->DATA to puRxBuffer
******************************************************************************/
void Uart0_Byte_Get(uint8_t *puRxBuffer, uint16_t *puLen)
{
    puRxBuffer[*puLen] = LPUART0->DATA;
    *puLen += 1;
}


