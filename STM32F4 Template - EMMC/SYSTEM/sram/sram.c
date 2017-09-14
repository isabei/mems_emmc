#include "stm32f4xx.h"
#include "fsmc_sram.h"

#define Bank1_SRAM3_ADDR ((u32)(0x68000000))

void FSMC_SRAM_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
 
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	
      
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_14|
                                GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF, &GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2 |GPIO_Pin_3 |
                                GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_8|
                                GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|
                                GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_15|
                                GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4,  GPIO_AF_FSMC);//NOE
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5,  GPIO_AF_FSMC);//NWE
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0,  GPIO_AF_FSMC);//NBL0
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1,  GPIO_AF_FSMC);//NBL1
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, GPIO_AF_FSMC);//NE3
 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);//D0
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);//D1
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0,  GPIO_AF_FSMC);//D2
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1,  GPIO_AF_FSMC);//D3
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7,  GPIO_AF_FSMC);//D4
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8,  GPIO_AF_FSMC);//D5
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9,  GPIO_AF_FSMC);//D6
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);//D7
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);//D8
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);//D9
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);//D10
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);//D11
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);//D12
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8,  GPIO_AF_FSMC);//D13
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9,  GPIO_AF_FSMC);//D14
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);//D15
 
 
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource0,  GPIO_AF_FSMC);//A0
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource1,  GPIO_AF_FSMC);//A1
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource2,  GPIO_AF_FSMC);//A2
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource3,  GPIO_AF_FSMC);//A3
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource4,  GPIO_AF_FSMC);//A4
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource5,  GPIO_AF_FSMC);//A5
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource12, GPIO_AF_FSMC);//A6
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource13, GPIO_AF_FSMC);//A7
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource14, GPIO_AF_FSMC);//A8
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource15, GPIO_AF_FSMC);//A9
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0,  GPIO_AF_FSMC);//A10
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource1,  GPIO_AF_FSMC);//A11
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource2,  GPIO_AF_FSMC);//A12
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource3,  GPIO_AF_FSMC);//A13
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource4,  GPIO_AF_FSMC);//A14
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource5,  GPIO_AF_FSMC);//A15
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);//A16
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);//A17
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FSMC);//A18
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource3,  GPIO_AF_FSMC);//A19
  




    
  
  FSMC_NORSRAMTimingInitTypeDef p;
                    
  p.FSMC_AddressSetupTime = 0x00;     
  p.FSMC_AddressHoldTime = 0x00;        
  p.FSMC_DataSetupTime = 0x08;             
  p.FSMC_BusTurnAroundDuration = 0x00;
  p.FSMC_CLKDivision = 0x00;
  p.FSMC_DataLatency = 0x00;
  p.FSMC_AccessMode = FSMC_AccessMode_A;     


  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;    
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);                                     
                                            
}




/* -------------------------------
   指定地址开始连续写入n个字节
   pBuffer:字节指针
   WriteAddr:写入地址
   n:写入的个数
--------------------------------*/
void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 n)
{
  for(;n!=0;n--)
  {                                        
    *(vu8*)(Bank1_SRAM3_ADDR+WriteAddr)=*pBuffer;    
//    WriteAddr+=2;
		WriteAddr++;
    pBuffer++;
  }
}

/*-------------------------------
   指定地址开始连续读出n个字节
   pBuffer:字节指针
   ReadAddr:读出地址
   n:读出个数
--------------------------------*/
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 n)
{
  for(;n!=0;n--)
  {                                            
    *pBuffer++=*(vu8*)(Bank1_SRAM3_ADDR+ReadAddr);
//    ReadAddr+=2;
		ReadAddr++;
  }
}








