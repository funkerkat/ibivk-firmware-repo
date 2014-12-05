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

	// пропустить сторожевой элемент в списке микросекунд
	this_microsecond = this_microsecond->next;

	while(this_microsecond->next != NULL)
	{
		// подождать если ядро занято (идет передача)
		CORE1553_EXECUTION_DELAY();

		// выдать сообщение МКИО
		CORE1553_RELOAD(this_microsecond->core1553_entry->reg_leon, this_microsecond->core1553_entry->reg_core1553);
		CORE1553_START_EXECUTION();

		// перейти к следующему элементу в списке микросекунд
		this_microsecond = this_microsecond->next;
	}

}
*/

void HertzHandler()
{
	static int needToClear = FALSE;

	NodeBshv* this = node_bshv_start;	// вхождение в список отсчетов БШВ
	if (this==NULL){ return; }			// аварийный выход
	this = this->next;					// пропустить первый (сторожевой) элемент списка

	while(this)
	{
		result_type res = CompareBshv(&(this->myBshv), &system_bshv);

		if (res == Equal)
		{
			// Программирование таймера на выдачу первого сообщения МКИО в текущей секунде БШВ
			global_microsecond = this->ptr;
			TIMER2_START(global_microsecond->timer_value);
			// Переключить указатель на следующее сообщение
			global_microsecond = global_microsecond->next;
			break;
		}
		else if(res == FirstValueIsLess)
		{
			// прибор содержит устаревшие данные, произвести очистку после выдачи сообщений
			needToClear = TRUE;
			// продолжить поиск актуального значения в списке
			this = this->next;
		}
		else if(res == FirstValueIsGreater)
		{
			// на текущую секунду не запланирована выдача сообщений
			// ожидание следующей секунды
			break;
		}
		else
		{
			return; // аварийный выход
		}

	}

	// Очистка устаревших данных (аномальная ситуация, возможна при переводе системного времени)
	if (needToClear == TRUE)
	{
		// очистить устаревшие значения
	}



}
