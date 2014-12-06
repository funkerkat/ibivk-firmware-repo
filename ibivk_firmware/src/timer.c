/*
 * timer.c
 *
 *  Created on: 04.12.2014
 *      Author: Ludmila
 */

#include <stdlib.h>
#include "xSystem.h"
#include "xMil1553BC.h"
#include "xTimer.h"
//#include "list_microseconds.h"
#include "rx_queue.h"
#include "mil1553.h"

#include "nodes.h"

// ���������:
void RemoveItemFromListMicrosecond(NodeMicrosecond* this_item);


void ExecuteCore1553()
{
	// ����������������� ������ �� ��������� ��������� (���� ��� ������������� � ���� �������)
	if(global_microsecond->timer_value != 0) { TIMER2_START(global_microsecond->timer_value); }

	// ������ ��������� ����
	CORE1553_RELOAD(global_microsecond->core1553_entry->reg_leon, global_microsecond->core1553_entry->reg_core1553);
	CORE1553_START_EXECUTION();

	// ������� ������ � ��������� (��� ����������� ������ � UART � ������ �2 ��� ��������� �������� � �� ����)
	Bshv tempBshv;
	tempBshv.fouryears = 1;
	tempBshv.day = 2;
	tempBshv.hour = 3;
	tempBshv.minute = 4;
	tempBshv.second = 5;

	BshvExtention thisBshv;
	thisBshv.myBshv = tempBshv;
	thisBshv.microsecond = global_microsecond->microsecond;

	// �������� ��������� ������ ���� 1553 (��� ���������� ��������� �����)
	CORE1553_EXECUTION_DELAY();

	// ������� � ���������������� �������� ����� �� ������� ��������� �������� (����������� ������� 5 �������� - ����� ��)
	unsigned short sw = global_microsecond->core1553_entry->cmd_block_1.StatusWord1;
	unsigned short error_bits = sw & 0x7FF;

	// ���������������� ������ ��������� (�1 ��� �2)
	unsigned short cw = global_microsecond->core1553_entry->cmd_block_1.CommandWord1;
	unsigned int direction_bit;
	MIL1553_GET_DIRECTION_BIT(cw, &direction_bit);

	// ��������� ���������� ������ ��� ������ ����
	if( (error_bits != 0) || (direction_bit == RTtoBC) )
	{
		// �������� ������ ��� �������� ������ ���� ��� ����������� ������ � ����
		Data1553* thisData1553 = (Data1553*) malloc(sizeof(Data1553));
		// ����������� ������ �� �������� ������ � ��������� �������� ������
		thisData1553->myBshvExtenion.myBshv.fouryears 	= global_microsecond->base_node_bshv->myBshv.fouryears;
		thisData1553->myBshvExtenion.myBshv.day 		= global_microsecond->base_node_bshv->myBshv.day;
		thisData1553->myBshvExtenion.myBshv.hour 		= global_microsecond->base_node_bshv->myBshv.hour;
		thisData1553->myBshvExtenion.myBshv.minute 		= global_microsecond->base_node_bshv->myBshv.minute;
		thisData1553->myBshvExtenion.myBshv.second 		= global_microsecond->base_node_bshv->myBshv.second;
		thisData1553->myBshvExtenion.microsecond		= global_microsecond->microsecond;
		thisData1553->command_word = cw;
		thisData1553->status_word = sw;
		int i; for (i=0; i<32; i++) { thisData1553->data_words[i] = global_microsecond->core1553_entry->data_words[i]; }

		// ���������� �������, �� ������� ������� ��������� ������: ��������� ������� � �� ���� ��� �2
		 if (error_bits != 0)
		 {
			 thisData1553->isNormal = FALSE;
			 UartTransmitQueueStore(thisData1553);
		 }

		 if (direction_bit == RTtoBC)
		 {
			 thisData1553->isNormal = TRUE;
			 UartTransmitQueueStore(thisData1553);
		 }
	}

	// ����������� ��������� �� ��������� ��������� � ������� ������� ������ �� ������ �����������
	NodeMicrosecond* nextNodeMicrosecond = global_microsecond->next;
	RemoveItemFromListMicrosecond(global_microsecond);
	global_microsecond = nextNodeMicrosecond;
}
