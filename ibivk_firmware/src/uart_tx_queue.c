/*
 * rx_queue.c
 *
 *  Created on: 05.12.2014
 *      Author: Ludmila
 */

// страндартная библиотека Си
#include <stdlib.h>

// библиотеки текущего проекта
#include "uart_tx_queue_struct.h"
#include "mil1553.h"
#include "ibivk_uart_packets.h"

// прототипы функций
#include "uart_tx.h"

UartTransmitQueue uart_tx_queue;

void UartTransmitQueueStore(Data1553* thisData1553)
{
	if (uart_tx_queue.store == DATA1553_ARRAY_SIZE)
	{
		// аварийный выход, очередь переполнена
	}

	// Загрузить данные в очередь
	uart_tx_queue.data1553_array[uart_tx_queue.store] = thisData1553;

	// Установить указатель на следующую ячейку очереди
	uart_tx_queue.store++;
}

Data1553* UartTransmitQueueRetrive()
{
	if (uart_tx_queue.retrive == DATA1553_ARRAY_SIZE)
	{
		// аварийный выход, очередь переполнена
	}

	// Считать данные из очереди
	Data1553* p_data = uart_tx_queue.data1553_array[uart_tx_queue.retrive];

	// Установить указатель на следующую ячейку очереди
	uart_tx_queue.retrive++;

	return p_data;
}

void SendUartPackets(Data1553 *p_data)
{
    unsigned short cw = p_data->command_word;
    unsigned int direction;
    MIL1553_GET_DIRECTION_BIT(cw, &direction);
    if(direction == BCtoRT)
    {
        IbivkToPcMessageF1(p_data->myBshvExtenion, p_data->command_word, p_data->status_word);
    }
    else if(direction == RTtoBC)
    {
    	IbivkToPcMessageF2(p_data->myBshvExtenion, p_data->command_word, p_data->status_word, p_data->data_words);
    }
    else
    {
		// аварийный выход, телеметрия - алгоритмическая ошибка
    }

    // освободить память
    free(p_data);
}


static void QueueShiftLeft()
{
	int i;
	for (i=0; i<DATA1553_ARRAY_SIZE; i++)
	{
		uart_tx_queue.data1553_array[i] = uart_tx_queue.data1553_array[i+1];
	}
	uart_tx_queue.retrive--;
	uart_tx_queue.store--;
}

void AnalyzeQueue()
{
	if (uart_tx_queue.store > uart_tx_queue.retrive)
	{
		// в очереди имеются сообщения для выдачи в УАРТ
		Data1553* p_data = UartTransmitQueueRetrive();
	    SendUartPackets(p_data);
	    QueueShiftLeft();
	}
	else
	{
		// аварийный выход, телеметрия - алгоритмическая ошибка
	}

}



