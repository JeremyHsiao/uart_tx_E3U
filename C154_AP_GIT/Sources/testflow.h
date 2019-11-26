#ifndef _TESTFLOW_H_
#define _TESTFLOW_H_

#define STATE_TESTFLOW_INIT       		(uint8_t)0x00
#define STATE_TESTFLOW_SUCCES			(uint8_t)0x01
#define STATE_TESTFLOW_FAIL             (uint8_t)0x02

typedef enum
{
  SLEEP,
  WAKEUP,
  SET_I2C,
  SET_UART_Panel_ON,
  SET_UART_Panel_OFF,
  SET_UART_Backlight_32,
  SET_UART_Backlight_64,
  GET_UART_Panel,
  GET_UART_Backlight,
  GET_UART_Temp,
  GET_UART_Version,
  MULTIFRAME_TEST,
  TESTFLOW_END
}Testflow_State;

typedef enum
{
  SRUN,
  SWAIT,
  SCHECK,
  SRUN1,
  SWAIT1,
  SCHECK1,
  WRITE_TX_FAIL,
  WRITE_RX_FAIL,
  WRITE_PANEL_FAIL,
  WRITE_BACKLIGHT_FAIL,
  WRITE_VERSION_ERROR
}STEPS_IN_State;

typedef struct
{
    uint8_t u8I2C_init_check;
    uint8_t u8SW_version_check;
    uint8_t u8pnael_check;
    uint8_t u8status;
}Test_Flow_TypeDef;

void F_Test_Flow();

extern Test_Flow_TypeDef tgtestflow_info;

#endif




