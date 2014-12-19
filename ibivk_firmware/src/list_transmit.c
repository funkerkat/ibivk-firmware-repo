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

// -------------------------------------------------------------------------
void SendItemToUart()
{
	// извлечь из списка первый элемент
	ListTransmit* item = head_list_transmit;
	head_list_transmit = item->next;

	// выдать извлеченный элемент в УАРТ
	switch(item->packet_id)
	{
		case ID_PACKET_IBIVK_TO_PC_DIAGNOSTIC_ANSWER:
			DiagnosticAnswer(item->data.data_diagnostic_answer.receiver_id, item->data.data_diagnostic_answer.receiver_cs, item->data.data_diagnostic_answer.code_error);
			break;

		case ID_PACKET_IBIVK_TO_PC_F1:
			//AddItemPriorityLeast(item);
			break;

		case ID_PACKET_IBIVK_TO_PC_F2:
			//IbivkToPcMessageF2();
			break;

		default:
			// алгоритмическая ошибка
			break;
	}

	// освободить память
	free(item);
}

// -------------------------------------------------------------------------
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


// -------------------------------------------------------------------------
/*
void AddData1553ToListTransmit(Data1553* data)
{
	// создать новый объект в буфере для выдачи в УАРТ
	ListTransmit* item = (ListTransmit*) malloc(sizeof(ListTransmit));
	item->data.data1553_obj = data;

	// определить формат пакета УАРТ (Ф1/Ф2)
	unsigned short direction;
	MIL1553_GET_DIRECTION_BIT(data->command_word, &direction);

	// пределить ID пакета
	if (direction == BCtoRT) 		{ item->packet_id = ID_PACKET_IBIVK_TO_PC_F1; }
	else if (direction == RTtoBC) 	{ item->packet_id = ID_PACKET_IBIVK_TO_PC_F2; }
	else
	{
		// агоритмическая ошибка
	}

	// включить объект в буфер
	AddItemPriorityLeast(item);
}

void AddDiagnAnswerToListTransmit(DataDiagnosticAnswer* data)
{
	// создать новый объект в буфере для выдачи в УАРТ
	ListTransmit* item = (ListTransmit*) malloc(sizeof(ListTransmit));
	item->data.data_diagnostic_answer_obj = data;

	// пределить ID пакета
	item->packet_id = ID_DIAGNOSTIC_ANSWER;

	// включить объект в буфер
	AddItemPriorityFirst(item);
}
*/

// -------------------------------------------------------------------------

void AddItemToListTransmit(ListTransmit* item)
{
	switch(item->packet_id)
	{
		case ID_PACKET_IBIVK_TO_PC_DIAGNOSTIC_ANSWER:
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
	head_list_transmit = NULL;
}


void DemoListTransmit2()
{
	InitListTransmit();
}

void DemoListTransmit()
{
	InitListTransmit();

	ListTransmit* item1 = (ListTransmit*) malloc(sizeof(ListTransmit));
	item1->packet_id = 5;
	AddItemPriorityFirst(item1);

	ListTransmit* item2 = (ListTransmit*) malloc(sizeof(ListTransmit));
	item2->packet_id = 7;
	AddItemPriorityLeast(item2);

	ListTransmit* item3 = (ListTransmit*) malloc(sizeof(ListTransmit));
	item3->packet_id = 8;
	AddItemPriorityLeast(item3);



	ListTransmit* item4 = (ListTransmit*) malloc(sizeof(ListTransmit));
	item4->packet_id = 9;
	AddItemPriorityFirst(item4);




	ListTransmit* item5 = (ListTransmit*) malloc(sizeof(ListTransmit));
	item5->packet_id = 0x0a;
	AddItemPriorityLeast(item5);



	ListTransmit* item6 = (ListTransmit*) malloc(sizeof(ListTransmit));
	item6->packet_id = 0x0b;
	AddItemPriorityFirst(item6);




	while(1)
	{

		if (head_list_transmit != NULL)
		{
			SendItemToUart();
		}


		int t = 1;
		t++;

	}


	int t = 1;
	t++;

}

