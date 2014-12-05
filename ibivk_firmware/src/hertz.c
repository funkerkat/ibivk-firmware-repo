/*
 * hertz.c
 *
 *  Created on: 03.12.2014
 *      Author: Ludmila
 */

#include "xSystem.h"
#include "xMil1553BC.h"
#include "xTimer.h"
#include "bshv.h"
#include "node_bshv.h"
#include "list_microseconds.h"

/*
static void Execute1553(NodeBshv* this_bshv)
{
	NodeMicrosecond* this_microsecond = this_bshv->ptr;

	// ���������� ���������� ������� � ������ �����������
	this_microsecond = this_microsecond->next;

	while(this_microsecond->next != NULL)
	{
		// ��������� ���� ���� ������ (���� ��������)
		CORE1553_EXECUTION_DELAY();

		// ������ ��������� ����
		CORE1553_RELOAD(this_microsecond->core1553_entry->reg_leon, this_microsecond->core1553_entry->reg_core1553);
		CORE1553_START_EXECUTION();

		// ������� � ���������� �������� � ������ �����������
		this_microsecond = this_microsecond->next;
	}

}
*/

void HertzHandler()
{
	static int needToClear = FALSE;

	NodeBshv* this = node_bshv_start;	// ��������� � ������ �������� ���
	if (this==NULL){ return; }			// ��������� �����
	this = this->next;					// ���������� ������ (����������) ������� ������

	while(this)
	{
		result_type res = CompareBshv(&(this->myBshv), &system_bshv);

		if (res == Equal)
		{
			// ���������������� ������� �� ������ ������� ��������� ���� � ������� ������� ���
			global_microsecond = this->ptr;
			TIMER2_START(global_microsecond->timer_value);
			// ����������� ��������� �� ��������� ���������
			global_microsecond = global_microsecond->next;
			break;
		}
		else if(res == FirstValueIsLess)
		{
			// ������ �������� ���������� ������, ���������� ������� ����� ������ ���������
			needToClear = TRUE;
			// ���������� ����� ����������� �������� � ������
			this = this->next;
		}
		else if(res == FirstValueIsGreater)
		{
			// �� ������� ������� �� ������������� ������ ���������
			// �������� ��������� �������
			break;
		}
		else
		{
			return; // ��������� �����
		}

	}

	// ������� ���������� ������ (���������� ��������, �������� ��� �������� ���������� �������)
	if (needToClear == TRUE)
	{
		// �������� ���������� ��������
	}



}
