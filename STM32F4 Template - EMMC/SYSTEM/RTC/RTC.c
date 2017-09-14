#include "stm32f4xx.h"
#include "RTC.h"

#define Write_Pcf8563 0xa2       //写入地址
#define Read_Pcf8563  0xa3       //读出地址

#define  Addr_second  0x02
#define  Addr_mint    0x03
#define  Addr_hour    0x04
#define  Addr_day     0x05
#define  Addr_month   0x07
#define  Addr_year    0x08






void I2C1_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  //I2C1外设也使能Clock
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);
	
	//配置I2C
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = Write_Pcf8563; // 这个参数是指当STM32作为从设备时的地址，主设备访问通过该地址就可以访问STM32
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;             
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;  
  
	I2C_Init(I2C1, &I2C_InitStructure);
	//使能I2C模块
  I2C_Cmd(I2C1,ENABLE);
	

}



void I2C1_WriteByte(u8 write_address,u8 byte)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1, ENABLE);

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));//等待ACK
	
	I2C_Send7bitAddress(I2C1,Write_Pcf8563,I2C_Direction_Transmitter);//发送设备地址

	while(!I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//等待ACK
	
	I2C_SendData(I2C1, write_address);//发送寄存器地址
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //等待ACK
	
	I2C_SendData(I2C1, byte);//发送数据
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //等待ACK发送完成
	
	I2C_GenerateSTOP(I2C1, ENABLE);//发送完成


}

unsigned char I2C1_ReadByte(u8 read_address) 
{
	I2C1_Configuration();
	
	//I2C_WaitStandbyState();
	
	unsigned char temp;
	
	I2C_GenerateSTART(I2C1,ENABLE); //产生起始条件
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));//等待ACK
	
	I2C_Send7bitAddress(I2C1,Write_Pcf8563,I2C_Direction_Transmitter);//发送设备地址
	
	while(!I2C_CheckEvent(I2C1,  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//等待ACK
	
	I2C_Cmd(I2C1, ENABLE);//重新设置可以清楚EV6
	
	I2C_SendData(I2C1, read_address);//发送寄存器地址
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //等待ACK
	
	I2C_GenerateSTART(I2C1, ENABLE);//重新发送
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));//EV5
	
	I2C_Send7bitAddress(I2C1, Read_Pcf8563, I2C_Direction_Receiver);//发送读地址
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));//EV6
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
  
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)));
  
	temp = I2C_ReceiveData(I2C1);
  
	I2C_AcknowledgeConfig(I2C1, ENABLE);
  
	return temp;

	
}

void I2C1_WaitStandbyState(void)        
{
     vu16 SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    /* Read I2C1 SR1 register */
    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1,  Write_Pcf8563, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));      //estimate the value of ADDR
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);  
  I2C_GenerateSTOP(I2C1, ENABLE); 
}


































