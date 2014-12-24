/*
 * uart_tx.h
 *
 *  Created on: 06.12.2014
 *      Author: Ludmila
 */

#ifndef UART_TX_H_
#define UART_TX_H_

#include "bshv_struct.h"
#include "tmi_struct.h"

void Uart_send_diagnostic_answer(unsigned int received_id, unsigned int received_cs, unsigned int error_code);
void Uart_send_tmi(Tmi* this_tmi);
void Uart_send_ibivk_to_pc_f1(BshvExtention bshv_ext, unsigned short cw, unsigned short sw);
void Uart_send_ibivk_to_pc_f2(BshvExtention bshv_ext, unsigned short cw, unsigned short sw, unsigned short dw[]);

#endif /* UART_TX_H_ */
