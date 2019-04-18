#ifndef __SYS_CONFIG_H_
#define __SYS_CONFIG_H_
#include "stdint.h"
#include "string.h"
#include "./motor/motor.h"
#include "./sys/sys.h"
#include "./usart/usart.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

typedef struct
{
  uint16_t Pointer;
  uint8_t Rx_Buff[256]; 
  uint16_t Len;
  uint16_t Count;
  uint16_t CRC_Value;
  uint8_t RevCount;
  uint8_t Temp_Rev[256];
  uint32_t DataArray[48];
} USART_DataTypeDef;

extern uint16_t TargetCCRValue[5];
extern int WorkFlag;
extern int16_t Motor_Speed[4];
extern USART_DataTypeDef U1_DataTypeStructure;
extern USART_DataTypeDef U3_DataTypeStructure;
extern USART_DataTypeDef U4_DataTypeStructure;
extern USART_DataTypeDef U5_DataTypeStructure;
extern uint32_t DataArray[48];
extern uint32_t DataSendBuffer[128];
#endif