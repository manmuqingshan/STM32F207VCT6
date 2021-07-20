/****************************************Copyright (c)****************************************************
**                                                         
**                                     
**                      ����ʵ��IO��ģ��I2C����ʱ��ĺ궨��
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			  i2c_io.c
** Last modified Date:  2011-05-27
** Last Version:		1.0
** Descriptions:		
**--------------------------------------------------------------------------------------------------------
** Created by:			Zhanghuping
** Created date:		2011-05-27
** Version:				  1.0
** Descriptions:		The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef __I2C_IO_H 
#define __I2C_IO_H

#include "stm32f2xx_gpio.h"



/*! 
 * ����SCL��дIO����  PC14
 */

#define SCL_SET()  GPIO_SetBits(GPIOC,GPIO_Pin_14)   //!< IOд1
#define SCL_CLR()  GPIO_ResetBits(GPIOC,GPIO_Pin_14) //!< IOд0
#define SCL_OUT()  GPIOC->MODER = (GPIOC->MODER&0xCFFFFFFF)|0x10000000 //!IO���

/*! 
 * ����SDA��дIO����  PC15
 */

#define SDA_SET()  GPIO_SetBits(GPIOC,GPIO_Pin_15)   //!IOд1
#define SDA_CLR()  GPIO_ResetBits(GPIOC,GPIO_Pin_15) //!IOд0

#define SDA_OUT()  GPIOC->MODER = (GPIOC->MODER&0x3FFFFFFF)|0x40000000 //!IO���
#define SDA_IN()   GPIOC->MODER = (GPIOC->MODER&0x3FFFFFFF)            //!< IO����
#define SDA_READ() GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)    //!IO����



extern void I2C_IO_init(void);
//extern uint8_t I2C_Read_nbyte(uint8_t addr,uint8_t sub_addr,uint8_t *data_cache,uint32_t num);
//extern uint8_t I2C_Write_nbyte(uint8_t addr,uint8_t sub_addr,uint8_t *data_cache,uint32_t num);


extern void I2C_Start(void);
extern uint8_t I2C_Send_byte(uint8_t data);
extern uint8_t I2C_Wait_Ack(void);
extern void I2C_Stop(void);
extern uint8_t I2C_Receive_byte(uint8_t flg);




#endif