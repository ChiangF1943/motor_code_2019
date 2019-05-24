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
#include ".\IMU\IMU.h"

#define BaudRate 1000000

#define ALL_ServoNum 18 //不带压力板的舵机总数
#define U2_ServoNum 6 //仅指舵机
#define U3_ServoNum 6  //仅指舵机
#define U4_ServoNum 6   //仅指舵机
#define Velocity_Limit ('v')
#define Torque_Limit ('t')

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

extern USART_DataTypeDef U1_DataTypeStructure;
extern USART_DataTypeDef U2_DataTypeStructure;
extern USART_DataTypeDef U3_DataTypeStructure;
extern USART_DataTypeDef U4_DataTypeStructure;

extern uint8_t *DataArray;
extern uint8_t *pDataSendBuffer;
extern uint8_t NewRawData[30][40]; //算上脚底，总共24个status packet, 每个包最长30byte.
extern uint32_t DataSendBuffer[128];
extern uint32_t PreDataSendBuffer[128];

extern uint8_t tim7_flag;
extern uint8_t IDList[30];
extern int PeriodWriteFlag;
extern int FootFlag;

extern uint16_t speed_limit_buff[30];
extern uint16_t torque_limit_buff[30];
extern unsigned char accraw_adis[6];
extern unsigned char gyroraw_adis[6];
extern unsigned char magnraw_adis[6];
#endif
