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

int PeriodWriteFlag = 0;  // 保证周期内收到多个上位机的位置值，只执行一次的flag
int FootFlag = 0;         // 是否打开脚底读取的flag，由上位机控制，跟随Instruction=0x84而打开，=0x85而关闭
uint8_t *SendPointer;

int main(void)
{
    sys_init();
    delay_ms(50);
    Sync_Send_Init(USART2, ServoIDStart, U2_ServoNum);
    Sync_Send_Init(USART3, ServoIDStart + U2_ServoNum, U3_ServoNum);
    Sync_Send_Init(UART4, ServoIDStart + U2_ServoNum + U3_ServoNum, U4_ServoNum);
    while (1)
    {
			//如果UART1收到上位机的指令，且没有重复包
			if (U1_DataTypeStructure.RevCount == 0x01 && PeriodWriteFlag == 0) 
        {
            PeriodWriteFlag = 1;
            Sync_Send_Write(USART2, &U1_DataTypeStructure.Temp_Rev[0], U2_ServoNum);
            Sync_Send_Write(USART3, &U1_DataTypeStructure.Temp_Rev[0 + 3 * U2_ServoNum], U3_ServoNum);
            Sync_Send_Write(UART4, &U1_DataTypeStructure.Temp_Rev[0 + 3 * (U2_ServoNum + U3_ServoNum)], U4_ServoNum);
            
            delay_us(20);
            Sync_Send_Read(USART2, ServoIDStart, U2_ServoNum);
            Sync_Send_Read(USART3, ServoIDStart + U2_ServoNum, U3_ServoNum);
            Sync_Send_Read(UART4, ServoIDStart + U2_ServoNum + U3_ServoNum, U4_ServoNum);
            U1_DataTypeStructure.RevCount = 0;
        }
				//如果UART2-UART4收到舵机和脚底的状态包，且包数量正确
        if (U2_DataTypeStructure.RevCount == U2_ServoNum+FootFlag && U3_DataTypeStructure.RevCount == U3_ServoNum && U4_DataTypeStructure.RevCount == U4_ServoNum+FootFlag && PeriodWriteFlag == 1)
        {
            //中断修改将数据放到DataSendBuffer中 遵循 8*ServoNum  前4位速度后四位位置
            ADIS_ReadData();
            SendPointer = (uint8_t *)&DataSendBuffer[2 * ALL_ServoNum+4*FootFlag];//需要修改起始位置
            memcpy(SendPointer, accraw_adis, 6);
            memcpy(SendPointer + 6, gyroraw_adis, 6);
            memcpy(SendPointer + 12, magnraw_adis, 6);

            U2_DataTypeStructure.RevCount = 0;
            U3_DataTypeStructure.RevCount = 0;
            U4_DataTypeStructure.RevCount = 0;
            PeriodWriteFlag               = 0;
            SendOrder(DataSendBuffer,4*ALL_ServoNum+ FootFlag*16 + 18);//修改ok
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
