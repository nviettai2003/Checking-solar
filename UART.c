#include "UART.h"

void UART_Config(void)
{
	RCC->APB2ENR |= 0x08; // enable clock portB
	RCC->APB1ENR |= 0x40000; // enable clock USART2
	
	GPIOB->CRH &= ~0xFF00; // clear PortB11B10
	GPIOB->CRH |= 0x4A00; // PB10:1010 PB11:0100
	
	USART3->CR1 |= 0x2000; // enable uart
	USART3->CR1 |= 0x08 | 0x04; // enable bit Tx
	
	USART3->BRR = 0x0EA6; // 9600 baud rate
}


void UART_Write(char ch)
{
		USART3->DR = (uint32_t)(ch & 0x1FF); // ghi data
		while ((USART3->SR & USART_SR_TXE) != USART_CR1_TXEIE);
}

void printString(const char *str)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		UART_Write(str[i]);
	}
}

void reverse(char *str, int length) 
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char* intToStringStatic(int num) 
{
    static char str[50]; 
    int i = 0;
    int isNegative = 0;

    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    do {
        str[i++] = (num % 10) + '0';
        num = num / 10;
    } while (num != 0);

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';
    reverse(str, i);
    return str;
}

void PrintData(void)
{
	printString("Light intensity: ");
	printString(intToStringStatic(adcValue[0]));
	UART_Write('\n');
	printString("Temperature: ");
	printString(intToStringStatic(temperature));
	UART_Write('\n');
	printString("Pressure: ");
	printString(intToStringStatic(pressure));
	UART_Write('\n');
	UART_Write('\n');
}
