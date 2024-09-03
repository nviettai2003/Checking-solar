#include "stm32f10x.h"
#include "I2C.h"

#define BME280_ADDRESS 0x76 // Ðia chi I2C cua BME280
#define BME280_REG_TEMP_MSB 0xFA
#define BME280_REG_PRESS_MSB 0xF7
#define BME280_REG_HUM_MSB 0xFD
#define BME280_REG_CTRL_MEAS 0xF4
#define BME280_REG_CTRL_HUM 0xF2
#define BME280_REG_CONFIG 0xF5


void BME280_Init(void);
void BME280_ReadCompensatedData(int32_t* temperature, uint32_t* pressure, uint32_t* humidity);
