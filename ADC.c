#include "ADC.h"

void ADC_Register(void) {
    // Enable ADC1 clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // Configure ADC prescaler
    RCC->CFGR &= ~RCC_CFGR_ADCPRE;
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6; // Set ADC prescaler to 6

    // Configure ADC
    ADC1->CR1 = ADC_CR1_SCAN; // Enable scan mode
    ADC1->CR2 = ADC_CR2_CONT  // Enable continuous conversion mode
              | ADC_CR2_DMA   // Enable DMA mode
              | ADC_CR2_ADON; // Enable ADC

    // Set sequence length to 4 conversions
    ADC1->SQR1 = ((4 - 1) << 20); // L[3:0] = 4-1 = 3 (4 conversions)

    // Set sequence: PA2 -> PA3 -> PA4 -> PA5
    ADC1->SQR3 = (2 << 0)   // SQ1: ADC channel 1 (PA2)
               | (3 << 5)   // SQ2: ADC channel 2 (PA3)
               | (4 << 10)  // SQ3: ADC channel 3 (PA4)
               | (5 << 15); // SQ4: ADC channel 4 (PA5)

    // Set sample time for each channel (55.5 cycles)
    ADC1->SMPR2 = ADC_SMPR2_SMP2_1 | ADC_SMPR2_SMP2_0
                | ADC_SMPR2_SMP3_1 | ADC_SMPR2_SMP3_0
                | ADC_SMPR2_SMP4_1 | ADC_SMPR2_SMP4_0
                | ADC_SMPR2_SMP5_1 | ADC_SMPR2_SMP5_0;

    // Calibrate ADC
		
		ADC1->CR2 |= ADC_CR2_ADON;
		__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
		ADC1->CR2 |= ADC_CR2_ADON;
	
    ADC1->CR2 |= ADC_CR2_RSTCAL;
    while (ADC1->CR2 & ADC_CR2_RSTCAL);
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL);

    // Start conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

void GPIO_ADC_Config(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2); // PA2
    GPIOA->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3); // PA3
    GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4); // PA4
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5); // PA5
}

void ADC_Config(void)
{
	GPIO_ADC_Config();
	ADC_Register();
}
