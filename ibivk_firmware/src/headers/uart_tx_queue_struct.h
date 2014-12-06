/*
 * rx_queue.h
 *
 *  Created on: 05.12.2014
 *      Author: Ludmila
 */

#ifndef UART_TX_QUEUE_STRUCT_H_
#define UART_TX_QUEUE_STRUCT_H_

#include "bus_controller_struct.h"
#include "bshv_struct.h"

typedef struct _Data1553
{
	BshvExtention myBshvExtenion;
	unsigned short command_word;
	unsigned short status_word;
	unsigned short data_words[32];
	unsigned int isNormal;
}Data1553;


#define DATA1553_ARRAY_SIZE	255
typedef struct _UartTransmitQueue
{
	Data1553* data1553_array[DATA1553_ARRAY_SIZE];
	unsigned int store;
	unsigned int retrive;
}UartTransmitQueue;

extern UartTransmitQueue uart_tx_queue;

#define QUEUE_CLEAN_POINTERS()		\
{									\
	uart_tx_queue.store = 0;		\
	uart_tx_queue.retrive = 0;		\
}

void UartTransmitQueueStore(Data1553* thisData1553);

#endif /* UART_TX_QUEUE_STRUCT_H_ */
