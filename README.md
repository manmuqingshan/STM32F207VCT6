# STM32F207VCT6
> 此仓库所有的硬件和软件资料，只用于学习参考使用，在普通室温环境下使用没问题，对于参考此仓库中资料造成商业产品或工业产品故障，本人不负责

[GitHub仓库](https://github.com/strongercjd/STM32F207VCT6)

[码云仓库](https://gitee.com/strongercjd/STM32F207VCT6)



代码对应的[STM32F207系列文章](https://mp.weixin.qq.com/mp/appmsgalbum?__biz=MzIxNTg1NzQwMQ==&action=getalbum&album_id=1359585244344696836&scene=173&from_msgid=2247485288&from_itemidx=1&count=3#wechat_redirect)



本仓库代码多个工程同时使用01-Libraries中的库文件，如果每个工程都有个库文件，会导致整个git仓库文件很大，拉取缓慢。

| name                 | 描述                                   |
| :------------------- | -------------------------------------- |
| 00-Hardware          | 学习STM32F207使用硬件的PCB和原理图     |
| 01-Libraries         | STM32F207使用的库文件                  |
| 02-Template          | Keil和IAR的模板工程                    |
| 03-ASM               | 使用汇编函数编写接口                   |
| 04-Delay             | 使用查询定时器计数器的方式实现延时函数 |
| 05-LCD               | 使用FSMC驱动LCD屏幕代码                |
| 06-GPIO-Input-Output | GPIO输入输出代码                       |
| 07-GPIO-JTAG         | GPIO复用JTAG代码                       |
| 08-GPIO-bit-band     | GPIO位带代码                           |
| 09-EXTI              | 中断控制器代码，使用GPIO中断             |
| 10-CRC&RNG           | STM32硬件CRC单元和随机数单元            |
| 11-SPI               | 使用STM32的SPI控制器读取norflash       |
| 12-SPI-Simulation    | 使用模拟SPI读取norflash               |
| 13-IIC               | 使用模拟IIC读取PCF8536             |
| 14-Flash-Program     | STM32内部Flash编程            |
| 15-Flash-Write-Protection| STM32内部Flash写保护       |
| 16-Flash-Read-Protection | STM32内部Flash读保护（**没有完成**）      |
| 17-DMA-M-To-M | DMA内存到内存传输 |
| 18-IWDG       | 独立看门狗 |
| 19-WWDG       | 窗口看门狗 |
| 20-Timer-Base | 定时器的基础定制中断功能 |
| 21-Timer-PWMOUT | 定时器的PWM输出功能 |
| 22-Timer-InputCapture | 定时器输入捕获功能 |
| 23-Timer-Touchpad | 触摸按键功能 |
| 24-ADC | ADC外设代码 |
| 25-DAC | DAC外设代码                            |
| 26-SDIO | SDIO驱动SD卡代码 |
| 27-USART-Interrupt | 使用串口中断方式收发数据 |
| 28-USART-Printf | 使用硬件串口打印 |
| 29-IDE-Printf | 硬件仿真时使用IDE打印 |
| 30-ETH | 以太网应用代码 |
| 31-USB-Host-MSC | USB Host MSC代码，读取U盘 |
| 32-USB-Slave-MSC | USB Slave MSC代码，模拟U盘 |
| 33-USART-DMA-Receive | DMA串口接收（没有使用中断） |
| 34-USART-Receive-DMAInterrupt | DMA串口接收（DMA中断） |
| 35-USART-DMA-Receive-Interrupt | DMA串口接收（串口空闲中断） |
| 36-USART-DMA-Receive-Send | 串口使用DMA接收，并将收到的数据使用DMA发送出去 |
| 37-USART-Ringbuff | 串口接收，使用环形缓冲区 |
| 38-MPU6050 | IIC读取MPU6050 |

