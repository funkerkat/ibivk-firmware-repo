/*
 * put_into_linkedlist.c
 *
 *  Created on: 22.11.2014
 *      Author: Ludmila
 */

// ������������ ���������� ��
#include <stdlib.h>

// ���������� ��� ������ � ����������
#include "xSystem.h"
#include "xMil1553BC.h"

// ���������� �������� �������
#include "bus_controller_struct.h"
#include "bshv_struct.h"
#include "nodes.h"

// ��������� �������
#include "list_microseconds.h"

int LoadPacketF1(BshvExtention* b, unsigned int cw, unsigned short dw[])
{
	HertzHandler();
	/*
	// �������� ������ � ���� ��� ������ ����
	EntryCore1553* p_entry = (EntryCore1553*) malloc(sizeof(EntryCore1553));
	Load1553MessageF1(p_entry, cw, dw);

	// ����� ��������������� ������ ��� ��� �������e �����
	NodeBshv* thisNodeBshv = AddNodeBshvItem(&node_bshv_start, &(b->myBshv));

	// ���� �� ���������� ������ �����������, ������������ � �������� �������� ���, �� ������� ���:
	if (thisNodeBshv->ptr == NULL) { CreateListMicrosecond(&(thisNodeBshv->ptr)); }

	// �������� ������� ������������ � ������ �����������
	AddNodeMicrosecondItem(&(thisNodeBshv->ptr), p_entry, b->microsecond, thisNodeBshv);
*/
	return EXIT_SUCCESS;
}



int LoadPacketF2(BshvExtention* b, unsigned int cw)
{
	//HertzHandler();

	// �������� ������ � ���� ��� ������ ����
	EntryCore1553* p_entry = (EntryCore1553*) malloc(sizeof(EntryCore1553));
	Load1553MessageF2(p_entry, cw);

	// ����� ��������������� ������ ��� ��� �������e �����
	NodeBshv* thisNodeBshv = AddNodeBshvItem(&node_bshv_start, &(b->myBshv));

	// ���� �� ���������� ������ �����������, ������������ � �������� �������� ���, �� ������� ���:
	if (thisNodeBshv->ptr == NULL) { CreateListMicrosecond(&(thisNodeBshv->ptr)); }

	// �������� ������� ������������ � ������ �����������
	AddNodeMicrosecondItem(&(thisNodeBshv->ptr), p_entry, b->microsecond, thisNodeBshv);

	return EXIT_SUCCESS;
}

