#ifndef __LED_H
#define __LED_H
#include "sys.h"




//LED端口定义
#define LED3 PAout(12)	
#define LED4 PAout(11)		
#define LED5 PGout(8)	

extern void LED_Init(void);//初始化		 				    
#endif





