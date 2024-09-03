#include "stm32f10x.h"
#include "string.h"

extern uint16_t adcValue[4];
extern volatile int32_t temperature, pressure, humidity;

void UART_Config(void);
void UART_Write(char ch);
void printString(const char *str);
void reverse(char *str, int length);
char* intToStringStatic(int num);
void PrintData(void);
