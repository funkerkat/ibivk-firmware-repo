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
	// Запрограммировать таймер на следующее сообщение (если оно запланировано в этой секунде)
	if(global_microsecond->timer_value != 0)
	{
		TIMER2_START(global_microsecond->timer_value);
	}

	// Выдать текущее сообщение:
	// 1. подождать если ядро занято (идет передача)
	CORE1553_EXECUTION_DELAY();

	// 2. выдать сообщение МКИО
	CORE1553_RELOAD(global_microsecond->core1553_entry->reg_leon, global_microsecond->core1553_entry->reg_core1553);
	CORE1553_START_EXECUTION();

	// Переключить указатель на следующее сообщение
	global_microsecond = global_microsecond->next;
}
