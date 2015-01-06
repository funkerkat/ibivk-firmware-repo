/*
 * bshv.c
 *
 *  Created on: 22.11.2014
 *      Author: Ludmila
 */

// библиотеки для работы с периферией
#include "xSystem.h"

// библиотеки текущего проекта
#include "bshv_struct.h"

Bshv system_bshv;
unsigned int bshv_correction;

void InitSystemBshv()
{
	// значение, которое необходимо установить как системное время с ближайшего импульса 1 Гц
	predict_bshv.fouryears = 0;
	predict_bshv.day = 0;
	predict_bshv.hour = 0;
	predict_bshv.minute = 0;
	predict_bshv.second = 0;

	// системное время ИБИВК
	system_bshv.fouryears = 1;
	system_bshv.day = 15;
	system_bshv.hour = 23;
	system_bshv.minute = 30;
	system_bshv.second = 15;

/*
	system_bshv.fouryears = 15;
	system_bshv.day = 1461;
	system_bshv.hour = 23;
	system_bshv.minute = 59;
	system_bshv.second = 59;
*/
	bshv_correction = CORRECTIOM_DISABLED;
}

void CopyBshv(Bshv* copyFrom, Bshv* copyTo)
{
	copyTo->fouryears 	= copyFrom->fouryears;
	copyTo->day 		= copyFrom->day;
	copyTo->hour 		= copyFrom->hour;
	copyTo->minute 		= copyFrom->minute;
	copyTo->second 		= copyFrom->second;
}

void IncrementBshv(Bshv* myBshv, unsigned int second_value)
{
	if (myBshv->second < second_value) 						{ (myBshv->second)++; return; }
	else 													{ (myBshv->second) = 0; }

	if (myBshv->minute < BSHV_MINUTE_UPPER_BOUNDARY) 		{ (myBshv->minute)++; return; }
	else 													{ (myBshv->minute) = 0; }

	if (myBshv->hour < BSHV_HOUR_UPPER_BOUNDARY) 			{ (myBshv->hour)++; return; }
	else 													{ (myBshv->hour) = 0; }

	if (myBshv->day < BSHV_DAY_UPPER_BOUNDARY) 				{ (myBshv->day)++; return; }
	else 													{ (myBshv->day) = 0; }

	if (myBshv->fouryears < BSHV_FOURYEARS_UPPER_BOUNDARY) 	{ (myBshv->fouryears)++; return; }
	else 													{ (myBshv->second) = 0; (myBshv->minute) = 0; (myBshv->hour) = 0; (myBshv->day) = 0; (myBshv->fouryears) = 0; }
}

unsigned int ValidateBshvBoundaries(Bshv* myBshv)
{
	// Проверка значений БШВ на диапазон
	if ((myBshv->second 		< BSHV_SECOND_LOWER_BOUNDARY) 	||
		(myBshv->second 		> BSHV_SECOND_UPPER_BOUNDARY)) 			{ return EXIT_FAILURE;}

	if ((myBshv->minute 		< BSHV_MINUTE_LOWER_BOUNDARY) 	||
		(myBshv->minute 		> BSHV_MINUTE_UPPER_BOUNDARY)) 			{ return EXIT_FAILURE;}

	if ((myBshv->hour 			< BSHV_HOUR_LOWER_BOUNDARY) 	||
		(myBshv->hour 			> BSHV_HOUR_UPPER_BOUNDARY)) 			{ return EXIT_FAILURE;}

	if ((myBshv->day 			< BSHV_DAY_LOWER_BOUNDARY) 		||
		(myBshv->day 			> BSHV_DAY_UPPER_BOUNDARY)) 			{ return EXIT_FAILURE;}

	if ((myBshv->fouryears 		< BSHV_FOURYEARS_LOWER_BOUNDARY)||
		(myBshv->fouryears 		> BSHV_FOURYEARS_UPPER_BOUNDARY)) 		{ return EXIT_FAILURE;}

	return EXIT_SUCCESS;
}

void PredictNextBshvValue(Bshv* myBshv)
{
	if ((bshv_correction == CORRECTIOM_DISABLED) || (myBshv->hour != CORRECTION_HOUR) || (myBshv->minute != CORRECTION_MINUTE))
	{
		IncrementBshv(myBshv, 59);
	}
	else
	{
		switch(bshv_correction)
		{
			case CORRECTIOM_PLUS_1S:
				IncrementBshv(myBshv, 60);
				break;

			case CORRECTIOM_MINUS_1S:
				IncrementBshv(myBshv, 58);
				break;

			default:
				// алгоритмическая ошибка
				break;
		}
	}
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
