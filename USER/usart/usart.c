/**
 ******************************************************************************
 * @file    usart.c
 * @version V1.0
 * @date    2018-02-01
 * @brief   ????
 ******************************************************************************
 */
#include "./usart/usart.h"
uint8_t Feedback[256]; //反馈指令缓冲


void SendData(uint8_t *data,uint8_t len,USART_TypeDef *USARTx)
{
    uint8_t i;
    for(i=0;i<len;i++)
    {
        USART_SendData(USARTx, data[i]);
	    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) {};
    }
}

void SendOrder(uint32_t *databuffer,uint8_t len)
{
		uint8_t *data;
		data=(uint8_t *)databuffer;
    uint8_t i;
    uint16_t sum=0;
    uint8_t Checksum=0;
    Feedback[0]=0x12;
    Feedback[1]=0x34;
    Feedback[2]=len+6;
		sum=Feedback[2];
    memcpy(&Feedback[3],data,len);
    for(i=0;i<len;i++)
    {
        sum=sum+data[i];
    }
    Checksum=(uint8_t)(sum&0xFF);
    Feedback[3+len]=Checksum;
    Feedback[4+len]=0x1F;
    Feedback[5+len]=0x2F;
    SendData(Feedback,len+6,USART1);
}

void Send_Package(uint8_t ID, uint32_t Data)
{
    int i;
    uint16_t CRC_Data;
    Feedback[0]  = 0xFF;
    Feedback[1]  = 0xFF;
    Feedback[2]  = 0xFD;
    Feedback[3]  = 0x00;
    Feedback[4]  = ID;
    Feedback[5]  = 0x08;
    Feedback[6]  = 0x00;
    Feedback[7]  = 0x55;
    Feedback[8]  = 0x00;
    Feedback[9]  = (uint8_t)(Data)&0xFF;
    Feedback[10] = (uint8_t)(Data >> 8) & 0xFF;
    Feedback[11] = (uint8_t)(Data >> 16) & 0xFF;
    Feedback[12] = (uint8_t)(Data >> 24) & 0xFF;
    CRC_Data     = CRC_Acc(0, Feedback, 13);
    Feedback[13] = CRC_Data & 0x00FF;
    Feedback[14] = (CRC_Data >> 8) & 0x00FF;
    for (i = 0; i < 15; i++)
    {
        USART_SendData(DEBUG_USART, (uint8_t)Feedback[i]);
        while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET) {};
    }
}

void Sync_Send_Read(USART_TypeDef *USARTx, uint8_t StartID, uint8_t Num, uint8_t Type)
{
    uint16_t Len, CRC_Data, i;
    Feedback[0] = 0xFF;
    Feedback[1] = 0xFF;
    Feedback[2] = 0xFD;
    Feedback[3] = 0x00;
    Feedback[4] = 0xFE;
    Len         = Num  + 7;
    Feedback[5] = (uint8_t)Len & 0xFF;
    Feedback[6] = (uint8_t)(Len >> 8) & 0xFF;
    if (Type == 0) /// Read Pos
    {
        Feedback[7] = 0x82;
        Feedback[8] = 0x84;
        Feedback[9] = 0x00;
    }
    if (Type == 1) /// Read Speed
    {
        Feedback[7] = 0x82;
        Feedback[8] = 0x80;
        Feedback[9] = 0x00;
    }
    Feedback[10] = 0x04;
    Feedback[11] = 0x00;
    for (i = 0; i < Num; i++) { Feedback[12 + i] = StartID + i; }
    CRC_Data           = CRC_Acc(0, Feedback, 12 + Num);
    Feedback[12 + Num] = (uint8_t)CRC_Data & 0xFF;
    Feedback[13 + Num] = (uint8_t)(CRC_Data >> 8) & 0xFF;
		if(USARTx==USART2)
			B485_1_S;
		else if(USARTx==USART3)
			B485_2_S;
		else if(USARTx==UART4)
			B485_3_S;
    for (i = 0; i < 14 + Num; i++)
    {
        USART_SendData(USARTx, (uint8_t)Feedback[i]);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) {};
    }
		delay_us(10);
		if(USARTx==USART2)
			B485_1_R;
		else if(USARTx==USART3)
			B485_2_R;
		else if(USARTx==UART4)
			B485_3_R;
}

