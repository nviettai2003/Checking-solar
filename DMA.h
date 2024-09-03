#include "stm32f10x.h"

#define ADC1_DR_ADDRESS ((uint32_t)0x4001244C) // Base address + offset
#define BUFFER_SIZE 4

extern uint16_t adcValue[BUFFER_SIZE];

void DMA_Config(void);
