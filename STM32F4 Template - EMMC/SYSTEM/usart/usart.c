#include "stm32f4xx.h"
#include "usart.h"


void Usart_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 
//  NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_USART1); 
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  
 
  

  USART_DeInit(USART1);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
  USART_InitStructure.USART_StopBits = USART_StopBits_1;  
  USART_InitStructure.USART_Parity = USART_Parity_No;  
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   
  USART_Init(USART1, &USART_InitStructure);
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_Cmd(USART1, ENABLE); 
 
//  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//�����ж�	
//  USART_ITConfig(USART1, USART_IT_TC , ENABLE);//��������ж�
//  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
//		
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

//	//Usart1 NVIC ����
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����




	
 






}








































