#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "sys.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "pcm3168a.h"
#include "fsmc_sram.h"
#include "stm32f4xx.h"
#include "sdio.h"


extern volatile unsigned int DMA_FLAG1;
extern volatile unsigned int DMA_FLAG2;
//extern u16 I2S2_BUF1[4096];
//extern u16 I2S3_BUF1[4096];
//extern u16 I2S2_BUF2[4096];
//extern u16 I2S3_BUF2[4096];
u32 i=0;
int b;

//u8 data[4] ;
//u8 data1[4];
//u8 shuju1[4096];
u8 shuju2[4096];
u8 data;
u32 cardstatus;
EmmcError result = EMMC_OK;

void USART_sendI2Sdata(void);
void NVIC_Configuration(void);
void RCC_Configuration(void);


int main(void)
{
	 delay_init(168);
	 LED_Init();
	 RCC_Configuration();
	
   FSMC_SRAM_Init();

   DMA1_I2S2_Configuration();
//	 DMA1_I2S3_Configuration();
   I2C2_Configuration();	
 
	 I2C2_WriteByte(0x88,0x50,0x01);   //single rate
   I2C2_WriteByte(0x88,0x51,0x13);   //16bit right-justed
	 I2C2_WriteByte(0x88,0x52,0x40);   //1234通道正常工作
   I2C2_WriteByte(0x88,0x53,0x0F);   //1234通道单端输入
	
	 I2S_Configuration();
	 Usart_Configuration();
	 NVIC_Configuration();
	
//	 FSMC_SRAM_WriteBuffer(data,0,4);
//   FSMC_SRAM_ReadBuffer(data1,0,4);
     LED3=1;
		 LED4=1;
		 LED5=1;
		 result = EmmcInit();
		 if(result != EMMC_OK)
		 {
			 LED3=0;
			 LED4=1;
			 LED5=1;
			 delay_ms(100);
		 }
		 result = EmmcBusTest();
//		 if(result != EMMC_OK)		 
//			{
//			 LED3=1;
//			 LED4=0;
//			 LED5=1;
//		 }
		 
			
		 
			
		 

	 
	
	while(1)
{
//	if(DMA_FLAG1==1)
//    {
//			 DMA_FLAG1=0;
//			 USART_sendI2Sdata();

//		 
//	   }
//			if(DMA_FLAG2==1)
//    {
//			 DMA_FLAG2=0;
////			 FSMC_SRAM_ReadBuffer(shuju2,0x00100000,4096);
//		   i=0x68100000;		
//	     for(a=0;a<4096;a++)
//	       {
//					 data=*(u8*)(i);
//		       USART_ClearFlag(USART1,USART_FLAG_TC);
//					 USART_SendData(USART1,data );
//					 while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
//					 i=i+0x00000001;
//						
//					 
//		       
//		     }
//	   }

}













	
//while(1)
//{
////	if(DMA_FLAG1==1)
////    {
////			 DMA_FLAG1=0;
////		   i=(u32)I2S2_BUF1;	
////	     for(a=0;a<8192;a++)
////	       {
////					 data=*(u8*)(i);
////		       USART_ClearFlag(USART1,USART_FLAG_TC);
////					 USART_SendData(USART1,data );
////					 while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
////					 i=i+0x00000001; 
////		     }
////	   }
////			if(DMA_FLAG2==1)
////    {
////			 DMA_FLAG2=0;
////		   i=(u32)I2S3_BUF1;		
////	     for(a=0;a<8192;a++)
////	       {
////					 data=*(u8*)(i);
////		       USART_ClearFlag(USART1,USART_FLAG_TC);
////					 USART_SendData(USART1,data );
////					 while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
////					 i=i+0x00000001;
////						
////					 
////		       
////		     }
////	   }

//}





	 
	
	
	

	
	
	
	
	
//	while(1)
//	{
//		LED3=0;
//		LED4=1;
//		LED5=1;
//		delay_ms(100);
//		
//		LED3=1;
//		LED4=0;
//		LED5=1;
//		delay_ms(100);
//		
//		LED3=1;
//		LED4=1;
//		LED5=0;
//		delay_ms(100);
//		
//	
//	}
	
	
	
	

//}

//void USART_sendI2Sdata(void)
//{
//	if(DMA_GetCurrentMemoryTarget(DMA2_Stream1) == 1)
//	{
//		 i=0x68000000;		
//	     for(a=0;a<126976;a++)
//	       {
//					 data=*(u8*)(i);
//		       USART_ClearFlag(USART1,USART_FLAG_TC);
//					 USART_SendData(USART1,data );
//					 while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
//					 i=i+0x00000001;
//					}
//	}
//	else
//	{
//		i=0x68080000;		
//	     for(a=0;a<126976;a++)
//	       {
//					 data=*(u8*)(i);
//		       USART_ClearFlag(USART1,USART_FLAG_TC);
//					 USART_SendData(USART1,data );
//					 while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
//					 i=i+0x00000001;
//					}
//	
//	}








}



void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	#ifdef VECT_TAB_RAM
	NVIC_SetVectorTable(NVIC_VectTab_RAM,0x0);
	#else
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0);
	#endif 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

}

void RCC_Configuration(void)
{
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();
}



















































































































































