void Sync_Send_Write(USART_TypeDef *USARTx, uint8_t *Data, uint8_t Num)
{
    uint16_t Len, CRC_Data, i;
    Feedback[0]  = 0xFF;
    Feedback[1]  = 0xFF;
    Feedback[2]  = 0xFD;
    Feedback[3]  = 0x00;
    Feedback[4]  = 0xFE;
    Len          = Num * 5 + 7;
    Feedback[5]  = (uint8_t)Len & 0xFF;
    Feedback[6]  = (uint8_t)(Len >> 8) & 0xFF;
    Feedback[7]  = 0x83;
    Feedback[8]  = 0x74;
    Feedback[9]  = 0x00;
    Feedback[10] = 0x04;
    Feedback[11] = 0x00;
    memcpy(&Feedback[12], Data, Num * 5);
    CRC_Data               = CRC_Acc(0, Feedback, 12 + Num * 5);
    Feedback[12 + Num * 5] = (uint8_t)CRC_Data & 0xFF;
    Feedback[13 + Num * 5] = (uint8_t)(CRC_Data >> 8) & 0xFF;
		if(USARTx==USART2)
			B485_1_S;
		else if(USARTx==USART3)
			B485_2_S;
		else if(USARTx==UART4)
			B485_3_S;
    for (i = 0; i < 14 + Num * 5; i++)
    {
        USART_SendData(USARTx, (uint8_t)Feedback[i]);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) {};
    }
		delay_us(10);
		if(USARTx==USART2)
			B485_1_R;
		else if(USARTx==USART3)
			B485_2_R;
		else if(USARTx==UART4)
			B485_3_R;
}

void Sync_Send_Init(USART_TypeDef *USARTx, uint8_t StartID,uint8_t Num)
{
    uint16_t Len, CRC_Data, i;
    Feedback[0]  = 0xFF;
    Feedback[1]  = 0xFF;
    Feedback[2]  = 0xFD;
    Feedback[3]  = 0x00;
    Feedback[4]  = 0xFE;
    Len          = Num * 2 + 7;
    Feedback[5]  = (uint8_t)Len & 0xFF;
    Feedback[6]  = (uint8_t)(Len >> 8) & 0xFF;
    Feedback[7]  = 0x83;
    Feedback[8]  = 0x40;
    Feedback[9]  = 0x00;
    Feedback[10] = 0x01;
    Feedback[11] = 0x00;
    for(i=0;i<Num;i++)
		{
			Feedback[12+2*i]=StartID+i;
			Feedback[13+2*i]=0x01;
		}
    CRC_Data               = CRC_Acc(0, Feedback, 12 + Num * 2);
    Feedback[12 + Num * 2] = (uint8_t)CRC_Data & 0xFF;
    Feedback[13 + Num * 2] = (uint8_t)(CRC_Data >> 8) & 0xFF;
		if(USARTx==USART2)
			B485_1_S;
		else if(USARTx==USART3)
			B485_2_S;
		else if(USARTx==UART4)
			B485_3_S;
    for (i = 0; i < 14 + Num * 2; i++)
    {
        USART_SendData(USARTx, (uint8_t)Feedback[i]);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) {};
    }
		delay_us(10);
		if(USARTx==USART2)
			B485_1_R;
		else if(USARTx==USART3)
			B485_2_R;
		else if(USARTx==UART4)
			B485_3_R;
}



int fputc(int ch, FILE *f)
{
    USART_SendData(DEBUG_USART, (uint8_t)ch);
    while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET) {};
    return (ch);
}

int fgetc(FILE *f)
{
    while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET) {};
    return (int)USART_ReceiveData(DEBUG_USART);
}

/*********************************************END OF FILE**********************/
