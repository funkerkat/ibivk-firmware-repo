/*
 * bshv.c
 *
 *  Created on: 22.11.2014
 *      Author: Ludmila
 */

#include "xSystem.h"
#include "bshv_struct.h"

Bshv system_bshv;

void InitSystemBshv()
{
	system_bshv.fouryears = 1;
	system_bshv.day = 15;
	system_bshv.hour = 23;
	system_bshv.minute = 30;
	system_bshv.second = 15;
}

result_type CompareBshv(Bshv* first_value, Bshv* second_value)
{
	// Сравнить четырехлетия
	if ( (first_value->fouryears) < (second_value->fouryears) ) { return FirstValueIsLess; }
	if ( (first_value->fouryears) > (second_value->fouryears) ) { return FirstValueIsGreater; }

	// Сравнить сутки
	if ( (first_value->day) < (second_value->day) ) { return FirstValueIsLess; }
	if ( (first_value->day) > (second_value->day) ) { return FirstValueIsGreater; }

	// Сравнить час
	if ( (first_value->hour) < (second_value->hour) ) { return FirstValueIsLess; }
	if ( (first_value->hour) > (second_value->hour) ) { return FirstValueIsGreater; }

	// Сравнить минуту
	if ( (first_value->minute) < (second_value->minute) ) { return FirstValueIsLess; }
	if ( (first_value->minute) > (second_value->minute) ) { return FirstValueIsGreater; }

	// Сравнить секунду
	if ( (first_value->second) < (second_value->second) ) { return FirstValueIsLess; }
	if ( (first_value->second) > (second_value->second) ) { return FirstValueIsGreater; }

	// Если не произошел выход из функции по любому из условий выше, оба значения равны:
	return Equal;
}
/*
int CompareWithSystemBshv(Bshv* system_bshv, Bshv* local_bshv)
{

	return EXIT_SUCCESS;

	if (local_bshv->fouryears < system_bshv->fouryears)
	{
		return EXIT_SUCCESS;
		//return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}
*/

