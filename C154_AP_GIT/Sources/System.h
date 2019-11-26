/******************************************************************************
;       Program  : System.h
;       Function : Declare System Typedef Function & Variable
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2019 / 01 / 11
;       Author   : Frank Chen
******************************************************************************/
#ifndef __System_H__
#define __System_H__
//---------------------------- Define Structure ------------------------------//
typedef struct
{
}Panel_TypeDef;

typedef struct
{
	uint8_t u8duty_cycle;
	bool bDutyReadyFlag;
}Backlight_Typedef;

typedef struct
{
	uint8_t u8power_switch;
	bool bSwitchReadyFlag;
}Power_Management_TypeDef;

typedef struct
{
	uint8_t u8SleepFlage;
}Sleep_TypeDef;

typedef struct
{
	int8_t s8temperature;
}System_TypeDef;

typedef struct
{
	Panel_TypeDef tpanel_info;
	Backlight_Typedef tbacklight_info;
	Power_Management_TypeDef tpower_info;
	Sleep_TypeDef tsleep_info;
	System_TypeDef tsystem_info;
}C154_TypeDef;

typedef struct
{
	int32_t u32year;
	int8_t 	u8month;
	int8_t 	u8day;
	int8_t 	u8hour;
	int8_t 	u8minute;
	int8_t 	u8second;
}Time_TypeDef;
//---------------------------- Declare Function -----------------------------// 
#endif


