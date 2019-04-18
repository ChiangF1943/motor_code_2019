#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "sys_config.h"

void Peripherals_Init(void);
void Led1_Toggle(void);
void Led2_Toggle(void);

#define LED1_ON GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define LED1_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define LED2_ON GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define LED2_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_1)

#define B485_1_S GPIO_SetBits(GPIOC, GPIO_Pin_0)
#define B485_1_R GPIO_ResetBits(GPIOC, GPIO_Pin_0)
#define B485_2_S GPIO_SetBits(GPIOC, GPIO_Pin_1)
#define B485_2_R GPIO_ResetBits(GPIOC, GPIO_Pin_1)
#define B485_3_S GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define B485_3_R GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define B485_4_S GPIO_SetBits(GPIOC, GPIO_Pin_3)
#define B485_4_R GPIO_ResetBits(GPIOC, GPIO_Pin_3)

#endif
