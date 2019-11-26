/******************************************************************************
;       Program  : Fun.h
;       Function : Declare Scenario Functions
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2019 / 5 / 16
;       Author   : CYS
******************************************************************************/
#ifndef __Fun_H__
#define __Fun_H__
//---------------------------- Define Constant ------------------------------//
#define	BASE_DELAY					2986		// Based on 1ms, system clock 48MHz

//Test Button Status Define
#define TB_STATUS_NOT_TRIGGER     	(uint8_t)0x00
#define TB_STATUS_TRIGGER     		(uint8_t)0x01
#define TB_STATUS_FINISH     		(uint8_t)0x02

#define TCON_BIST_DISABLE    		(uint8_t)0x00
#define TCON_BIST_ENABLE   			(uint8_t)0x01

#define ICTFC_STATUS_INIT			(uint8_t)0x00
#define ICTFC_STATUS_FINISH			(uint8_t)0x01
#define ICTFC_PRIORITY_HIGH_DISABLE	(uint8_t)0x00
#define ICTFC_PRIORITY_HIGH_ENABLE	(uint8_t)0x01
#define ICTFC_SENSE_PUT_HIGH		(uint8_t)0x01

#define PMIC_PUT_LOW				(uint8_t)0x00
#define PMIC_PUT_HIGH				(uint8_t)0x01

#define BUZZER_MIN_LEVEL			(uint8_t)0x00
#define BUZZER_DEFAULT_LEVEL		(uint8_t)0x05
#define BUZZER_MAX_LEVEL			(uint8_t)0x0A
//---------------------------- Define Structure ------------------------------//
typedef struct
{
    uint8_t u8udata[256];
    uint8_t u8status;
    uint8_t u8SleepCnt;
    uint8_t u8SW308_TCON_BIST_status;
    uint8_t u8SW307_PMIC_status;
    uint8_t u8SW306_ICTFC_status;
    uint8_t u8SW305;
    uint8_t u8EB_PTC16_buzzer_volume_level;		//External button PTC15
}Test_Button_TypeDef;

//---------------------------- Declare Extern  Constant -----------------------//
extern Test_Button_TypeDef gttest_button;
extern Time_TypeDef gttime;

//---------------------------- Declare Function -----------------------------//
void Fun_Init(void);
uint8_t Fun_Down_Counter(uint8_t *ulCountValues);
void Fun_Delay_Millisecond(uint32_t ulTime);
void Fun_SW308(void);
void Fun_SW307(void);
void Fun_SW306(void);
void Fun_SW305(void);
void Fun_Complex_SW305_SW306(void);
void Fun_EB_Buzzer(void);
void Fun_Remove_Fun_Limit(void);
void Fun_World_Period_Count(void);
#endif


