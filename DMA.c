#include "DMA.h"

void DMA_Config(void) {
    RCC->AHBENR |= RCC_AHBENR_DMA1EN; // Enable clock for DMA1

    // Disable DMA1_Channel1 before configuring it
    DMA1_Channel1->CCR &= ~DMA_CCR1_EN;

    // Set peripheral address to ADC1 data register
    DMA1_Channel1->CPAR = (uint32_t)ADC1_DR_ADDRESS;

    // Set memory address to the adcValue array
    DMA1_Channel1->CMAR = (uint32_t)&adcValue[0];

    // Set the number of data items to transfer
    DMA1_Channel1->CNDTR = BUFFER_SIZE;

    // Configure the DMA channel
    DMA1_Channel1->CCR = DMA_CCR1_PL_1       // Priority level: High
                       | DMA_CCR1_MSIZE_0    // Memory size: 16-bit
                       | DMA_CCR1_PSIZE_0    // Peripheral size: 16-bit
                       | DMA_CCR1_MINC       // Memory increment mode
                       | DMA_CCR1_CIRC       // Circular mode
                       | DMA_CCR1_EN;        // Enable DMA
}
