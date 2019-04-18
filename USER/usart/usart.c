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

<<<<<<< HEAD
// DMA方式
void SendAll(uint32_t *databuffer, uint8_t len)
=======

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
>>>>>>> 12286d8bf7e1105d32dcdc6a86d7d39d0ad1dd7b
{
    uint8_t *data;
    data = (uint8_t *)databuffer;
    uint8_t i;
    uint16_t sum     = 0;
    uint8_t Checksum = 0;
    Feedback[0]      = 0x12;
    Feedback[1]      = 0x34;
    Feedback[2]      = len + 6;
    sum              = Feedback[2];
    memcpy(&Feedback[3], data, len);
    for (i = 0; i < len; i++) { sum = sum + data[i]; }
    Checksum          = (uint8_t)(sum & 0xFF);
    Feedback[3 + len] = Checksum;
    Feedback[4 + len] = 0x1F;
    Feedback[5 + len] = 0x2F;
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}

//使用DMA方式 需要区别发送缓存
void Sync_Send_Read(USART_TypeDef *USARTx, uint8_t StartID, uint8_t Num, uint8_t Type, uint8_t *SendBuffer)
{
    uint16_t Len, CRC_Data, i;
<<<<<<< HEAD
    if (Num != 0)
=======
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
>>>>>>> 12286d8bf7e1105d32dcdc6a86d7d39d0ad1dd7b
    {
        SendBuffer[0] = 0x11;
        SendBuffer[1] = 0xFF;
        SendBuffer[2] = 0xFD;
        SendBuffer[3] = 0x00;
        SendBuffer[4] = 0xFE;
        Len           = Num + 7;
        SendBuffer[5] = (uint8_t)Len & 0xFF;
        SendBuffer[6] = (uint8_t)(Len >> 8) & 0xFF;
        if (Type == 0) /// Read Pos
        {
            SendBuffer[7] = 0x82;
            SendBuffer[8] = 0x84;
            SendBuffer[9] = 0x00;
        }
        if (Type == 1) /// Read Speed
        {
            SendBuffer[7] = 0x82;
            SendBuffer[8] = 0x80;
            SendBuffer[9] = 0x00;
        }
        SendBuffer[10] = 0x04;
        SendBuffer[11] = 0x00;
        for (i = 0; i < Num; i++) { SendBuffer[12 + i] = StartID + i; }
        CRC_Data             = CRC_Acc(0, SendBuffer, 12 + Num);
        SendBuffer[12 + Num] = (uint8_t)CRC_Data & 0xFF;
        SendBuffer[13 + Num] = (uint8_t)(CRC_Data >> 8) & 0xFF;
        if (USARTx == USART2)
            B485_1_S;
        else if (USARTx == USART3)
            B485_2_S;
        else if (USARTx == UART4)
            B485_3_S;
				USART2->DR;
				delay_us(10);
        USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
				
    }
<<<<<<< HEAD
}
//同样使用普通方式 DMA传输是固定长度的 如果传输不同长度的数据的话 需要重新配置DMA 花费时间成本更高
void Sync_Send_Write(USART_TypeDef *USARTx, uint8_t *Data, uint8_t Num)
{
    uint16_t Len, CRC_Data, i;
    if (Num != 0)
=======
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
>>>>>>> 12286d8bf7e1105d32dcdc6a86d7d39d0ad1dd7b
    {
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
        if (USARTx == USART2)
            B485_1_S;
        else if (USARTx == USART3)
            B485_2_S;
        else if (USARTx == UART4)
            B485_3_S;
        for (i = 0; i < 14 + Num * 5; i++)
        {
            USART_SendData(USARTx, (uint8_t)Feedback[i]);
            while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) {};
        }
        delay_us(10);
        if (USARTx == USART2)
            B485_1_R;
        else if (USARTx == USART3)
            B485_2_R;
        else if (USARTx == UART4)
            B485_3_R;
    }
		delay_us(10);
		if(USARTx==USART2)
			B485_1_R;
		else if(USARTx==USART3)
			B485_2_R;
		else if(USARTx==UART4)
			B485_3_R;
}
// Init采用普通方式 无需实时性
void Sync_Send_Init(USART_TypeDef *USARTx, uint8_t StartID, uint8_t Num)
{
    uint16_t Len, CRC_Data, i;
<<<<<<< HEAD
    if (Num != 0)
=======
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
>>>>>>> 12286d8bf7e1105d32dcdc6a86d7d39d0ad1dd7b
    {
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
        for (i = 0; i < Num; i++)
        {
            Feedback[12 + 2 * i] = StartID + i;
            Feedback[13 + 2 * i] = 0x01;
        }
        CRC_Data               = CRC_Acc(0, Feedback, 12 + Num * 2);
        Feedback[12 + Num * 2] = (uint8_t)CRC_Data & 0xFF;
        Feedback[13 + Num * 2] = (uint8_t)(CRC_Data >> 8) & 0xFF;
        if (USARTx == USART2)
            B485_1_S;
        else if (USARTx == USART3)
            B485_2_S;
        else if (USARTx == UART4)
            B485_3_S;
        for (i = 0; i < 14 + Num * 2; i++)
        {
            USART_SendData(USARTx, (uint8_t)Feedback[i]);
            while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) {};
        }
        delay_us(10);
        if (USARTx == USART2)
            B485_1_R;
        else if (USARTx == USART3)
            B485_2_R;
        else if (USARTx == UART4)
            B485_3_R;
    }
		delay_us(10);
		if(USARTx==USART2)
			B485_1_R;
		else if(USARTx==USART3)
			B485_2_R;
		else if(USARTx==UART4)
			B485_3_R;
}

<<<<<<< HEAD
//数据传输采用DMA
// USART1_TX用的是DMA2数据流7 通道4
// USART2_TX用的是DMA1数据流6 通道4
// USART3_TX用的是DMA1数据流3 通道4
// UART4_TX用的是DMA1数据流4 通道4
=======
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


>>>>>>> 12286d8bf7e1105d32dcdc6a86d7d39d0ad1dd7b

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
