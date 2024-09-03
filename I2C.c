#include "I2C.h"

void I2C_Config(void)
{
	/* clock */
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // I2C1
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // port B
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // alternate function
	
	/* Config IO I2C PB6, PB7: alternate function */
	GPIOB->CRL &= ~0xFF000000;
	GPIOB->CRL |= GPIO_CRL_CNF6 | GPIO_CRL_CNF7; //open drain
	GPIOB->CRL |= GPIO_CRL_MODE6  | GPIO_CRL_MODE7;
	
	
	/* I2C config */
	I2C1->CR1 |= I2C_CR1_SWRST;
	I2C1->CR1 &= ~ I2C_CR1_SWRST;
	
	I2C1->CR2 = 36;
	I2C1->OAR1 |= (1 << 14);
	I2C1->CCR = 180;
	I2C1->TRISE = 37;	

	I2C1->CR1 |= I2C_CR1_PE;
}


void I2C_Start(void)
{
	I2C1->CR1 |= I2C_CR1_ACK;
	
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB));
}

void I2C_Address(uint8_t address, uint8_t direction)
{
	uint8_t addr = (address << 1) | direction;
	I2C1->DR = addr;
	while(!(I2C1->SR1 & I2C_SR1_ADDR));
	
	(void) I2C1->SR1;
	(void) I2C1->SR2;
}

void I2C_Stop(void)
{
	I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C1_WriteData(uint8_t address, uint8_t reg, uint8_t data) 
{
	I2C_Start();

	I2C_Address(address, I2C_WRITE);

	// Gui thanh ghi
	I2C1->DR = reg;
	while (!(I2C1->SR1 & I2C_SR1_TXE));

	// Gui du lieu
	I2C1->DR = data;
	while (!(I2C1->SR1 & I2C_SR1_TXE));

	I2C_Stop();
}

void I2C1_ReadData(uint8_t addr_i2c, uint8_t addr_reg, uint8_t* buffer, uint8_t length) 
{
	I2C_Start();
	I2C_Address(addr_i2c, I2C_WRITE);

   // Gui thanh ghi
	I2C1->DR = addr_reg;
	while (!(I2C1->SR1 & I2C_SR1_TXE));

	I2C_Start();

	I2C_Address(addr_i2c, I2C_READ);

	// Doc du lieu
	while (length) 
	{
		if (length == 1) 
		{
			I2C1->CR1 &= ~I2C_CR1_ACK; // dissable ACK
			I2C_Stop();
		}

    if (I2C1->SR1 & I2C_SR1_RXNE) 
		{
			*buffer = I2C1->DR;
			buffer++;
			length--;
		}        
	}
	// enable ACK
	I2C1->CR1 |= I2C_CR1_ACK;
}
