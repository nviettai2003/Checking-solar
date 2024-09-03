#include "stm32f10x.h"
#include "UART.h"
#include "delay.h"
#include "ADC.h"
#include "DMA.h"
#include "PWM.h"
#include "BME.h"

#define BUFFER_SIZE 4

uint16_t adcValue[BUFFER_SIZE];

volatile int trungian1 = 0;
volatile int trungian2;
volatile int32_t temperature, pressure, humidity;


int abs(int x)
{
	if(x>0) return x;
	else return -x;
}


int main(void) 
{
	Delay_Systick();
	ADC_Config();
	DMA_Config();
	PWM_Config();
	UART_Config();
	I2C_Config();
	BME280_Init();
	
	int tg1;
	int tg2;

	
	while(1)
	{
		while((abs(adcValue[0] - adcValue[1]) > 50) || (abs(adcValue[2] - adcValue[3]) > 50))
		{
			if (trungian1 > 1000)
				trungian1 = 1000;
			else if (trungian1 <300) 
				trungian1 = 300;
			else
				trungian1 = TIM2->CCR1;
				
			if (trungian2 > 1250)
				trungian2 = 1250;
			else if (trungian2 <300) 
				trungian2 = 300;
			else
				trungian2 = TIM2->CCR2;
			
			if(abs(adcValue[0] - adcValue[1]) > 100)
			{
				if(adcValue[0] > adcValue[1]) 
					tg1 = 1;
				else 
					tg1 = -1;
				TIM2->CCR1 = trungian1 + 10*tg1;
				delay_ms(10);
			}
			
			if(abs(adcValue[2] - adcValue[3]) > 100)
			{
				if(adcValue[2] < adcValue[3]) 
					tg2 = 1;
				else 
					tg2 = -1;
				TIM2->CCR2 = trungian2 + 5*tg2;
				delay_ms(10);
			}
		}
		
		
	}
	
}
