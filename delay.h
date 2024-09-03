#include "stm32f10x.h"
#include "UART.h"
#include "BME.h"

extern volatile uint32_t msTicks;
extern volatile uint32_t dem;

extern volatile int32_t temperature, pressure, humidity;

void Delay_Systick(void);
void SysTick_Handler(void);
void delay_ms(uint32_t ms);
