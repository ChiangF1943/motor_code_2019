#include "sys_config.h"

void Peripherals_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    // A9 U1_TX A10 U1_RX
    // B10 U3_TX B11 U3_RX
    // C10 U4_TX C11 U4_RX
    // C12 U5_TX D2 U5_RX
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 | RCC_APB1Periph_UART4 | RCC_APB1Periph_UART5, ENABLE);

    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);

    USART_InitStructure.USART_BaudRate            = 1000000;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Init(USART3, &USART_InitStructure);
    USART_Init(UART4, &USART_InitStructure);
    USART_Init(UART5, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);

    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART4, ENABLE);

    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART5, ENABLE);
}
