#ifndef __SYS_CONFIG_H_
#define __SYS_CONFIG_H_
#include "stdint.h"
#include "./motor/motor.h"
#include "./sys/sys.h"
#include "./usart/usart.h"
#include ".\IMU\IMU.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
<<<<<<< HEAD
#include "string.h"

#define BaudRate 1000000
#define ALL_ServoNum 3
#define U2_ServoNum 3
#define U3_ServoNum 0
#define U4_ServoNum 0
#define ServoIDStart 0
=======
#include ".\IMU\IMU.h"
>>>>>>> 12286d8bf7e1105d32dcdc6a86d7d39d0ad1dd7b

#define BaudRate 1000000
#define ALL_ServoNum 3
#define U2_ServoNum 3
#define U3_ServoNum 0
#define U4_ServoNum 0
#define ServoIDStart 0
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
    uint8_t Tx_Buff[256];
} USART_DataTypeDef;

extern uint16_t TargetCCRValue[5];
extern int WorkFlag;
extern int16_t Motor_Speed[4];
extern USART_DataTypeDef U1_DataTypeStructure;
extern USART_DataTypeDef U2_DataTypeStructure;
extern USART_DataTypeDef U3_DataTypeStructure;
extern USART_DataTypeDef U4_DataTypeStructure;
extern uint32_t *DataArray;
extern uint32_t DataSendBuffer[128];
<<<<<<< HEAD
extern uint8_t Send_Cycle_Flag;
extern uint8_t Feedback[256];
=======
extern int SyncW_Flag;
>>>>>>> 12286d8bf7e1105d32dcdc6a86d7d39d0ad1dd7b

extern unsigned char accraw_adis[6];
extern unsigned char gyroraw_adis[6];
extern unsigned char magnraw_adis[6];
<<<<<<< HEAD

extern uint8_t U2_Tx_Buff[255];
extern uint8_t U3_Tx_Buff[255];
extern uint8_t U4_Tx_Buff[255];
=======
>>>>>>> 12286d8bf7e1105d32dcdc6a86d7d39d0ad1dd7b
#endif
