/*
 * rx_queue.h
 *
 *  Created on: 05.12.2014
 *      Author: Ludmila
 */

#ifndef RX_QUEUE_H_
#define RX_QUEUE_H_

#include "bshv.h"
#include "bus_controller.h"

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

#endif /* RX_QUEUE_H_ */
