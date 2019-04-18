#ifndef __IMU_H_
#define __IMU_H_

#include "sys_config.h"

void IMU_SPI_Init(void);
unsigned char IMU_SeRe(unsigned char data);
void ADIS_ReadData(void);
void SPI2_Init(void);

#define SPI1_CS0 GPIO_ResetBits(GPIOA, GPIO_Pin_4);
#define SPI1_CS1 GPIO_SetBits(GPIOA, GPIO_Pin_4);

#define SPI2_CS0 GPIO_ResetBits(GPIOB, GPIO_Pin_12);
#define SPI2_CS1 GPIO_SetBits(GPIOB, GPIO_Pin_12);

#define SPI3_CS0 GPIO_ResetBits(GPIOC, GPIO_Pin_13);
#define SPI3_CS1 GPIO_SetBits(GPIOC, GPIO_Pin_13);

#endif
