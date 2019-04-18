#include "sys_config.h"

void Peripherals_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_DMA2, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | RCC_APB1Periph_UART4, ENABLE);

    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);

    USART_InitStructure.USART_BaudRate            = BaudRate;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Init(USART2, &USART_InitStructure);
    USART_Init(USART3, &USART_InitStructure);
    USART_Init(UART4, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);

    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART4, ENABLE);

    DMA_DeInit(DMA2_Stream7);
    while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE) {}
    DMA_DeInit(DMA1_Stream6);
    while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE) {}
    DMA_DeInit(DMA1_Stream3);
    while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE) {}
    DMA_DeInit(DMA1_Stream4);
    while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE) {}

    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    // U1
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (USART1_BASE + 0x04);
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)Feedback;
    DMA_InitStructure.DMA_BufferSize         = 4 * 2 * ALL_ServoNum + 24;
    DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
    DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
    DMA_Init(DMA2_Stream7, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream7, ENABLE);
    while (DMA_GetCmdStatus(DMA2_Stream7) != ENABLE) {}
    // U2
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (USART2_BASE + 0x04);
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&U2_Tx_Buff[0];
    DMA_InitStructure.DMA_BufferSize         = 14+U3_ServoNum;
    DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
    DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);
    DMA_Init(DMA1_Stream6, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream6, ENABLE);
    while (DMA_GetCmdStatus(DMA1_Stream6) != ENABLE) {}
    // U3
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (USART3_BASE + 0x04);
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)U3_Tx_Buff;
    DMA_InitStructure.DMA_BufferSize         = 14 + U3_ServoNum;
    DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
    DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);
    DMA_Init(DMA1_Stream3, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream3, ENABLE);
    while (DMA_GetCmdStatus(DMA1_Stream3) != ENABLE) {}
    // U4
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (UART4_BASE + 0x04);
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)U4_Tx_Buff;
    DMA_InitStructure.DMA_BufferSize         = 14 + U4_ServoNum;
    DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);
    DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);
    DMA_Init(DMA1_Stream4, &DMA_InitStructure);
    DMA_Cmd(DMA1_Stream4, ENABLE);
    while (DMA_GetCmdStatus(DMA1_Stream4) != ENABLE) {}
}

void Led1_Toggle(void)
{
    static char led1_state = 0x00;
    if (led1_state)
    {
        led1_state = 0;
        LED1_OFF;
    }
    else
    {
        led1_state = 1;
        LED1_ON;
    }
}

void Led2_Toggle(void)
{
    static char led2_state = 0x00;
    if (led2_state)
    {
        led2_state = 0;
        LED2_OFF;
    }
    else
    {
        led2_state = 1;
        LED2_ON;
    }
}
