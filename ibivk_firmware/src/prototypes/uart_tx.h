/*
 * uart_tx.h
 *
 *  Created on: 06.12.2014
 *      Author: Ludmila
 */

#ifndef UART_TX_H_
#define UART_TX_H_

#include "bshv_struct.h"

void DiagnosticAnswer(unsigned int received_cs, unsigned int received_id, unsigned int error_code);
void IbivkToPcMessageF1(BshvExtention bshv_ext, unsigned short cw, unsigned short sw);
void IbivkToPcMessageF2(BshvExtention bshv_ext, unsigned short cw, unsigned short sw, unsigned short dw[]);

#endif /* UART_TX_H_ */
