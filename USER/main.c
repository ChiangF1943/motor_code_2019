#include "stm32f4xx.h"
#include "sys_config.h"

// 测试用程序 模拟21个舵机
// 需要任务: 接收指令Sync Write不返回
// 接收指令Sync Read 返回21个小包

USART_DataTypeDef U1_DataTypeStructure;
USART_DataTypeDef U2_DataTypeStructure;
USART_DataTypeDef U3_DataTypeStructure;
USART_DataTypeDef U4_DataTypeStructure;
#define ServoIDStart 0
#define ReadPos 0
#define ReadSpeed 1
uint8_t IDList[30] = \
{1, 8, 16, 18, 21, 22,\
	2, 3, 4, 5, 6, 7,\
9, 10, 11, 12, 13,14\
};

uint8_t NewRawData[30][40];

int PeriodWriteFlag = 0; // 保证周期内收到多个上位机的位置值，只执行一次的flag
int FootFlag        = 0; // 是否打开脚底读取的flag，由上位机控制，跟随Instruction=0x84而打开，=0x85而关闭
uint8_t *pDataSendBuffer;
uint16_t TimeCount;
uint8_t tim7_flag = 0;

int main(void)
{
    uint16_t index;
    uint16_t Len;
    uint16_t ACCResult;

    sys_init();
		U2_DataTypeStructure.RevCount = 0;
		U3_DataTypeStructure.RevCount = 0;
		U4_DataTypeStructure.RevCount = 0;
    delay_ms(300);
    Sync_Send_Init(USART2, U2_ServoNum, &IDList[0]);
		delay_ms(1);
    Sync_Send_Init(USART3, U3_ServoNum, &IDList[U2_ServoNum]);
		delay_ms(1);
    Sync_Send_Init(UART4,U4_ServoNum, &IDList[U2_ServoNum+U3_ServoNum]);
		delay_ms(1);
    while (1)
    {
				// slow start
				if(tim7_flag <= 8 ){
						if(tim7_flag == 2 )			{Set_Fixed_Motor_Limit((uint16_t)200, Velocity_Limit);}
						if(tim7_flag == 3 )			Set_Fixed_Motor_Limit((uint16_t)300, Velocity_Limit);
						if(tim7_flag == 4 )			Set_Fixed_Motor_Limit((uint16_t)500, Velocity_Limit);
						if(tim7_flag == 6 )			Set_Fixed_Motor_Limit((uint16_t)1023, Velocity_Limit);
						if(tim7_flag == 8 )			{TIM_Cmd(TIM7, DISABLE); tim7_flag = 10;}
				}
				
        //如果UART1收到上位机的指令，且没有重复包
        if (U1_DataTypeStructure.RevCount == 0x01 && PeriodWriteFlag == 0)
        {
            PeriodWriteFlag = 1;
            Sync_Send_Write(USART2, &U1_DataTypeStructure.Temp_Rev[0], U2_ServoNum);
            Sync_Send_Write(USART3, &U1_DataTypeStructure.Temp_Rev[0 + 3 * U2_ServoNum], U3_ServoNum);
            Sync_Send_Write(UART4, &U1_DataTypeStructure.Temp_Rev[0 + 3 * (U2_ServoNum + U3_ServoNum)], U4_ServoNum);

            delay_ms(1);
            Sync_Send_Read(USART2,  U2_ServoNum, &IDList[0]);
						delay_ms(1);
            Sync_Send_Read(USART3,  U3_ServoNum, &IDList[U2_ServoNum]);
						delay_ms(1);
            Sync_Send_Read(UART4, U4_ServoNum, &IDList[U2_ServoNum + U3_ServoNum]);
            U1_DataTypeStructure.RevCount = 0;
        }
        //如果UART2-UART4收到舵机和脚底的状态包，且包数量正确
        if (U2_DataTypeStructure.RevCount == U2_ServoNum && U3_DataTypeStructure.RevCount == U3_ServoNum + FootFlag && U4_DataTypeStructure.RevCount == U4_ServoNum + FootFlag && PeriodWriteFlag == 1)
        {
						PeriodWriteFlag = 2;
						for (index = 0; index < ALL_ServoNum ; index++)
						{
							DataSendBuffer[index]=SetData(&NewRawData[index][9]);
						}
            //中断修改将数据放到DataSendBuffer中 遵循 8*ServoNum  前4位速度后四位位置
            for (index = 0; index < ALL_ServoNum + 2 * FootFlag; index++)
            {
                Len       = ((NewRawData[index][6] << 8) | NewRawData[index][5]) + 7;            //包长度
                ACCResult = CRC_Acc(0, NewRawData[index], Len - 2);                              // CRC
                if (ACCResult != (NewRawData[index][Len - 1] << 8 | NewRawData[index][Len - 2])) //检验是否相等
                {
                    if (index < ALL_ServoNum) //舵机数据占1个uint32,直接对应赋值
                    {
                        DataSendBuffer[index] = PreDataSendBuffer[index];
                    }
                    else
                    {
                        if (index == ALL_ServoNum) //足底数据占2个uint32_t 对应赋值
                        {
                            DataSendBuffer[ALL_ServoNum]     = PreDataSendBuffer[ALL_ServoNum];
                            DataSendBuffer[ALL_ServoNum + 1] = PreDataSendBuffer[ALL_ServoNum + 1];
                        }
                        if (index == ALL_ServoNum + 1)
                        {
                            DataSendBuffer[ALL_ServoNum + 2] = PreDataSendBuffer[ALL_ServoNum + 2];
                            DataSendBuffer[ALL_ServoNum + 3] = PreDataSendBuffer[ALL_ServoNum + 3];
                        }
                    }
                }
            }

            ADIS_ReadData();
            pDataSendBuffer = (uint8_t *)&DataSendBuffer[ALL_ServoNum + 4 * FootFlag]; //需要修改起始位置 //3+4*1
            memcpy(pDataSendBuffer + 6, gyroraw_adis, 6); //gyro-x, gyro-y, gyro-z
            memcpy(pDataSendBuffer, accraw_adis, 6); 			//acc-x,  acc-y,  acc-z
            memcpy(pDataSendBuffer + 12, magnraw_adis, 6);//magn-x, magn-y, magn-z
            U2_DataTypeStructure.RevCount = 0;
            U3_DataTypeStructure.RevCount = 0;
            U4_DataTypeStructure.RevCount = 0;           
            memcpy(PreDataSendBuffer, DataSendBuffer, 4 * ALL_ServoNum + FootFlag * 16); //拷贝数据到PreBuffer
						TimeCount = TIM_GetCounter(TIM7);
						//pDataSendBuffer=(uint8_t *)DataSendBuffer;
						//pDataSendBuffer[1]=TimeCount&0xFF;
						//pDataSendBuffer[0]=(TimeCount>>8)&0xFF;
            SendOrder(DataSendBuffer, 4 * ALL_ServoNum + FootFlag * 4 * 4 + 18);         //修改ok  // 4byte*3motor + 1flag*16byte + 18
            memset(DataSendBuffer,0,4 * ALL_ServoNum + FootFlag * 4 * 4 + 18);
						PeriodWriteFlag               = 0;
        }
    }
}
//流程
// 1. ROS发送正常的Sync_write的21个舵机位置指令下来
// 2. STM32接收到之后放到暂存区,并用标志位控制在主程序中发送3部分的写位置信息指令
// 3. STM32接着发送读位置指令,开启中断等待全部接收完毕,通过RevCount==当前串口控制舵机数量判断,判断OK则在主程序中将接收到
//   的数据转换到传输缓存区 接着发送读速度指令
// 4. 主程序中循环判断RevCount==当前串口控制舵机数量*2,判断OK则传输完毕,将数据再搬到传输缓存区,这时读取IMU数据,同样放到传输缓存区
// 5. 最后将传输缓存区的数据当作uint8_t数组发出 ROS部分按读取后再转换回Int类型和IMU数据

// TODOList:
// 1. 修改串口引脚到正式测试板上
// 2. 移植IMU读取函数
// 3. 将关键参数改成宏形式方便修改

/*********************************************END OF FILE**********************/
