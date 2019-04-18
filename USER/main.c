#include "stm32f4xx.h"
#include "sys_config.h"

// 测试用程序 模拟21个舵机
// 需要任务: 接收指令Sync Write不返回
// 接收指令Sync Read 返回21个小包

USART_DataTypeDef U1_DataTypeStructure;
USART_DataTypeDef U3_DataTypeStructure;
USART_DataTypeDef U4_DataTypeStructure;
USART_DataTypeDef U5_DataTypeStructure;

#define Servo_Num 21

int data=0;
int main(void)
{
    sys_init();

    while (1)
    {
        if (U1_DataTypeStructure.RevCount == 1)
        {
            Sync_Send_Write(USART3, &U1_DataTypeStructure.Temp_Rev[0], 7);
            Sync_Send_Write(UART4, &U1_DataTypeStructure.Temp_Rev[0 + 5 * 7], 7);
            Sync_Send_Write(UART5, &U1_DataTypeStructure.Temp_Rev[0 + 5 * 14], 7);

            Sync_Send_Read(USART3, 0, 7, 0);
            Sync_Send_Read(UART4, 7, 7, 0);
            Sync_Send_Read(UART5, 14, 7, 0);
            U1_DataTypeStructure.RevCount = 0;
        }
        if (U3_DataTypeStructure.RevCount == 7 && U4_DataTypeStructure.RevCount == 7 && U5_DataTypeStructure.RevCount == 7)
        {
            //将数据转到另一个区域
            memcpy(DataSendBuffer, DataArray, Servo_Num);
            Sync_Send_Read(USART3, 0, 7, 1);
            Sync_Send_Read(UART4, 7, 7, 1);
            Sync_Send_Read(UART5, 14, 7, 1);
        }
        if (U3_DataTypeStructure.RevCount == 14 && U4_DataTypeStructure.RevCount == 14 && U5_DataTypeStructure.RevCount == 14)
        {
            memcpy(&DataSendBuffer[Servo_Num], DataArray, Servo_Num);
            // ReadIMUData(&DataSendBuffer[2*Servo_Num]);
            U3_DataTypeStructure.RevCount = 0;
            U4_DataTypeStructure.RevCount = 0;
            U5_DataTypeStructure.RevCount = 0;
            // SendDataAll();
        }
    }
}

//流程 
//1. ROS发送正常的Sync_write的21个舵机位置指令下来
//2. STM32接收到之后放到暂存区,并用标志位控制在主程序中发送3部分的写位置信息指令
//3. STM32接着发送读位置指令,开启中断等待全部接收完毕,通过RevCount==当前串口控制舵机数量判断,判断OK则在主程序中将接收到
//   的数据转换到传输缓存区 接着发送读速度指令
//4. 主程序中循环判断RevCount==当前串口控制舵机数量*2,判断OK则传输完毕,将数据再搬到传输缓存区,这时读取IMU数据,同样放到传输缓存区
//5. 最后将传输缓存区的数据当作uint8_t数组发出 ROS部分按读取后再转换回Int类型和IMU数据

// TODOList:
// 1. 修改串口引脚到正式测试板上
// 2. 移植IMU读取函数
// 3. 将关键参数改成宏形式方便修改 

/*********************************************END OF FILE**********************/
