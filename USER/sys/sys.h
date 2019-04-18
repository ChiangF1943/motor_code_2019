#ifndef __SYS_H_
#define __SYS_H_

#include "sys_config.h"

void sys_init(void);
void delay_ms(int nms);
void delay_us(int nus);
void delay_s(float timw_s);
void NVIC_Config(void);
uint32_t U8toU32(uint8_t *Data);
uint16_t CRC_Acc(uint16_t crc_accum, uint8_t *data_blk_ptr, uint16_t data_blk_size);

#endif
