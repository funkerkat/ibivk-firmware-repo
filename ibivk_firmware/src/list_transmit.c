/*
 * list_transmit.c
 *
 *  Created on: 18.12.2014
 *      Author: Ludmila
 */

// страндартная библиотека Си
#include <stdlib.h>

// библиотеки текущего проекта
#include "list_transmit_struct.h"
#include "ibivk_uart_packets.h"
#include "mil1553.h"

// прототипы функций
#include "uart_tx.h"

ListTransmit* head_list_transmit;

static void AddItemPriorityFirst(ListTransmit* new_item)
{
	// список изначально пустой, тогда новый элемент -- одновременно первый и последний
	if (head_list_transmit == NULL)
	{
		head_list_transmit = new_item;
		new_item->next = NULL;
		return;
	}

	// список изначально не пустой, вставить новый элемент перед первым
	new_item->next = head_list_transmit;
	head_list_transmit = new_item;
}

static void AddItemPriorityLeast(ListTransmit* new_item)
{
	// новый элемент будет последним в списке
	new_item->next = NULL;

	// список изначально пустой, тогда новый элемент -- одновременно первый и последний
	if (head_list_transmit == NULL)
	{
		head_list_transmit = new_item;
		return;
	}

	// список изначально не пустой, найти последний элемент и вставить новый элемент после него
	ListTransmit* p = head_list_transmit;		// установить указатель на начало списка
	while(p->next) { p = p->next; } 			// найти конечный элемент списка

	// вставить новый элемент в конец списка
	p->next = new_item;
}


void SendItemToUart()
{
	// извлечь из списка первый элемент
	ListTransmit* item = head_list_transmit;
	head_list_transmit = item->next;

	// выдать извлеченный элемент в УАРТ
	switch(item->packet_id)
	{
		case ID_PACKET_IBIVK_TO_PC_DIAGNOSTIC_ANSWER:
			DiagnosticAnswer  (item->data.data_diagnostic_answer.receiver_id,
							   item->data.data_diagnostic_answer.receiver_cs,
							   item->data.data_diagnostic_answer.code_error);
			break;


		case ID_PACKET_IBIVK_TO_PC_TM:
			RS485_send_tmi(item->data.data_tmi.p_tmi);
			// очистить телеметрию
			CleanTmi();
			break;


		case ID_PACKET_IBIVK_TO_PC_F1:
			IbivkToPcMessageF1(item->data.data_ibivk_to_pc_f1.myBshvExtenion,
							   item->data.data_ibivk_to_pc_f1.command_word,
							   item->data.data_ibivk_to_pc_f1.status_word);
			break;

		case ID_PACKET_IBIVK_TO_PC_F2:
			IbivkToPcMessageF2(item->data.data_ibivk_to_pc_f2.myBshvExtenion,
							   item->data.data_ibivk_to_pc_f2.command_word,
							   item->data.data_ibivk_to_pc_f2.status_word,
							   item->data.data_ibivk_to_pc_f2.data_words);
			break;

		default:
			// алгоритмическая ошибка
			break;
	}

	// освободить память
	free(item);
}

void AddItemToListTransmit(ListTransmit* item)
{
	switch(item->packet_id)
	{
		case ID_PACKET_IBIVK_TO_PC_DIAGNOSTIC_ANSWER:
			AddItemPriorityFirst(item);
			break;

		case ID_PACKET_IBIVK_TO_PC_TM:
			AddItemPriorityFirst(item);
			break;

		case ID_PACKET_IBIVK_TO_PC_F1:
			AddItemPriorityLeast(item);
			break;

		case ID_PACKET_IBIVK_TO_PC_F2:
			AddItemPriorityLeast(item);
			break;

		default:
			// алгоритмическая ошибка
			break;
	}
}

void MakeDiagnosticAnswer(unsigned short cs, unsigned short id, unsigned short code_error)
{
	// Подготовить диагностический ответ и загрузить в буфер УАРТ на выдачу
	ListTransmit* item = (ListTransmit*) malloc(sizeof(ListTransmit));
	item->packet_id = ID_PACKET_IBIVK_TO_PC_DIAGNOSTIC_ANSWER;
	item->data.data_diagnostic_answer.receiver_id = id;
	item->data.data_diagnostic_answer.receiver_cs = cs;
	item->data.data_diagnostic_answer.code_error = code_error;
	AddItemToListTransmit(item);
}

void InitListTransmit()
{
	// Установить начало очереди в NULL (т. е. очередь отсутствует)
	head_list_transmit = NULL;
}

