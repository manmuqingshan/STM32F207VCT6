#include  "i2c_io.h"
#include "stm32f2xx.h"
#include "sys_delay.h"

/*
 * @brief  SysCtlDelay
 * @param  ulCount ��ʱֵ���������0
 * @retval None
 */
void SysCtlDelay_IIC(unsigned long ulCount)
{
  SysCtlDelay(ulCount);
}


/////����ʱ��Ƶ��,300KHz
#define I2C_DELAY()  SysCtlDelay_IIC(100)


void MPU_IIC_Delay()
{
  SysCtlDelay(2*(SystemCoreClock/3000000));
}
/*******************************************************************************
** Function name:       I2C_IO_init
** Descriptions:        I2C����IO��ʼ��
** Input parameters:    None
** output parameters:   None
** Created Date:        
*******************************************************************************/
void I2C_IO_init(void)
{
  
  GPIO_InitTypeDef  GPIO_Init_s;	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
  GPIO_Init_s.GPIO_Pin =  GPIO_Pin_15 ;
  GPIO_Init_s.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_Init_s.GPIO_OType = GPIO_OType_OD;
  GPIO_Init_s.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init_s.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_Init_s);	
  
  GPIO_Init_s.GPIO_Pin = GPIO_Pin_14 ;
  GPIO_Init_s.GPIO_OType = GPIO_OType_OD;
  GPIO_Init(GPIOC, &GPIO_Init_s);
  
  SDA_SET();  
  SCL_SET(); 
}
      
/*******************************************************************************
** Function name:       I2C_Start
** Descriptions:        ��ʼ����һ��I2C��ʼ�ź�
** Input parameters:    None
** output parameters:   None
** Created Date:        
*******************************************************************************/
void I2C_Start(void)
{
    //IO���
    SDA_OUT(); 
    SCL_OUT(); 
    I2C_DELAY();
    //IO�ø�
    SDA_SET();  
    SCL_SET(); 
    //��ʱ
    I2C_DELAY();  
    //Ϊ��
    SDA_CLR();
    I2C_DELAY();
    I2C_DELAY();
    SCL_CLR();
}
/*******************************************************************************
** Function name:       I2C_Stop
** Descriptions:        ����һ��I2C���߽����ź�
** Input parameters:    None
** output parameters:   None
** Created Date:        
*******************************************************************************/
void I2C_Stop(void)
{
  SDA_OUT(); //sda�����
  SCL_CLR(); 
  SDA_CLR();//STOP:when CLK is high DATA change form low to high
  MPU_IIC_Delay();
  SCL_SET(); 
  SDA_SET();//����I2C���߽����ź�
  MPU_IIC_Delay();	  
}
/*******************************************************************************
** Function name:       I2C_Wait_Ack
** Descriptions:        �ȴ�Ӧ���źŵ���
** Input parameters:    None
** output parameters:   1������Ӧ��ʧ��,0������Ӧ��ɹ�
** Created Date:        
*******************************************************************************/
uint8_t I2C_Wait_Ack(void)
{
  uint8_t ucErrTime=0;
  SDA_IN();     //SDA����Ϊ����  
  SDA_SET();
  MPU_IIC_Delay();	   
  SCL_SET();  
  MPU_IIC_Delay();	 
  while(SDA_READ())
  {
    ucErrTime++;
    if(ucErrTime>250)
    {
      //      MPU_IIC_Stop();
      return 1;
    }
  }
  SCL_CLR();//ʱ�����0 	   
  return 0;    

} 
/*******************************************************************************
** Function name:       I2C_Send_byte
** Descriptions:        ������I2C���߷���һ���ֽ�
** Input parameters:    data
** output parameters:   0-false 1_ture
** Created Date:        
*******************************************************************************/
uint8_t I2C_Send_byte(uint8_t txd)
{
  uint8_t t;   
  SDA_OUT(); 	    
  SCL_CLR();//����ʱ�ӿ�ʼ���ݴ���
  for(t=0;t<8;t++)
  {              
    if(txd&0x80){
      SDA_SET();
    }
    else{
      SDA_CLR();
    }
    txd<<=1; 	  
    SCL_SET();
    MPU_IIC_Delay(); 
//    I2C_DELAY();
    SCL_CLR();	
    MPU_IIC_Delay();
//    I2C_DELAY();
  }
  
  return 1;
}
//����ACKӦ��
void MPU_IIC_Ack(void)
{
  SCL_CLR();
  SDA_OUT();
  SDA_CLR();
  MPU_IIC_Delay();
  SCL_SET();
  MPU_IIC_Delay();
  SCL_CLR();
}
//������ACKӦ��		    
void MPU_IIC_NAck(void)
{
  SCL_CLR();
  SDA_OUT();
  SDA_SET();
  MPU_IIC_Delay();
  SCL_SET();
  MPU_IIC_Delay();
  SCL_CLR();
}	

/*******************************************************************************
** Function name:       I2C_Receive_byte
** Descriptions:        ������I2C���߽���һ���ֽ�
** Input parameters:    None
** output parameters:   data
** Created Date:        
*******************************************************************************/
uint8_t I2C_Receive_byte(uint8_t flg)
{
  //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
  
  unsigned char i,receive=0;
  SDA_IN();//SDA����Ϊ����
  for(i=0;i<8;i++ )
  {
    SCL_CLR();
    MPU_IIC_Delay();
    SCL_SET();
    receive<<=1;
    if(SDA_READ())
      receive++;   
    MPU_IIC_Delay(); 
  }					 
  if (!flg)
    MPU_IIC_NAck();//����nACK
  else
    MPU_IIC_Ack(); //����ACK   
  return receive;


}


