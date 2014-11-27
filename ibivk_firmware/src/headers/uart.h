/*
 * uart.h
 *
 *  Created on: 26.11.2014
 *      Author: Ludmila
 */

#ifndef UART_H_
#define UART_H_

#define BUFFER_LENGTH  (5 + 255)

typedef struct ReceiveBufferParams
{
	unsigned int buffer_cnt;
	unsigned int validate_cnt;
}ReceiveBufferParams;

#endif /* UART_H_ */
