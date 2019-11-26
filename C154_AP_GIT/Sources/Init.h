#ifndef _SERDES_H_
#define _SERDES_H_

#define	SLA_ADDR_ADC3100		0x1B	/* I2C adress 0x36 */

void Init_I2C_Buzzer_ASC3100();
void Init_I2C_SERDES();

uint8_t Init_I2C_Buzzer_ASC3100_Check();
uint8_t Init_I2C_SERDES_Check();

#endif




