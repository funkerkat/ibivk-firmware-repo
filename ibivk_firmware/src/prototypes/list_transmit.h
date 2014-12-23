/*
 * list_transmit.h
 *
 *  Created on: 19.12.2014
 *      Author: Ludmila
 */

#ifndef LIST_TRANSMIT_H_
#define LIST_TRANSMIT_H_


void InitListTransmit();
void MakeDiagnosticAnswer(unsigned short cs, unsigned short id, unsigned short code_error);
//void AddItemToListTransmit(ListTransmit* item);
void SendItemToUart();

#endif /* LIST_TRANSMIT_H_ */
