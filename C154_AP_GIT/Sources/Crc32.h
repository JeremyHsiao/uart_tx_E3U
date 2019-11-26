/******************************************************************************
;       Program  : Crc32.h
;       Function : Declare CRC32 Function & Variable
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2019 / 01 / 11
;       Author   : Frank Chen
******************************************************************************/
#ifndef __Crc32_H__
#define __Crc32_H__ 
//---------------------------- Define Constant ------------------------------//
//---------------------------- Declare Function -----------------------------// 
uint32_t CRC32_Calculation(uint8_t *data,uint32_t size);
#endif




