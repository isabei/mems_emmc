#include "stm32f4xx.h" 



void I2C2_Configuration(void);
void I2C2_WriteByte(u8 id,u8 write_address,u8 byte);
unsigned char I2C2_ReadByte(u8 id,u8 read_address) ;
void I2C2_WaitStandbyState(void);
void I2S_Configuration(void);
void DMA1_I2S2_Configuration(void);
void DMA1_I2S3_Configuration(void);



