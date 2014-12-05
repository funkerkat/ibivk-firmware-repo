/*
 * timer.c
 *
 *  Created on: 04.12.2014
 *      Author: Ludmila
 */

#include "xSystem.h"
#include "xMil1553BC.h"
#include "xTimer.h"
#include "list_microseconds.h"

void ExecuteCore1553()
{
	// ����������������� ������ �� ��������� ��������� (���� ��� ������������� � ���� �������)
	if(global_microsecond->timer_value != 0)
	{
		TIMER2_START(global_microsecond->timer_value);
	}

	// ������ ������� ���������:
	// 1. ��������� ���� ���� ������ (���� ��������)
	CORE1553_EXECUTION_DELAY();

	// 2. ������ ��������� ����
	CORE1553_RELOAD(global_microsecond->core1553_entry->reg_leon, global_microsecond->core1553_entry->reg_core1553);
	CORE1553_START_EXECUTION();

	// ����������� ��������� �� ��������� ���������
	global_microsecond = global_microsecond->next;
}
