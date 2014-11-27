/*
 * selftest.c
 *
 *  Created on: 07.11.2014
 *      Author: Ludmila
 */
#include "xUart.h"

void Uart2_ErrorDetected(unsigned int error_code)
{
	int t = 1;
	t++;

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
