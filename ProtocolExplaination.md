## 指令解析

### 发送部分

#### 格式

前五位固定为 FF FF FD 00 FE

接长度位 Len_L 和 Len_H

后接 83 74 00 02 00 

之后每个舵机占3位，第一位为ID 第二位为位置数据低位 第三位为位置数据高位

之后是CRC的两位 低位在前

#### 代码（Python）

```Python
def SendData(self):
        self.Order = [0 for x in range(5)]
        self.Order[0] = 0xFF
        self.Order[1] = 0xFF
        self.Order[2] = 0xFD
        self.Order[3] = 0x00
        self.Order[4] = 0xFE
        Len = 7 + self.ServoNum * 3
        self.Order.append(Len & 0xFF)  # 修改
        self.Order.append((Len >> 8) & 0xFF)
        self.Order.append(0x83)
        self.Order.append(0x74)
        self.Order.append(0x00)
        self.Order.append(0x02)
        self.Order.append(0x00)
        for i in range(self.ServoNum):
            self.Order.append(self.IDList[i])
            self.Order.append(self.PosData[i] & 0xFF)
            self.Order.append((self.PosData[i] >> 8) & 0xFF)
        CRC = self.CRC_Acc(0, self.Order, 12 + 3 * self.ServoNum)
        self.Order.append(CRC & 0xFF)
        self.Order.append((CRC) >> 8 & 0xFF)
        self.ser.write(bytes(self.Order))
# 设ID=2的位置设置为2048 ID=3的位置是1024
# FF FF FD 00 FE 0D 00 83 74 00 02 00 02 00 08 03 00 40 CRC_L CRC_H
```

### 接收部分

#### 格式

之所以不同第一是降低CRC的计算耗时 第二是有现成的解析代码

| 指令头 | 指令头 | 指令长度（1位） | 指令内容 | 指令校验          | 指令尾 | 指令尾 |
| ------ | ------ | --------------- | -------- | ----------------- | ------ | ------ |
| 0x12   | 0x34   | 指令总长度      | 指令内容 | 指令长度+指令内容 | 0x1F   | 0x2F   |

指令总长小于255 

指令校验是将长度和内容累加取低八位

#### 解析代码

Python示例

```Python
def InfoAnalysis(self, data):  # 修改
        res_data = bytearray(data)
        while len(res_data) != 0:
            temp = res_data.pop(0)
            if self.pointer < 2:
                self.InfoLoad(temp)
                continue
            if self.pointer == 2:
                if (self.OrderBuffer[0] == 0x12) and (self.OrderBuffer[1] == 0x34):
                    self.InfoLoad(temp)
                    self.LEN = temp
                else:
                    self.OrderBuffer[0] = self.OrderBuffer[1]
                    self.OrderBuffer[1] = temp
                continue
            if self.pointer > 2:
                if self.pointer < self.LEN:
                    self.InfoLoad(temp)
                    if self.pointer == self.LEN:
                        if (self.OrderBuffer[self.LEN - 1] == 0x2F) and (self.OrderBuffer[self.LEN - 2] == 0x1F):
                            for i in range(2, self.LEN - 3):
                                self.Checksum = self.Checksum + self.OrderBuffer[i]
                            if self.OrderBuffer[-3] == self.Checksum & 0xFF:
                                self.RevCount += 1
                                self.Data_DisPlay(self.OrderBuffer)
                            else:
                                self.DisPlayMessage("LOG：校验错误,校验值应当为: %X" % (self.Checksum & 0xFF))
                        else:
                            self.DisPlayMessage(
                                "LOG：指令尾错误 %X %X" % (self.OrderBuffer[self.LEN - 2], self.OrderBuffer[self.LEN - 1]))
                        self.Checksum = 0
                        self.pointer = 0
                        self.OrderBuffer = []
                    else:
                        continue
def Data_DisPlay(self, Data):
        Res = []
        str1 = "位置："
        str2 = "速度："
        str3 = "陀螺仪数据："
        str4 = "足底板数据："
        for i in range(self.ServoNum):
            Res.append(Data[3 + 4 * i] << 8 | Data[4 + 4 * i])
            str1 = str1 + " {:10d}".format(Res[-1])
        for i in range(self.ServoNum):
            Res.append(Data[5 + 4 * i] << 8 | Data[6 + 4 * i])
            str2 = str2 + " {:10d}".format(Res[-1])
        for i in range(9):
            Temp = (Data[3 + 4 * self.ServoNum + 2 * i] << 8) | Data[4 + 4 * self.ServoNum + 2 * i]
            if Temp >= 32768:
                Temp = Temp - 65535
            Res.append(Temp)
            str3 = str3 + " {:10d}".format(Res[-1])
        if (len(Data) >= 38 + 4 * self.ServoNum):
            for i in range(8):
                Res.append(Data[22 + 4 * self.ServoNum + 2 * i] << 8 | Data[21 + 4 * self.ServoNum + 2 * i])
                str4 = str4 + " {:10d}".format(Res[-1])
```

C/C++读取示例

```C++
//这个代码需要挨个字符放进去
void AnalysisData(uint8_t temp)
{
    static uint8_t pointer = 0;   // 接收位置
	static uint8_t rx_buff[255]; // 指令接收暂存区
	static uint8_t LEN;           // 指令长度指示位
	static uint8_t count;         // 共用计数值
	static uint8_t sum;           // 校验和
    if (pointer < 2)
    {
        rx_buff[pointer++] = temp;
        return;
    }
    if (pointer == 2)
    {
        if ((rx_buff[0] == 0x12) && (rx_buff[1] == 0x34))
        {
            rx_buff[pointer++] = temp;
            LEN                = temp;
        }
        else
        {
            rx_buff[0] = rx_buff[1];
            rx_buff[1] = temp;
        }
        return;
    }
    if (pointer > 2)
    {
        if (pointer < LEN)
        {
            rx_buff[pointer++] = temp;
            if (pointer == LEN)
            {
                if (rx_buff[LEN - 1] == 0x2F && rx_buff[LEN - 2] == 0x1F)
                {
                    for (count = 2; count < LEN - 3; count++) { sum = rx_buff[count] + sum; }
                    if (rx_buff[LEN - 3] == sum)
                    {
                        DataAnlaysis(rx_buff);
                    }
                }
                sum     = 0;
                pointer = 0;
                memset(rx_buff, 0, 256 * sizeof(uint8_t));
            }
            else
            {
                return;
            }
        }
    }
}
// 数据是高位在前的  i为0到ServoNum之间的数值 [0,ServoNum)
// Data[3 + 4 * i] << 8 | Data[4 + 4 * i] 为位置数据
// Data[5 + 4 * i] << 8 | Data[6 + 4 * i] 为速度数据
// i为0-9的数值 [0,9) 共9个 为陀螺仪数据
// Data[3 + 4 * ServoNum + 2 * i] << 8) | Data[4 + 4 * ServoNum + 2 * i]
// i为0-8之间的数值 分别为8个压力传感器的原始数值 （需要下位机开启脚底板读取 可以用数据长度判断）
// Data[22 + 4 * ServoNum + 2 * i] << 8 | Data[21 + 4 * ServoNum + 2 * i]
```



