/******************************************************************************
;       Program  : Uds.h
;       Function : Declare UDS Protocol Function & Variable
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2019 / 05 / 16
;       Author   : CYS
******************************************************************************/
#ifndef __Uds_H__
#define __Uds_H__
//---------------------------- Define Constant ------------------------------//
//RXB MSG ID Define
#define MID_REQUEST_UDS				(uint8_t)0x3C
#define MID_RESPONSE_UDS       	    (uint8_t)0x3D
#define MID_DIAGNOSIS_UDS			(uint8_t)0x3E
#define MID_PANEL_POWER				(uint8_t)0x80
#define MID_BACKLIGHT_DUTY			(uint8_t)0x81
#define MID_TEMPERATURE				(uint8_t)0x82
#define MID_VERSION					(uint8_t)0x83
#define MID_RESPONSE_DTC			(uint8_t)0x84
#define MID_REQUEST_DTC				(uint8_t)0x85
#define MID_SN_BARCODE				(uint8_t)0x88
#define MID_VOLUME_ADJUST			(uint8_t)0x8A
#define MID_BAT_VOLT				(uint8_t)0x8B
#define MID_BL_DERATING				(uint8_t)0x8C
#define MID_UPDATE_MCU             	(uint8_t)0x90

//RXB Factory MSG ID Define
#define	FMID_BLU_BARCODE					(uint8_t)0xB0
#define	FMID_PS_BARCODE						(uint8_t)0xB1
#define	FMID_EEPROM_ERASE_ALL				(uint8_t)0xB2
#define	FMID_TCON_BIST						(uint8_t)0xB3
#define	FMID_PMIC_WPN						(uint8_t)0xB4
#define	FMID_ICT_FCT						(uint8_t)0xB5
#define	FMID_BAT_SENSE						(uint8_t)0xB6
#define	FMID_BAT_SENSE_PRIORITY_SWITCH		(uint8_t)0xB7

//RXB Private MSG ID Define
#define PMID_BL_DR_SAVE				(uint8_t)0xD0

//TXB Factory MSG ID Define
#define FMID_SLEEPCTL               (uint8_t)0xA0

#endif


