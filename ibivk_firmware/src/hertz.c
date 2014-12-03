/*
 * hertz.c
 *
 *  Created on: 03.12.2014
 *      Author: Ludmila
 */

#include "xSystem.h"
#include "bshv.h"
#include "node_bshv.h"


void Execute1553()
{


}

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
			// выдача сообщений МКИО, запланированных на текущую секунду
			Execute1553();
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





	// Поиск по списку, содержащему отсчеты БШВ
	/*
	NodeBshv* this = node_bshv_start;		// указатель на текущий элемент списка
	while(this)
	{
		this = this->next;
	}
*/

}
