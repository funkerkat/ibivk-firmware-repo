/*
 * selftest.c
 *
 *  Created on: 07.11.2014
 *      Author: Ludmila
 */
#include "xUart.h"
#include "tmi_struct.h"

void Uart2_Selftest(unsigned int error_detected)
{
	// есть ошибки
	if (error_detected)
	{
		// очистить биты неисправности из регистра УАРТ
		UART2_STATUS_RESET();
		// занести отчет о неисправности в телеметрию
		ibivk_tmi.selftest_uart.uart2_error_code = error_detected;
	}
}


void Uart2_ErrorDetected2(unsigned int error_code)
{
	switch(error_code)
	{
		case 1:	// Break received (BR) - indicates that a BREAK has been received.
			UART2_STATUS_RESET();
			break;

		case 2:	// Overrun (OV) - indicates that one or more character have been lost due to overrun.
			UART2_STATUS_RESET();
			break;

		case 3:	// Parity error (PE) - indicates that a parity error was detected.
			UART2_STATUS_RESET();
			break;

		case 4:	// Framing error (FE) - indicates that a framing error was detected.
			UART2_STATUS_RESET();
			break;

		default: // Unknown code
			break;
	}

}
