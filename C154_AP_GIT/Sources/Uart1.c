/******************************************************************************
;       Program  : Uart1.c
;       Function : Debug LPUart1 Message  
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2019 / 01 / 25
;       Author   : Fenderson Lu
;       Describe : PTC7 = UART_TX1
;                  PTC6 = UART_RX1
******************************************************************************/
//---------------------------- Include File ---------------------------------//
#include "PinIO.h"
//---------------------------- Declare Global Variable ------------------------//
uint16_t ucTemp;
Uart1_Rx_Typedef gtUart1_rx;

/******************************************************************************
* Name         :LPUART1_RxTx_IRQHandler
* Called by    :INT_SYS_InstallHandler for LPUART1_RxTx_IRQn
* Return code  :N/A
* Description  :Function of UART0 Interrupt, receive data and clear status flag
******************************************************************************/
void LPUART1_RxTx_IRQHandler(void)
{
    // Transmit process
    if((LPUART1->STAT & LPUART_STAT_TDRE_MASK) == LPUART_STAT_TDRE_MASK)
    {

    }

    // Receive process
    if((LPUART1->STAT & LPUART_STAT_RDRF_MASK) == LPUART_STAT_RDRF_MASK)
    {
        Uart1_Byte_Get(gtUart1_rx.ucRxBuffer,&gtUart1_rx.uLength);				// Receive data
    }

    // Idle interrupt, data is received finish
    if((LPUART1->STAT & LPUART_STAT_IDLE_MASK) == LPUART_STAT_IDLE_MASK)
    {
        gtUart1_rx.bDataFlag = true;
        LPUART1->STAT |= LPUART_STAT_IDLE_MASK;
    }

    // Frame error flag clear
    if((LPUART1->STAT & LPUART_STAT_FE_MASK) == LPUART_STAT_FE_MASK)
    {
        LPUART1->STAT |= LPUART_STAT_FE_MASK ;
    }

    // Noise error flag clear
    if((LPUART1->STAT & LPUART_STAT_NF_MASK) == LPUART_STAT_NF_MASK)
    {
        LPUART1->STAT |= LPUART_STAT_NF_MASK;
    }

    // Overrun error flag clear
    if((LPUART1->STAT & LPUART_STAT_OR_MASK) == LPUART_STAT_OR_MASK)
    {
        LPUART1->STAT |= LPUART_STAT_OR_MASK;
    }

    // Parity error flag clear
    if((LPUART1->STAT & LPUART_STAT_PF_MASK) == LPUART_STAT_PF_MASK)
    {
        LPUART1->STAT |= LPUART_STAT_PF_MASK;
    }
}

/******************************************************************************
* Name         :Uart1_Init
* Called by    :Main
* Return code  :N/A
* Description  :Initial lpuart1 driver
******************************************************************************/
void Uart1_Init(void)
{   
	LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0);								// Initialize LPUART1
    LPUART1->FIFO |= (LPUART_FIFO_RXFE_MASK | LPUART_FIFO_TXFE_MASK);			// Enable Rx and Tx FIFOs
    LPUART1->CTRL |= LPUART_CTRL_RE_MASK;										// Enable Rx and Tx functions
    LPUART1->CTRL |= LPUART_CTRL_RIE_MASK;                                  								// Enable Rx interrupt
    LPUART1->CTRL |= LPUART_CTRL_IDLECFG(3);
    LPUART1->CTRL |= LPUART_CTRL_ILIE_MASK;                                 								// Enable IDLE interrupt
    INT_SYS_InstallHandler(LPUART1_RxTx_IRQn, &LPUART1_RxTx_IRQHandler, (isr_t *)0);
    INT_SYS_EnableIRQ(LPUART1_RxTx_IRQn);
}

/******************************************************************************
* Name         :Uart1_Data_Send
* Called by    :Need use UART1 send data
* Return code  :N/A
* Description  :send puData form UART1
******************************************************************************/
void Uart1_Data_Send(uint8_t *puData, uint16_t uLen)
{
    LPUART_DRV_SendDataPolling(INST_LPUART1, &puData[0], uLen);
}

/******************************************************************************
* Name         :Uart1_Byte_Get
* Called by    :LPUART1_RxTx_IRQHandler
* Return code  :N/A
* Description  :Save LPUART1->DATA to puRxBuffer
******************************************************************************/
void Uart1_Byte_Get(uint8_t *puRxBuffer, uint16_t *puLen)
{
    puRxBuffer[*puLen] = LPUART1->DATA;
    *puLen += 1;
}



