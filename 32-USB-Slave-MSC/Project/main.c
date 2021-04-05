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
#include "stm32f2x_sdio_sd.h"
#include "Ex_Flash.h"
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h" 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

SD_Error Status = SD_OK;

USB_OTG_CORE_HANDLE USB_OTG_dev;
extern uint8_t USB_STATUS_REG;		//USB״̬
extern uint8_t bDeviceState;		//USB���� ���

void NVIC_Configuration(void);
void FLASH_GPIO_Init(void);
void FLASH_SPIInit(void);


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t line = 0;
  uint32_t num = 0;
  RCC_ClocksTypeDef RCC_Clocks;
  GPIO_InitTypeDef  GPIO_Init_s;
  char LCD_string[200];
  
  uint8_t  uid[8];
  uint32_t RDID_data;
  uint32_t i;
  
  uint32_t offline_cnt=0;
  uint32_t tct=0;
  uint32_t USB_STA;
  uint32_t Divece_STA;
  
  uint8_t buf[200];
  char str[200];
  
  /* ����SysTickΪ1ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  
  /* ʹ��GPIOE�˿�ʱ�� */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  
  /* ����LED�ܽ� */
  GPIO_Init_s.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init_s.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init_s.GPIO_OType = GPIO_OType_PP;
  GPIO_Init_s.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init_s.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_Init_s);
  
  GPIO_SetBits(GPIOE,GPIO_Pin_4);//Ϩ��LED��
  
  /* Interrupt Config */
  NVIC_Configuration();
  
  LCD_Init();
  FLASH_SPIInit();
  
  
  Ex_FLASH_ReadUID(uid);  
  sprintf(str,"UID  of NOR Flash : %x%x%x%x%x%x%x%x\r\n",uid[0],uid[1],uid[2],uid[3],uid[4],uid[5],uid[6],uid[7]);
  LCD_ShowString(0,16*(line++),(uint8_t*)str);
  
  RDID_data = Ex_FLASH_ReadID();
  sprintf(str,"RDID of NOR Flash : %x",RDID_data);
  LCD_ShowString(0,16*(line++),(uint8_t*)str);
  
  
  
  if((Status = SD_Init()) != SD_OK)
  {
    LCD_ShowString(0,16*(line++),"SD is ERR");
  }
  else
  {
    LCD_ShowString(0,16*(line++),"SD is OK");
    sprintf(LCD_string,"SD Total Size:%dMbye \r\n",SDCardInfo.CardCapacity/1024/1024);
    LCD_ShowString(0,16*(line++),(uint8_t*)LCD_string);
    sprintf(LCD_string,"SD Block Size:%d byte \r\n",SDCardInfo.CardBlockSize);
    LCD_ShowString(0,16*(line++),(uint8_t*)LCD_string);
  }
  
  USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
  
  USB_OTG_BSP_mDelay(1800);
  USB_OTG_BSP_mDelay(1800);
  USB_OTG_BSP_mDelay(1800);
  USB_OTG_BSP_mDelay(1800);
  
  while (1)
  {
    Delay(100);    
    if(USB_STA!=USB_STATUS_REG)//״̬�ı��� 
    {	 						   		  	   
      if(USB_STATUS_REG&0x01)//����д		  
      {
        LCD_ShowString(0,16*(line),"USB Writing...");
      }
      if(USB_STATUS_REG&0x02)//���ڶ�
      {
        LCD_ShowString(0,16*(line),"USB Reading...");
      }	 										  
      if(USB_STATUS_REG&0x04)
        LCD_ShowString(0,16*(line),"USB Write Err");
      if(USB_STATUS_REG&0x08)
        LCD_ShowString(0,16*(line),"USB Read  Err ");
      USB_STA=USB_STATUS_REG;//��¼����״̬
    }
    if(Divece_STA!=bDeviceState) 
    {
      if(bDeviceState==1)
        LCD_ShowString(0,16*(line),"USB Connected");
      else 
        LCD_ShowString(0,16*(line),"USB DisConnected");
      Divece_STA=bDeviceState;
    }
    tct++;
    if(tct==200)
    {
      tct=0;
      if(USB_STATUS_REG&0x10)
      {
        offline_cnt=0;//USB������,�����offline������
        bDeviceState=1;
      }else//û�еõ���ѯ 
      {
        offline_cnt++;  
        if(offline_cnt>10)bDeviceState=0;//2s��û�յ����߱��,����USB���γ���
      }
      USB_STATUS_REG=0;
    }
  }
}

/**
  * @brief  Configures SDIO IRQ channel.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = SD_SDIO_DMA_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_Init(&NVIC_InitStructure);  
}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t SPI_ReadWriteByte(uint8_t data)
{
  while(!(SPI1->SR & SPI_I2S_FLAG_TXE));
  SPI1->DR = data;
  while(!(SPI1->SR & SPI_I2S_FLAG_RXNE));
  return SPI1->DR;                                               
}

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void FLASH_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /*!< Enable the SPI clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  
  /*!< Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
  /*!< SPI pins configuration *************************************************/
  
  /*!< Connect SPI pins to AF5 */  
  GPIO_PinAFConfig(GPIOA, 5, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, 6, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOB, 5, GPIO_AF_SPI1);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//GPIO_PuPd_DOWN;
  
  /*!< SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /*!< SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /*!< SPI MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /*!< Configure sFLASH Card CS pin in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void  FLASH_SPIInit(void)
{  
  
  SPI_InitTypeDef  SPI_InitStructure;
  
  FLASH_GPIO_Init();
  
  /*!< Deselect the FLASH: Chip Select high */
  GPIO_SetBits(GPIOE,GPIO_Pin_12);
  
  /*!< SPI configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫��˫��ȫ˫��
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//�� SPI
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;// SPI ���ͽ��� 8 λ֡�ṹ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//�ڶ������������ݱ�����
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS �ź����������
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//Ԥ��Ƶ 16
  
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ݴ���� MSB λ��ʼ
  SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC ֵ����Ķ���ʽ
  SPI_Init(SPI1, &SPI_InitStructure);
  /*!< Enable the sFLASH_SPI  */
  SPI_Cmd(SPI1, ENABLE);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}


/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
