/******************************************************************************
;       Program  : Init_buzzer_adc3100.h
;       Function : Setting of inital TX board TVL320ADC3100
;       Chip     : NXP S32K118
;       Clock    : Internal 48MHz
;       Date     : 2018 / 05 / 28
;       Author   : CYS
******************************************************************************/
#ifndef _INIT_BUZZER_ADC3100_H_
#define _INIT_BUZZER_ADC3100_H_

/* I2C address 0x36 */

uint32_t ADC3100_init[]={

/*
# Line-In (J9) Stereo Recording
# ---------------------------------------------
# For normal USB Audio, no hardware change is required.
# If using an external interface, SW2.4 and SW2.5 of the USB-MODEVM must
# be set to HI and clocks can be connected to J14 of the USB-MODEVM.
#
# ADC3101EVM Key Jumper Settings and Audio Connections:
# 1. Remove Jumpers W12 and W13
# 2. Insert Jumpers W4 and W5
# 3. Insert a 3.5mm stereo audio plug into J9 for
# single-ended input IN1L(P) - left channel and
# single-ended input IN1R(M) - right channel
*/

/*
# Software Reset
*/

/*
# Select Page 0
*/
0x0000,

/*
# Initialize the device through software reset
*/
0x0101,

/*
# Delay 100mS
*/

/*
# Clock Settings
# ---------------------------------------------
# The codec receives: MCLK = 12 MHz.
# The codec produces
# BCLK = 11.2896 MHz, WCLK = 44.1 kHz
*/

/*
# Select Page 0
*/
0x0000,

/*
#PLL_CLKIN=MCLK, CODEC_CLKIN=PLL_CLK
*/
0x0403,

/*
#PLL Powered up,P=1  R=1
*/
0x0591,

/*
#J=7
*/
0x0607,

/*
#D=5264
*/
0x0714,
0x0890,

/*
# NADC = 8
*/
0x1288,

/*
# MADC = 2
*/
0x1382,

/*
# AOSR = 128 (default)
*/

/*
# Audio Settings
*/

/*
# LJF mode, 32-bits, Master Mode (BCLK and WCLK are outputs), 3-stating of DOUT enabled
*/
0x1bfd,

/*
# BCLK inverted, BDIV_CLKIN=ADC_CLK
*/
0x1d0a,

/*
# BCLK N divider is powered up, N=
*/
0x1e81,

/*
# Signal Processing Settings
*/

/*
# Select Page 0
*/
0x0000,

/*
# Set the ADC Mode to PRB_P1 (default)
*/
0x3d01,

/*
# Recording Setup
*/

/*
# Left ADC Vol = 0dB
*/
0x5300,

/*
# Right ADC Vol = 0dB
*/
0x5400,

/*
# Select Page 1
*/
0x0001,

/*
# Mic Bias Powered Down
*/
0x3300,

/*
# Left ADC Input selection for Left PGA = IN2L(P) as Single-Ended
*/
0x34f3,

/*
# Right ADC Input selection for Right PGA = IN2R(M) as Single-Ended
*/
0x37f3,

/*
# Left Analog PGA Seeting = 0dB
*/
0x3b00,

/*
# Right Analog PGA Seeting = 0dB
*/
0x3c00,

/*
# Select Page 0
*/
0x0000,

/*
# Power-up Left ADC and Right ADC
*/
0x51c2,

/*
# mute Left and Right ADC Channels
*/
0x5288

};


#endif




