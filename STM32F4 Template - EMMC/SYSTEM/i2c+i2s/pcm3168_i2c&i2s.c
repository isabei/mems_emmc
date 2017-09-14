#include "stm32f4xx.h"
#include "pcm3168a.h"

#define I2C_pcm3168a_ADDR  0x88
//u16 I2S2_BUF1[4096];
//u16 I2S3_BUF1[4096];
//u16 I2S2_BUF2[4096];
//u16 I2S3_BUF2[4096];









void I2C2_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef I2C_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);  //I2C2外设也使能Clock
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2); 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);


	
	//配置I2C
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C_pcm3168a_ADDR; // 这个参数是指当STM32作为从设备时的地址，主设备访问通过该地址就可以访问STM32
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;             
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;  
  
	I2C_Init(I2C2, &I2C_InitStructure);
	//使能I2C模块
  I2C_Cmd(I2C2,ENABLE);
}

//*********************写操作*************************************//
void I2C2_WriteByte(u8 id,u8 write_address,u8 byte) 
{
	
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C2, ENABLE);

	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));//等待ACK
	
	I2C_Send7bitAddress(I2C2,id,I2C_Direction_Transmitter);//发送设备地址

	while(!I2C_CheckEvent(I2C2,  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//等待ACK
	
	I2C_SendData(I2C2, write_address);//发送寄存器地址
	
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //等待ACK
	
	I2C_SendData(I2C2, byte);//发送数据
	
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //等待ACK发送完成
	
	I2C_GenerateSTOP(I2C2, ENABLE);//发送完成
		
	
}



//**********************读操作**************************************//

unsigned char I2C2_ReadByte(u8 id,u8 read_address) 
{
	I2C2_Configuration();
	
	//I2C_WaitStandbyState();
	
	unsigned char temp;
	
	I2C_GenerateSTART(I2C2,ENABLE); //产生起始条件
	
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));//等待ACK
	
	I2C_Send7bitAddress(I2C2,id,I2C_Direction_Transmitter);//发送设备地址
	
	while(!I2C_CheckEvent(I2C2,  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//等待ACK
	
	I2C_Cmd(I2C2, ENABLE);//重新设置可以清楚EV6
	
	I2C_SendData(I2C2, read_address);//发送寄存器地址
	
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //等待ACK
	
	I2C_GenerateSTART(I2C2, ENABLE);//重新发送
	
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));//EV5
	
	I2C_Send7bitAddress(I2C2, id, I2C_Direction_Receiver);//发送读地址
	
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));//EV6
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);
  
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	while(!(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)));
  
	temp = I2C_ReceiveData(I2C2);
  
	I2C_AcknowledgeConfig(I2C2, ENABLE);
  
	return temp;

	
}

void I2C2_WaitStandbyState(void)        
{
  vu16 SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C2, ENABLE);
    /* Read I2C1 SR1 register */
    SR1_Tmp = I2C_ReadRegister(I2C2, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C2, I2C_pcm3168a_ADDR, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(I2C2, I2C_Register_SR1) & 0x0002));      //estimate the value of ADDR
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C2, I2C_FLAG_AF);  
  I2C_GenerateSTOP(I2C2, ENABLE); 
}


void I2S_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA, ENABLE);//I2S使用PB、PC
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//使能I2S_2
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE); 
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);//使能I2S_3
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,ENABLE); 
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,DISABLE);
	
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_5| GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,   GPIO_AF_SPI3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5,   GPIO_AF_SPI3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource12,  GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13,  GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15,  GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15,  GPIO_AF_SPI3);
  
	
  I2S_InitTypeDef I2S_InitStructure;
  SPI_I2S_DeInit(SPI2);
  SPI_I2S_DeInit(SPI3);
	
  I2S_InitStructure.I2S_Mode = I2S_Mode_SlaveRx; 
  I2S_InitStructure.I2S_Standard = I2S_Standard_LSB;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;//时钟失能
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_16k;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_Init(SPI2, &I2S_InitStructure);
	
  I2S_InitStructure.I2S_Mode = I2S_Mode_SlaveRx; 
  I2S_InitStructure.I2S_Standard = I2S_Standard_LSB;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;//时钟失能
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_16k;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_Init(SPI3, &I2S_InitStructure);
	
  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx,ENABLE);
  SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Rx,ENABLE);
	I2S_Cmd(SPI2, ENABLE);
	I2S_Cmd(SPI3, ENABLE);

}

void DMA1_I2S2_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);//使能DMA1时钟，I2S2
  DMA_DeInit(DMA1_Stream3);//数据流3 I2S2_RX设置为缺省值
	
	
  DMA_InitStructure.DMA_Channel  = DMA_Channel_0;
  
  DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t)(&(SPI2->DR));//(uint32_t)(SPI2->DR);  
  DMA_InitStructure.DMA_Memory0BaseAddr =0x68000000;
  DMA_InitStructure.DMA_DIR =  DMA_DIR_PeripheralToMemory;  //外设作为数据来源 DMA_DIR_PeripheralToMemory
  DMA_InitStructure.DMA_BufferSize =63488;  
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//同一通道只有一个外设
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存增加
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //16bit
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //设置成循环模式
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //设置正常模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_DoubleBufferModeConfig(DMA1_Stream3,0x68080000,DMA_Memory_0);
  DMA_DoubleBufferModeCmd(DMA1_Stream3,ENABLE);
	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);
	DMA_Init(DMA1_Stream3, &DMA_InitStructure);
//	DMA_ClearITPendingBit(DMA1_Stream3,DMA_IT_TCIF3|DMA_IT_TEIF3|DMA_IT_HTIF3|DMA_IT_DMEIF3|DMA_IT_FEIF3);
  
	DMA_Cmd(DMA1_Stream3, ENABLE);

 
  
 
	 
	
}


void DMA1_I2S3_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);//使能DMA1时钟，I2S2
  DMA_DeInit(DMA1_Stream0);//数据流3 I2S2_RX设置为缺省值
	
	
  DMA_InitStructure.DMA_Channel  = DMA_Channel_0;
  
  DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t)(&(SPI3->DR));//(uint32_t)(SPI2->DR);  
  DMA_InitStructure.DMA_Memory0BaseAddr =0x68100000;
  DMA_InitStructure.DMA_DIR =  DMA_DIR_PeripheralToMemory;  //外设作为数据来源 DMA_DIR_PeripheralToMemory
  DMA_InitStructure.DMA_BufferSize =63488; 	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//同一通道只有一个外设
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存增加
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //16bit
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //设置成循环模式
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //设置正常模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_DoubleBufferModeConfig(DMA1_Stream0,0x68180000,DMA_Memory_0);
  DMA_DoubleBufferModeCmd(DMA1_Stream0,ENABLE);
  DMA_Init(DMA1_Stream0, &DMA_InitStructure);
  DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);
  DMA_Init(DMA1_Stream0, &DMA_InitStructure);

  DMA_Cmd(DMA1_Stream0, ENABLE);


	
}





