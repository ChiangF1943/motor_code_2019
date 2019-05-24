/**
 ******************************************************************************
 * @file    Project/STM32F4xx_StdPeriph_Template/stm32f4xx_it.c
 * @author  MCD Application Team
 * @version V1.0.1
 * @date    13-April-2012
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "sys_config.h"
#include <stm32f4xx.h>

/** @addtogroup Template_Project
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief   This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void) {}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1) {}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1) {}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1) {}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void) {}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void) {}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void) {}

/**
 * @brief  This function handles SysTick Handler.6
 * @param  None
 * @retval None
 */
void SysTick_Handler(void) {}

#define OrderID 4
#define Instruction 7
#define SendParamStart 8
#define RevParamStart 9
uint8_t packetID;
uint32_t DataSendBuffer[128];
uint32_t PreDataSendBuffer[128];
	uint32_t ERROR_FLAG=0;

void USART_GetData(uint8_t Data, USART_DataTypeDef *USART_DataTypeStructure)
{
    if (USART_DataTypeStructure->Pointer < 4)
    {
        USART_DataTypeStructure->Rx_Buff[USART_DataTypeStructure->Pointer++] = Data;
        return;
    }
    if (USART_DataTypeStructure->Pointer == 4)
    {
        if ((USART_DataTypeStructure->Rx_Buff[0] == 0xFF) && (USART_DataTypeStructure->Rx_Buff[1] == 0xFF) && (USART_DataTypeStructure->Rx_Buff[2] == 0xFD) && (USART_DataTypeStructure->Rx_Buff[3] == 0x00))
        { // 正确检测到packet header
            USART_DataTypeStructure->Rx_Buff[USART_DataTypeStructure->Pointer++] = Data;
            USART_DataTypeStructure->Len                                         = Data;
        }
        else
        {
            USART_DataTypeStructure->Rx_Buff[0] = USART_DataTypeStructure->Rx_Buff[1];
            USART_DataTypeStructure->Rx_Buff[1] = USART_DataTypeStructure->Rx_Buff[2];
            USART_DataTypeStructure->Rx_Buff[2] = USART_DataTypeStructure->Rx_Buff[3];
            USART_DataTypeStructure->Rx_Buff[3] = Data;
        }
        return;
    }
    if (USART_DataTypeStructure->Pointer <= 6)
    {
        USART_DataTypeStructure->Rx_Buff[USART_DataTypeStructure->Pointer++] = Data;
        if (USART_DataTypeStructure->Pointer == 7)
        {
            USART_DataTypeStructure->Len = (USART_DataTypeStructure->Rx_Buff[6] << 8 | USART_DataTypeStructure->Rx_Buff[5]) + 7; // header + ID + len = 4byte + 1byte + 2byte = 7byte
        }
        return;
    }
    if (USART_DataTypeStructure->Pointer > 6)
    {
        if (USART_DataTypeStructure->Pointer < USART_DataTypeStructure->Len)
        {
            USART_DataTypeStructure->Rx_Buff[USART_DataTypeStructure->Pointer++] = Data;
            if (USART_DataTypeStructure->Pointer == USART_DataTypeStructure->Len)
            {
                // CRC校验放到主函数最后发送的地方做处理 建议丢掉第一个包
                //一个数据包接收完成，根据不同的instruction，分别做出不同的操作
                if (USART_DataTypeStructure->Rx_Buff[Instruction] == 0x83) // 0x83=SyncWrite，针对UART1，表示接收到上位机同步读指令
                {
										if(PeriodWriteFlag==1)
										{
											ERROR_FLAG++;
											U1_DataTypeStructure.RevCount=0;
											U2_DataTypeStructure.RevCount=0;
											U3_DataTypeStructure.RevCount=0;
											U4_DataTypeStructure.RevCount=0;
										}
                    TIM_SetCounter(TIM7, 0);
                    memcpy(&USART_DataTypeStructure->Temp_Rev[0], &USART_DataTypeStructure->Rx_Buff[12], ALL_ServoNum * 3); // ok
                    USART_DataTypeStructure->RevCount ++;
										
									// ok
                }
                if (USART_DataTypeStructure->Rx_Buff[Instruction] == 0x55) // 0x55=Status(Return)，针对UART2~4，表示接收到舵机状态返回包
                {
                    packetID = USART_DataTypeStructure->Rx_Buff[OrderID];
                    memcpy(NewRawData[SearchID(packetID,IDList)], USART_DataTypeStructure->Rx_Buff, USART_DataTypeStructure->Len);
                    USART_DataTypeStructure->RevCount++;
                }
                if (USART_DataTypeStructure->Rx_Buff[Instruction] == 0x84) // 0x84为个人特殊设定，针对UART1，表示接收到"打开"足底压力传感器指令
                {
                    FootFlag = 1;
                }
                if (USART_DataTypeStructure->Rx_Buff[Instruction] == 0x85) // 0x85为个人特殊设定，针对UART1，表示接收到"关闭"足底压力传感器指令
                {
                    FootFlag = 0;
                }
                if (USART_DataTypeStructure->Rx_Buff[Instruction] == 0x56 && FootFlag == 1) // 0x56为个人特殊设定，针对UART2~4，表示收到足底力传感器状态包
                {
											
                    if (USART_DataTypeStructure == &U4_DataTypeStructure) //左脚底板
                    {
                        memcpy(&DataSendBuffer[ALL_ServoNum], &USART_DataTypeStructure->Rx_Buff[RevParamStart], 8);
                        memcpy(NewRawData[ALL_ServoNum], USART_DataTypeStructure->Rx_Buff, USART_DataTypeStructure->Len);
                    }
                    if (USART_DataTypeStructure == &U3_DataTypeStructure) //右脚底板
                    {
                        memcpy(&DataSendBuffer[ALL_ServoNum + 2], &USART_DataTypeStructure->Rx_Buff[RevParamStart], 8);
                        memcpy(NewRawData[ALL_ServoNum + 1], USART_DataTypeStructure->Rx_Buff, USART_DataTypeStructure->Len);
                    }
                    USART_DataTypeStructure->RevCount++;
                }
                //接收和处理完成，接收指针和缓存清零
                USART_DataTypeStructure->Pointer = 0;
                memset(USART_DataTypeStructure->Rx_Buff, 0, 256 * sizeof(uint8_t));
            }
            else
            {
                return;
            }
        }
    }
}

