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


void SendItemToUart()
{
	// ������� �� ������ ������ �������
	ListTransmit* item = head_list_transmit;
	head_list_transmit = item->next;

	// ������ ����������� ������� � ����
	switch(item->packet_id)
	{
		case ID_PACKET_IBIVK_TO_PC_DIAGNOSTIC_ANSWER:
			DiagnosticAnswer  (item->data.data_diagnostic_answer.receiver_id,
							   item->data.data_diagnostic_answer.receiver_cs,
							   item->data.data_diagnostic_answer.code_error);
			break;


		case ID_PACKET_IBIVK_TO_PC_TM:
			RS485_send_tmi(item->data.data_tmi.p_tmi);
			// �������� ����������
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
			// ��������������� ������
			break;
	}

	// ���������� ������
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
	// ���������� ������ ������� � NULL (�. �. ������� �����������)
	head_list_transmit = NULL;
}

