/*
 * uart.h
 *
 *  Created on: 17.10.2014
 *      Author: Ludmila
 */

#ifndef UART_H_
#define UART_H_

// ----- UART Serial Interface -----
#define UART1_BASE_ADDRESS			0x80000100
#define UART2_BASE_ADDRESS			0x80000900

#define UART_REG_DATA				0x00
#define UART_REG_STATUS				0x04
#define UART_REG_CONTROL			0x08
#define UART_REG_SCALER				0x0C

// ----- UART Status Register (UART_REG_STATUS) -----
#define UART_REG_STATUS_RCNT		(0x3F	<< 26)


#define UART_REG_STATUS_RCNT_SHIFT	()

#endif /* UART_H_ */
