#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "stm32f4xx.h"
#include "sys_config.h"
#define DEBUG_USART USART1

void Send_Package(uint8_t ID, uint32_t Data);
void SendData(uint8_t *data, uint8_t len, USART_TypeDef *USARTx);
void SendOrder(uint32_t *databuffer, uint8_t len);
void Sync_Send_Init(USART_TypeDef *USARTx, uint8_t Num, uint8_t *idList);
void Sync_Send_Write(USART_TypeDef *USARTx, uint8_t *Data, uint8_t Num);
void Sync_Send_Read(USART_TypeDef *USARTx, uint8_t Num, uint8_t *idList);
void Set_Fixed_Motor_Limit(uint16_t Data, char InstructionCode);
#endif /* __USART_H */
