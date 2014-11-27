/*
 * uart.c
 *
 *  Created on: 17.10.2014
 *      Author: Ludmila
 */
#include "uart.h"

void InitUart()
{

}

void SendByte(unsigned int data)
{
	*((int*)(UART1_BASE_ADDRESS + UART_REG_DATA)) = data;
}