void USART1_IRQHandler(void)
{
    uint8_t temp;
    if (USART_GetITStatus(USART1, USART_IT_ORE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_ORE);
        USART_ReceiveData(USART1);
    }
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        temp = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        USART_ClearFlag(USART1, USART_FLAG_RXNE);
        USART_GetData(temp, &U1_DataTypeStructure);
    }
}

void USART2_IRQHandler(void)
{
    uint8_t temp;
    if (USART_GetITStatus(USART2, USART_IT_ORE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_ORE);
        USART_ReceiveData(USART2);
    }
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        USART_ClearFlag(USART2, USART_FLAG_RXNE);
        temp = USART_ReceiveData(USART2);
        USART_GetData(temp, &U2_DataTypeStructure);
    }
}

void USART3_IRQHandler(void)
{
    uint8_t temp;
    if (USART_GetITStatus(USART3, USART_IT_ORE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_ORE);
        USART_ReceiveData(USART3);
    }
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        temp = USART_ReceiveData(USART3);
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        USART_ClearFlag(USART3, USART_FLAG_RXNE);
        USART_GetData(temp, &U3_DataTypeStructure);
    }
}

void UART4_IRQHandler(void)
{
    uint8_t temp;
    if (USART_GetITStatus(UART4, USART_IT_ORE) != RESET)
    {
        USART_ClearITPendingBit(UART4, USART_IT_ORE);
        USART_ReceiveData(UART4);
    }
    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        temp = USART_ReceiveData(UART4);
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
        USART_ClearFlag(UART4, USART_FLAG_RXNE);
        USART_GetData(temp, &U4_DataTypeStructure);
    }
}

void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
		
		tim7_flag += 1;
	}
	
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
{
}*/

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
