/*
 * list_transmit.c
 *
 *  Created on: 18.12.2014
 *      Author: Ludmila
 */

// ������������ ���������� ��
#include <stdlib.h>

// ���������� �������� �������
#include "list_transmit_struct.h"
#include "ibivk_uart_packets.h"
#include "mil1553.h"

// ��������� �������
#include "uart_tx.h"


ListTransmit* head_list_transmit;

// -------------------------------------------------------------------------
void SendItemToUart()
{
	// ������� �� ������ ������ �������
	ListTransmit* item = head_list_transmit;
	head_list_transmit = item->next;

	// ������ ����������� ������� � ����
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
			// ��������������� ������
			break;
	}

	// ���������� ������
	free(item);
}

// -------------------------------------------------------------------------
static void AddItemPriorityFirst(ListTransmit* new_item)
{
	// ������ ���������� ������, ����� ����� ������� -- ������������ ������ � ���������
	if (head_list_transmit == NULL)
	{
		head_list_transmit = new_item;
		new_item->next = NULL;
		return;
	}

	// ������ ���������� �� ������, �������� ����� ������� ����� ������
	new_item->next = head_list_transmit;
	head_list_transmit = new_item;
}

static void AddItemPriorityLeast(ListTransmit* new_item)
{
	// ����� ������� ����� ��������� � ������
	new_item->next = NULL;

	// ������ ���������� ������, ����� ����� ������� -- ������������ ������ � ���������
	if (head_list_transmit == NULL)
	{
		head_list_transmit = new_item;
		return;
	}

	// ������ ���������� �� ������, ����� ��������� ������� � �������� ����� ������� ����� ����
	ListTransmit* p = head_list_transmit;		// ���������� ��������� �� ������ ������
	while(p->next) { p = p->next; } 			// ����� �������� ������� ������

	// �������� ����� ������� � ����� ������
	p->next = new_item;
}


// -------------------------------------------------------------------------
/*
void AddData1553ToListTransmit(Data1553* data)
{
	// ������� ����� ������ � ������ ��� ������ � ����
	ListTransmit* item = (ListTransmit*) malloc(sizeof(ListTransmit));
	item->data.data1553_obj = data;

	// ���������� ������ ������ ���� (�1/�2)
	unsigned short direction;
	MIL1553_GET_DIRECTION_BIT(data->command_word, &direction);

	// ��������� ID ������
	if (direction == BCtoRT) 		{ item->packet_id = ID_PACKET_IBIVK_TO_PC_F1; }
	else if (direction == RTtoBC) 	{ item->packet_id = ID_PACKET_IBIVK_TO_PC_F2; }
	else
	{
		// �������������� ������
	}

	// �������� ������ � �����
	AddItemPriorityLeast(item);
}

void AddDiagnAnswerToListTransmit(DataDiagnosticAnswer* data)
{
	// ������� ����� ������ � ������ ��� ������ � ����
	ListTransmit* item = (ListTransmit*) malloc(sizeof(ListTransmit));
	item->data.data_diagnostic_answer_obj = data;

	// ��������� ID ������
	item->packet_id = ID_DIAGNOSTIC_ANSWER;

	// �������� ������ � �����
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
			// ��������������� ������
			break;
	}

}


void MakeDiagnosticAnswer(unsigned short cs, unsigned short id, unsigned short code_error)
{
	// ����������� ��������������� ����� � ��������� � ����� ���� �� ������
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

