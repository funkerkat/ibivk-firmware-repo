/*
 * rx_queue.c
 *
 *  Created on: 05.12.2014
 *      Author: Ludmila
 */

#include "uart_tx_queue_struct.h"

UartTransmitQueue uart_tx_queue;

void UartTransmitQueueStore(Data1553* thisData1553)
{
	// Загрузить данные в очередь
	uart_tx_queue.data1553_array[uart_tx_queue.store] = thisData1553;

	// Установить указатель на следующую ячейку очереди
	uart_tx_queue.store++;
}

void UartTransmitQueueRetrive()
{

}
