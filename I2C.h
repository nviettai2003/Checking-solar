#include "stm32f10x.h"

#define I2C_WRITE	0
#define I2C_READ 	1

void I2C_Config(void);
void I2C_Start(void);
void I2C_Address(uint8_t address, uint8_t direction);
void I2C_Stop(void);
void I2C1_WriteData(uint8_t address, uint8_t reg, uint8_t data);
void I2C1_ReadData(uint8_t addr_i2c, uint8_t addr_reg, uint8_t* buffer, uint8_t length);
