#include "stm32f4xx.h"

void I2C1_Configuration(void);
void I2C1_WriteByte(u8 write_address,u8 byte);
unsigned char I2C1_ReadByte(u8 read_address);
void I2C1_WaitStandbyState(void);

