/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#define  RINGBUFF_LEN          (500)     //�����������ֽ��� 500
#define  RINGBUFF_OK           1     
#define  RINGBUFF_ERR          0   
void RingBuff_Init(void);
uint8_t Write_RingBuff(uint8_t data);
uint8_t Read_RingBuff(uint8_t *rData);

typedef struct
{
    uint16_t Head;           
    uint16_t Tail;
    uint16_t Lenght;
    uint8_t  Ring_data[RINGBUFF_LEN];
}RingBuff_t;
RingBuff_t ringBuff;//����һ��ringBuff�Ļ�����

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

void UART_Init(void);
/*120Mhzʱ��ʱ����ulCountΪ1ʱ��������ʱ3��ʱ�ӣ���ʱ=3*1/120us=1/40us*/
/*
SystemCoreClock=120000000

us����ʱ,��ʱn΢��
SysCtlDelay(n*(SystemCoreClock/3000000));

ms����ʱ,��ʱn����
SysCtlDelay(n*(SystemCoreClock/3000));

m����ʱ,��ʱn��
SysCtlDelay(n*(SystemCoreClock/3));
*/

#if defined   (__CC_ARM) /*!< ARM Compiler */
__asm void
SysCtlDelay(unsigned long ulCount)
{
    subs    r0, #1;
    bne     SysCtlDelay;
    bx      lr;
}
#elif defined ( __ICCARM__ ) /*!< IAR Compiler */
void
SysCtlDelay(unsigned long ulCount)
{
    __asm("    subs    r0, #1\n"
       "    bne.n   SysCtlDelay\n"
       "    bx      lr");
}

#elif defined (__GNUC__) /*!< GNU Compiler */
void __attribute__((naked))
SysCtlDelay(unsigned long ulCount)
{
    __asm("    subs    r0, #1\n"
       "    bne     SysCtlDelay\n"
       "    bx      lr");
}

#elif defined  (__TASKING__) /*!< TASKING Compiler */                           
/*��*/
#endif /* __CC_ARM */
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  GPIO_InitTypeDef  GPIO_Init_s;
  uint8_t data;
  
  /* ʹ��GPIOE�˿�ʱ�� */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  
  /* ����LED�ܽ� */
  GPIO_Init_s.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init_s.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init_s.GPIO_OType = GPIO_OType_PP;
  GPIO_Init_s.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init_s.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_Init_s);
  
  GPIO_ResetBits(GPIOE,GPIO_Pin_4);//����LED��

  RingBuff_Init();
  UART_Init();


  while (1)
  {
    if(Read_RingBuff(&data))            //�ӻ��λ������ж�ȡ����
    {
      USART_SendData(USART1, data);
    }
    SysCtlDelay(1*(SystemCoreClock/3000));
  }
}

void UART_Init(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  /* Enable UART1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(GPIOA, 9, GPIO_AF_USART1);
  
  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(GPIOA, 10, GPIO_AF_USART1);
  
  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* USART configuration */
  USART_Init(USART1, &USART_InitStructure);
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}

/**
* @brief  RingBuff_Init
* @param  void
* @return void
* @note   ��ʼ�����λ�����
*/
void RingBuff_Init(void)
{
  //��ʼ�������Ϣ
  ringBuff.Head = 0;
  ringBuff.Tail = 0;
  ringBuff.Lenght = 0;
}


/**
* @brief  Write_RingBuff
* @param  uint8_t data
* @return FLASE:���λ�����������д��ʧ��;TRUE:д��ɹ�
* @note   �����λ�����д��uint8_t���͵�����
*/
uint8_t Write_RingBuff(uint8_t data)
{
  if(ringBuff.Lenght >= RINGBUFF_LEN) //�жϻ������Ƿ�����
  {
    return RINGBUFF_ERR;
  }
  ringBuff.Ring_data[ringBuff.Tail]=data;
  ringBuff.Tail = (ringBuff.Tail+1)%RINGBUFF_LEN;//��ֹԽ��Ƿ�����
  ringBuff.Lenght++;
  return RINGBUFF_OK;
}


/**
* @brief  Read_RingBuff
* @param  uint8_t *rData�����ڱ����ȡ������
* @return FLASE:���λ�����û�����ݣ���ȡʧ��;TRUE:��ȡ�ɹ�
* @note   �ӻ��λ�������ȡһ��u8���͵�����
*/
uint8_t Read_RingBuff(uint8_t *rData)
{
  if(ringBuff.Lenght == 0)//�жϷǿ�
  {
    return RINGBUFF_ERR;
  }
  *rData = ringBuff.Ring_data[ringBuff.Head];//�Ƚ��ȳ�FIFO���ӻ�����ͷ��
  ringBuff.Head = (ringBuff.Head+1)%RINGBUFF_LEN;//��ֹԽ��Ƿ�����
  ringBuff.Lenght--;
  return RINGBUFF_OK;
}

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
