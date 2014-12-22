/*
 * xUart.h
 *
 *  Created on: 22.10.2014
 *      Author: Ludmila
 */
#include "xSystem.h"

#ifndef XUART_H_
#define XUART_H_

// ----- UART Serial Interface -----
#define UART1_BASE_ADDRESS			0x80000100
#define UART2_BASE_ADDRESS			0x80000900

#define UART_REG_DATA				0x00
#define UART_REG_STATUS				0x04
#define UART_REG_CONTROL			0x08
#define UART_REG_SCALER				0x0C

// ----- UART Status Register (UART_REG_STATUS) -----
// 31: 26 Receiver FIFO count (RCNT) - shows the number of data frames in the receiver FIFO.
#define UART_STATUS_RCNT			(26)

// 25: 20 Transmitter FIFO count (TCNT) - shows the number of data frames in the transmitter FIFO.
#define UART_STATUS_TCNT			(20)

// 10 Receiver FIFO full (RF) - indicates that the Receiver FIFO is full.
#define UART_STATUS_RF				(10)

// 9 Transmitter FIFO full (TF) - indicates that the Transmitter FIFO is full.
#define UART_STATUS_TF				(9)

// 8 Receiver FIFO half-full (RH) -indicates that at least half of the FIFO is holding data.
#define UART_STATUS_RR				(8)

// 7 Transmitter FIFO half-full (TH) - indicates that the FIFO is less than half-full.
#define UART_STATUS_TH				(7)

// 6 Framing error (FE) - indicates that a framing error was detected.
#define UART_STATUS_FE				(6)

// 5 Parity error (PE) - indicates that a parity error was detected.
#define UART_STATUS_PE				(5)

// 4 Overrun (OV) - indicates that one or more character have been lost due to overrun.
#define UART_STATUS_OV				(4)

// 3 Break received (BR) - indicates that a BREAK has been received.
#define UART_STATUS_BR				(3)

// 2 Transmitter FIFO empty (TE) - indicates that the transmitter FIFO is empty.
#define UART_STATUS_TE				(2)

// 1 Transmitter shift register empty (TS) - indicates that the transmitter shift register is empty.
#define UART_STATUS_TS				(1)

// 0 Data ready (DR) - indicates that new data is available in the receiver holding register
#define UART_STATUS_DR				(0)


// ----- UART Control Register (UART_REG_CONTROL) -----
// Receiver FIFO interrupt enable (RF) - when set, Receiver FIFO level interrupts are enabled
#define UART_CONTROL_RF					(0 << 10)

// Transmitter FIFO interrupt enable (TF) - when set, Transmitter FIFO level interrupts are enabled.
#define UART_CONTROL_TF					(0 << 9)

// External Clock (EC) - if set, the UART scaler will not be clocked.
#define UART_CONTROL_EC					(0 << 8)

// Loop back (LB) - if set, loop back mode will be enabled
#define UART_CONTROL_LB					(0 << 7)

// Flow control (FL) - if set, enables flow control using CTS/RTS (when implemented)
#define UART_CONTROL_FL					(0 << 6)

// Parity enable (PE) - if set, enables parity generation and checking (when implemented)
#define UART_CONTROL_PE					(0 << 5)

// Parity select (PS) - selects parity polarity (0 = even parity, 1 = odd parity) (when implemented)
#define UART_CONTROL_PS					(0 << 4)

//Transmitter interrupt enable (TI) - if set, interrupts are generated when a frame is transmitted.
#define UART_CONTROL_TI					(0 << 3)

// Receiver interrupt enable (RI) - if set, interrupts are generated when a frame is received.
#define UART_CONTROL_RI					(1 << 2)

// Transmitter enable (TE) - if set, enables the transmitter.
#define UART_CONTROL_TE					(1 << 1)

// Receiver enable (RE) - if set, enables the receiver.
#define UART_CONTROL_RE					(1 << 0)

#define UART_CONTROL_VALUE (UART_CONTROL_RF|UART_CONTROL_TF|UART_CONTROL_EC|UART_CONTROL_LB|UART_CONTROL_FL|\
		UART_CONTROL_PE|UART_CONTROL_PS|UART_CONTROL_TI|UART_CONTROL_RI|UART_CONTROL_TE|UART_CONTROL_RE)

// ----- UART API -----

#define UART1_READ_STATUSBIT_TS		(((*UART1_READ_STATUS)>>UART_STATUS_TS)&0x01)
#define UART2_READ_STATUSBIT_TS		(((*UART2_READ_STATUS)>>UART_STATUS_TS)&0x01)

#define UART1_READ_STATUSBIT_TE		(((*UART1_READ_STATUS)>>UART_STATUS_TE)&0x01)
#define UART2_READ_STATUSBIT_TE		(((*UART2_READ_STATUS)>>UART_STATUS_TE)&0x01)

#define UART1_SELFTEST(value) 	{ *value = ((*UART1_READ_STATUS)>>UART_STATUS_BR)&0x0F; }
#define UART2_SELFTEST(value) 	{ *value = ((*UART2_READ_STATUS)>>UART_STATUS_BR)&0x0F; }

#define UART1_STATUS_RESET() 	{*((int*)(UART1_BASE_ADDRESS + UART_REG_STATUS)) = 0;}
#define UART2_STATUS_RESET() 	{*((int*)(UART2_BASE_ADDRESS + UART_REG_STATUS)) = 0;}

// Чтение статусного регистра:
#define UART1_READ_STATUS	((int*)(UART1_BASE_ADDRESS + UART_REG_STATUS))
#define UART2_READ_STATUS	((int*)(UART2_BASE_ADDRESS + UART_REG_STATUS))

// Задержка до полного опустошения FIFO на передачу:
#define UART1_TRANSMIT_DELAY() 	{ while(!(UART1_READ_STATUSBIT_TS && UART1_READ_STATUSBIT_TE)) {;} }
#define UART2_TRANSMIT_DELAY() 	{ while(!(UART2_READ_STATUSBIT_TS && UART2_READ_STATUSBIT_TE)) {;} }

// Передача одного байта в UART с задержкой до полного опустошения FIFO:
#define UART1_SEND_BYTE(value) 	{*((int*)(UART1_BASE_ADDRESS + UART_REG_DATA)) = value&0xFF; UART1_TRANSMIT_DELAY(); }
#define UART2_SEND_BYTE(value) 	{*((int*)(UART2_BASE_ADDRESS + UART_REG_DATA)) = value&0xFF; UART2_TRANSMIT_DELAY(); }

// Передача одного байта в UART с задержкой до полного опустошения FIFO с автоматической проверкой статусного регистра:
#define UART2_SEND_BYTE_POLLING(value, error) 	{*((int*)(UART2_BASE_ADDRESS + UART_REG_DATA)) = value&0xFF; UART2_TRANSMIT_DELAY(); *error = ((*UART2_READ_STATUS)>>UART_STATUS_BR)&0x0F; }


#define UART2_READ_BYTE	((int*)(UART2_BASE_ADDRESS + UART_REG_DATA))



#define UART2_SET_BAUDRATE(baudrate) {*((int*)(UART2_BASE_ADDRESS + UART_REG_SCALER)) = SYSTEM_CLOCK/8/(baudrate);}
#define UART2_SET_CONTROL_REG()	{*((int*)(UART2_BASE_ADDRESS + UART_REG_CONTROL)) = UART_CONTROL_VALUE;}

#define UART_INIT(baudrate){ UART2_SET_BAUDRATE(baudrate); UART2_SET_CONTROL_REG(); }

#endif /* XUART_H_ */
