#include  "i2c_io.h"
#include "stm32f2xx.h"

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
#define I2C_DELAY()  SysCtlDelay_IIC(30)



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
  GPIO_Init_s.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init_s.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_Init_s);	
  
  
  GPIO_Init_s.GPIO_Pin = GPIO_Pin_14 ;
  GPIO_Init_s.GPIO_OType = GPIO_OType_OD;
  GPIO_Init(GPIOC, &GPIO_Init_s);
  
  
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
  //IO���
  SDA_OUT(); 
  SCL_OUT();
  //IO��0
  SDA_CLR();  
  SCL_CLR(); 
  I2C_DELAY();
  SCL_SET();
  //��ʱ
  I2C_DELAY();  
  I2C_DELAY();
  I2C_DELAY();
  //SDA��1
  SDA_SET();
  I2C_DELAY();
  I2C_DELAY();
}
/*******************************************************************************
** Function name:       I2C_Send_byte
** Descriptions:        ������I2C���߷���һ���ֽ�
** Input parameters:    data
** output parameters:   0-false 1_ture
** Created Date:        
*******************************************************************************/
uint8_t I2C_Send_byte(uint8_t data)
{
  uint8_t k;
  //����8bit����
  for(k=0;k<8;k++){
    
    I2C_DELAY();
    if(data&0x80){
      SDA_SET();
    }
    else{
      SDA_CLR();
    }
    data=data<<1;
    I2C_DELAY();
    SCL_SET();
    I2C_DELAY();
    I2C_DELAY();
    SCL_CLR();
  }
  //��ʱ��ȡACK��Ӧ
  I2C_DELAY();
  SDA_SET();
  //��Ϊ������
  SDA_IN();
  I2C_DELAY();
  SCL_SET();   
  I2C_DELAY(); //������������⣬��ʱ������޴�
  //������
  k=SDA_READ();
  if(k){ ////NACK��Ӧ
    return 0;
  }
  I2C_DELAY();
  SCL_CLR();
  I2C_DELAY();
  SDA_OUT();
  if(k){ ////NACK��Ӧ
    return 0;
  }
  return 1;
  
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
  uint8_t k,data;
  //����8bit����
  //��Ϊ������
  
  SDA_IN();
  data=0;
  for(k=0;k<8;k++){
    I2C_DELAY();
    SCL_SET();
    I2C_DELAY();
    data=data<<1;
    data=data|SDA_READ();//������
    I2C_DELAY();
    SCL_CLR();
    I2C_DELAY(); 
  }
  //����ACK��Ӧ
  //��Ϊ�����
  SDA_OUT();
  if(flg){
    SDA_SET(); //���1-NACK
  }else{
    SDA_CLR();//���0-ACK
  }
  I2C_DELAY();
  SCL_SET();
  I2C_DELAY();
  I2C_DELAY();
  SCL_CLR();
  I2C_DELAY();
  SDA_OUT();
  //���ض�ȡ������
  return (uint8_t)data;
}
/*******************************************************************************
** Function name:       I2C_Write_nbyte
** Descriptions:        ��I2C����������ָ���ĵ�ַд��������
** Input parameters:    1=addr,2_sub_addr,3-p,4-num
** output parameters:   0-false,1-ture
** Created Date:        
*******************************************************************************/
uint8_t I2C_Write_nbyte(uint8_t addr,uint8_t sub_addr,uint8_t *data_cache,uint32_t num)
{
  uint32_t temp_state,k;
  //����I2C����
  //I2C_Start();
  I2C_Start();
  //����������ַ
  temp_state=I2C_Send_byte(addr&0xFE);//дģʽ
  if(!temp_state){
    I2C_Stop();
    return 0;
  }
  //���������ӵ�ַ
  temp_state=I2C_Send_byte(sub_addr);
  if(!temp_state){
    I2C_Stop();
    return 0;
  }
  //���ͺ��������
  for(k=0;k<num;k++){
    
    temp_state=I2C_Send_byte(data_cache[k]);//д
    if(!temp_state){
      I2C_Stop();
      return 0;
    }   
  }
  
  I2C_Stop();
  return 1;
  
}
/*******************************************************************************
** Function name:       I2C_Read_nbyte
** Descriptions:        ��I2C����������ָ���ĵ�ַ��ȡ�������
** Input parameters:    1=addr,2_sub_addr,3-p,4-num
** output parameters:   0-false,1-ture
** Created Date:        
*******************************************************************************/
uint8_t I2C_Read_nbyte(uint8_t addr,uint8_t sub_addr,uint8_t *data_cache,uint32_t num)
{
  uint32_t temp_state,k;
  //����I2C����
  I2C_Start();
  //����������ַ
  temp_state=I2C_Send_byte(addr&0xFE);//дģʽ
  if(!temp_state){
    I2C_Stop();
    return 0;
  }
  //���������ӵ�ַ
  temp_state=I2C_Send_byte(sub_addr);
  if(!temp_state){
    I2C_Stop();
    return 0;
  }
  //����������ַ
  I2C_Start();
  temp_state=I2C_Send_byte(addr|0x01);//��ģʽ
  if(!temp_state){
    I2C_Stop();
    return 0;
  }
  //��ȡ���������
  for(k=0;k<num;k++){
    if(k == (num-1)){
      data_cache[k]=I2C_Receive_byte(1);//��ȡ���һ���ֽڣ�����NACK
    }else{
      data_cache[k]=I2C_Receive_byte(0);//��ȡ����  
    }
  }
  
  I2C_Stop();
 
  
  
  return 1; 
  
}
