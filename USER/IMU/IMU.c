#include ".\IMU\IMU.h"

signed short accxyz[3], gyroxyz[3], magnxyz[3];
float accf[3], gyrof[3], magnf[3];
unsigned char accraw_adis[6];
unsigned char gyroraw_adis[6];
unsigned char magnraw_adis[6];

void SPI2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    SPI2_CS1;

    SPI_I2S_DeInit(SPI2);
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);
}

unsigned char IMU_SeRe(unsigned char data)
{
    while (((SPI2->SR) & 0x02) == 0)
        ;
    SPI2->DR = data;
    while (((SPI2->SR) & 0X01) == 0)
        ;
    return SPI2->DR;
}

signed short comtoform(signed short data)
{
    signed short data1, newdata, flagbit;

    flagbit = (data & 0x2000) >> 13;

    if (flagbit == 0x0000)
        newdata = data;
    else
    {
        data1   = (signed short)((~(((unsigned short)data) - 1)) & 0x3fff);
        newdata = (-1) * data1;
    }
    return newdata;
}

void ADIS_ReadData(void)
{
    const unsigned char SPI_Trans_buf[22] = {0x04, 0x00, 0x06, 0x00, 0x08, 0x00, 0x0A, 0x00, 0x0C, 0x00, 0x0E, 0x00, 0x10, 0x00, 0x12, 0x00, 0x14, 0x00, 0x3c, 0x00, 0x00, 0x00};
    unsigned char SPI_Rec_buf[22];

    int i;

    for (i = 0; i <= 21; i = i + 1)
    {
        SPI2_CS0;
        SPI_Rec_buf[i] = IMU_SeRe(SPI_Trans_buf[i]);
        i              = i + 1;
        SPI_Rec_buf[i] = IMU_SeRe(SPI_Trans_buf[i]);
        SPI2_CS1;
        delay_us(10);
    }

    gyroxyz[0] = (signed short)(((unsigned short)SPI_Rec_buf[2]) << 8) | ((unsigned short)SPI_Rec_buf[3]);
    gyroxyz[1] = (signed short)(((unsigned short)SPI_Rec_buf[4]) << 8) | ((unsigned short)SPI_Rec_buf[5]);
    gyroxyz[2] = (signed short)(((unsigned short)SPI_Rec_buf[6]) << 8) | ((unsigned short)SPI_Rec_buf[7]);

    accxyz[0] = (signed short)(((unsigned short)SPI_Rec_buf[8]) << 8) | ((unsigned short)SPI_Rec_buf[9]);
    accxyz[1] = (signed short)(((unsigned short)SPI_Rec_buf[10]) << 8) | ((unsigned short)SPI_Rec_buf[11]);
    accxyz[2] = (signed short)(((unsigned short)SPI_Rec_buf[12]) << 8) | ((unsigned short)SPI_Rec_buf[13]);

    magnxyz[0] = (signed short)(((unsigned short)SPI_Rec_buf[14]) << 8) | ((unsigned short)SPI_Rec_buf[15]);
    magnxyz[1] = (signed short)(((unsigned short)SPI_Rec_buf[16]) << 8) | ((unsigned short)SPI_Rec_buf[17]);
    magnxyz[2] = (signed short)(((unsigned short)SPI_Rec_buf[18]) << 8) | ((unsigned short)SPI_Rec_buf[19]);

    gyroxyz[0] = gyroxyz[0] & 0x3fff;
    gyroxyz[1] = gyroxyz[1] & 0x3fff;
    gyroxyz[2] = gyroxyz[2] & 0x3fff;

    accxyz[0] = accxyz[0] & 0x3fff;
    accxyz[1] = accxyz[1] & 0x3fff;
    accxyz[2] = accxyz[2] & 0x3fff;

    magnxyz[0] = magnxyz[0] & 0x3fff;
    magnxyz[1] = magnxyz[1] & 0x3fff;
    magnxyz[2] = magnxyz[2] & 0x3fff;

    gyroxyz[0] = comtoform(gyroxyz[0]);
    gyroxyz[1] = comtoform(gyroxyz[1]);
    gyroxyz[2] = comtoform(gyroxyz[2]);

    accxyz[0] = comtoform(accxyz[0]);
    accxyz[1] = comtoform(accxyz[1]);
    accxyz[2] = comtoform(accxyz[2]);

    magnxyz[0] = comtoform(magnxyz[0]);
    magnxyz[1] = comtoform(magnxyz[1]);
    magnxyz[2] = comtoform(magnxyz[2]);

    gyroraw_adis[0] = (gyroxyz[0] & 0xFF00) >> 8;
    gyroraw_adis[1] = gyroxyz[0] & 0x0FF;
    gyroraw_adis[2] = (gyroxyz[1] & 0xFF00) >> 8;
    gyroraw_adis[3] = gyroxyz[1] & 0x0FF;
    gyroraw_adis[4] = (gyroxyz[2] & 0xFF00) >> 8;
    gyroraw_adis[5] = gyroxyz[2] & 0x0FF;

    accraw_adis[0] = (accxyz[0] & 0xFF00) >> 8;
    accraw_adis[1] = accxyz[0] & 0x0FF;
    accraw_adis[2] = (accxyz[1] & 0xFF00) >> 8;
    accraw_adis[3] = accxyz[1] & 0x0FF;
    accraw_adis[4] = (accxyz[2] & 0xFF00) >> 8;
    accraw_adis[5] = accxyz[2] & 0x0FF;

    magnraw_adis[0] = (magnxyz[0] & 0xFF00) >> 8;
    magnraw_adis[1] = (magnxyz[0] & 0xFF00) >> 8;
    magnraw_adis[2] = (magnxyz[1] & 0xFF00) >> 8;
    magnraw_adis[3] = (magnxyz[1] & 0xFF00) >> 8;
    magnraw_adis[4] = (magnxyz[2] & 0xFF00) >> 8;
    magnraw_adis[5] = (magnxyz[2] & 0xFF00) >> 8;
}
