/*
 * timer.c
 *
 *  Created on: 04.12.2014
 *      Author: Ludmila
 */

// страндартная библиотека Си
#include <stdlib.h>

// библиотеки для работы с периферией
#include "xSystem.h"
#include "xMil1553BC.h"
#include "xTimer.h"

// библиотеки текущего проекта
#include "list_transmit_struct.h"
#include "ibivk_uart_packets.h"
#include "mil1553.h"
#include "nodes.h"

// прототипы функций
#include "list_microseconds.h"
#include "list_transmit.h"

void ExecuteCore1553()
{
	// Запрограммировать таймер на следующее сообщение (если оно запланировано в этой секунде)
	if(global_microsecond->timer_value != 0) { TIMER2_START(global_microsecond->timer_value); }

	// Выдать сообщение МКИО
	CORE1553_RELOAD(global_microsecond->core1553_entry->reg_leon, global_microsecond->core1553_entry->reg_core1553);
	CORE1553_START_EXECUTION();

	// Пока длится выдача сообщения, начать собрать данные о сообщении (для последующей выдачи в UART ответного сообщения)

	// 1. выделить память для хранения данных для последующей выдачи в УАРТ
	ListTransmit* item = (ListTransmit*) malloc(sizeof(ListTransmit));

	// 2. скопировать данные БШВ (без разницы, в структуру data_ibivk_to_pc_f1 или data_ibivk_to_pc_f2, т. к. union)
	item->data.data_ibivk_to_pc_f2.myBshvExtenion.myBshv.fouryears = global_microsecond->base_node_bshv->myBshv.fouryears;
	item->data.data_ibivk_to_pc_f2.myBshvExtenion.myBshv.day 		= global_microsecond->base_node_bshv->myBshv.day;
	item->data.data_ibivk_to_pc_f2.myBshvExtenion.myBshv.hour 		= global_microsecond->base_node_bshv->myBshv.hour;
	item->data.data_ibivk_to_pc_f2.myBshvExtenion.myBshv.minute 	= global_microsecond->base_node_bshv->myBshv.minute;
	item->data.data_ibivk_to_pc_f2.myBshvExtenion.myBshv.second 	= global_microsecond->base_node_bshv->myBshv.second;
	item->data.data_ibivk_to_pc_f2.myBshvExtenion.microsecond		= global_microsecond->microsecond;

	// 3. скопировать командное слово (без разницы, в структуру data_ibivk_to_pc_f1 или data_ibivk_to_pc_f2, т. к. union)
	unsigned short cw = global_microsecond->core1553_entry->cmd_block_1.CommandWord1;
	item->data.data_ibivk_to_pc_f2.command_word = cw;

	// 4. проанализировать формат сообщения (Ф1 или Ф2)
	unsigned int direction_bit;
	MIL1553_GET_DIRECTION_BIT(cw, &direction_bit);

	// Ожидание окончания работы ядра 1553 (для считывания ответного слова)
	CORE1553_EXECUTION_DELAY();

	// Считать и проанализировать ответное слово на наличие ненулевых разрядов (маскируются старшие 5 разрядов - адрес ОУ)
	unsigned short sw = global_microsecond->core1553_entry->cmd_block_1.StatusWord1;

	// Сохранить данные для выдачи УАРТ:
	 if (direction_bit == BCtoRT)
	 {
		 // установить тип пакета
		 item->packet_id = ID_PACKET_IBIVK_TO_PC_F1;
		 // скопировать ответное слово
		 item->data.data_ibivk_to_pc_f1.status_word = sw;
	 }
	 else if(direction_bit == RTtoBC)
	 {
		 // установить тип пакета
		 item->packet_id = ID_PACKET_IBIVK_TO_PC_F2;
		 // скопировать ответное слово
		 item->data.data_ibivk_to_pc_f2.status_word = sw;
		 // скопировать слова данных
		 int i; for (i=0; i<32; i++) { item->data.data_ibivk_to_pc_f2.data_words[i] = global_microsecond->core1553_entry->data_words[i]; }
	 }
	 else
	 {
		 // алгоритмическая ошибка
	 }

	 // 3. отправить на хранения данные (до наступления очереди выдачи в УАРТ)
	 AddItemToListTransmit(item);

	// Переключить указатель на следующее сообщение и удалить текущую запись из списка микросекунд
	NodeMicrosecond* nextNodeMicrosecond = global_microsecond->next;
	RemoveItemFromListMicrosecond(global_microsecond);
	global_microsecond = nextNodeMicrosecond;
}
