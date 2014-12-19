/*
 * timer.c
 *
 *  Created on: 04.12.2014
 *      Author: Ludmila
 */

// ������������ ���������� ��
#include <stdlib.h>

// ���������� ��� ������ � ����������
#include "xSystem.h"
#include "xMil1553BC.h"
#include "xTimer.h"

// ���������� �������� �������
#include "list_transmit_struct.h"
#include "ibivk_uart_packets.h"
#include "mil1553.h"
#include "nodes.h"

// ��������� �������
#include "list_microseconds.h"
#include "list_transmit.h"

void ExecuteCore1553()
{
	// ����������������� ������ �� ��������� ��������� (���� ��� ������������� � ���� �������)
	if(global_microsecond->timer_value != 0) { TIMER2_START(global_microsecond->timer_value); }

	// ������ ��������� ����
	CORE1553_RELOAD(global_microsecond->core1553_entry->reg_leon, global_microsecond->core1553_entry->reg_core1553);
	CORE1553_START_EXECUTION();

	// ���� ������ ������ ���������, ������ ������� ������ � ��������� (��� ����������� ������ � UART ��������� ���������)

	// 1. �������� ������ ��� �������� ������ ��� ����������� ������ � ����
	ListTransmit* item = (ListTransmit*) malloc(sizeof(ListTransmit));

	// 2. ����������� ������ ��� (��� �������, � ��������� data_ibivk_to_pc_f1 ��� data_ibivk_to_pc_f2, �. �. union)
	item->data.data_ibivk_to_pc_f2.myBshvExtenion.myBshv.fouryears = global_microsecond->base_node_bshv->myBshv.fouryears;
	item->data.data_ibivk_to_pc_f2.myBshvExtenion.myBshv.day 		= global_microsecond->base_node_bshv->myBshv.day;
	item->data.data_ibivk_to_pc_f2.myBshvExtenion.myBshv.hour 		= global_microsecond->base_node_bshv->myBshv.hour;
	item->data.data_ibivk_to_pc_f2.myBshvExtenion.myBshv.minute 	= global_microsecond->base_node_bshv->myBshv.minute;
	item->data.data_ibivk_to_pc_f2.myBshvExtenion.myBshv.second 	= global_microsecond->base_node_bshv->myBshv.second;
	item->data.data_ibivk_to_pc_f2.myBshvExtenion.microsecond		= global_microsecond->microsecond;

	// 3. ����������� ��������� ����� (��� �������, � ��������� data_ibivk_to_pc_f1 ��� data_ibivk_to_pc_f2, �. �. union)
	unsigned short cw = global_microsecond->core1553_entry->cmd_block_1.CommandWord1;
	item->data.data_ibivk_to_pc_f2.command_word = cw;

	// 4. ���������������� ������ ��������� (�1 ��� �2)
	unsigned int direction_bit;
	MIL1553_GET_DIRECTION_BIT(cw, &direction_bit);

	// �������� ��������� ������ ���� 1553 (��� ���������� ��������� �����)
	CORE1553_EXECUTION_DELAY();

	// ������� � ���������������� �������� ����� �� ������� ��������� �������� (����������� ������� 5 �������� - ����� ��)
	unsigned short sw = global_microsecond->core1553_entry->cmd_block_1.StatusWord1;

	// ��������� ������ ��� ������ ����:
	 if (direction_bit == BCtoRT)
	 {
		 // ���������� ��� ������
		 item->packet_id = ID_PACKET_IBIVK_TO_PC_F1;
		 // ����������� �������� �����
		 item->data.data_ibivk_to_pc_f1.status_word = sw;
	 }
	 else if(direction_bit == RTtoBC)
	 {
		 // ���������� ��� ������
		 item->packet_id = ID_PACKET_IBIVK_TO_PC_F2;
		 // ����������� �������� �����
		 item->data.data_ibivk_to_pc_f2.status_word = sw;
		 // ����������� ����� ������
		 int i; for (i=0; i<32; i++) { item->data.data_ibivk_to_pc_f2.data_words[i] = global_microsecond->core1553_entry->data_words[i]; }
	 }
	 else
	 {
		 // ��������������� ������
	 }

	 // 3. ��������� �� �������� ������ (�� ����������� ������� ������ � ����)
	 AddItemToListTransmit(item);

	// ����������� ��������� �� ��������� ��������� � ������� ������� ������ �� ������ �����������
	NodeMicrosecond* nextNodeMicrosecond = global_microsecond->next;
	RemoveItemFromListMicrosecond(global_microsecond);
	global_microsecond = nextNodeMicrosecond;
}
