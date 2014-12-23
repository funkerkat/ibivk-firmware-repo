/*
 * bshv.c
 *
 *  Created on: 22.11.2014
 *      Author: Ludmila
 */

// ���������� ��� ������ � ����������
#include "xSystem.h"

// ���������� �������� �������
#include "bshv_struct.h"

Bshv system_bshv;
unsigned int bshv_correction;

void InitSystemBshv()
{
	// ��������, ������� ���������� ���������� ��� ��������� ����� � ���������� �������� 1 ��
	bshv_prev.fouryears = 0;
	bshv_prev.day = 0;
	bshv_prev.hour = 0;
	bshv_prev.minute = 0;
	bshv_prev.second = 0;

	// ��������� ����� �����
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

void IncrementBshvCorrectionAvailable(Bshv* myBshv)
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
				// ��������������� ������
				break;
		}
	}
}

result_type CompareBshv(Bshv* first_value, Bshv* second_value)
{
	// �������� ������������
	if ( (first_value->fouryears) < (second_value->fouryears) ) { return FirstValueIsLess; }
	if ( (first_value->fouryears) > (second_value->fouryears) ) { return FirstValueIsGreater; }

	// �������� �����
	if ( (first_value->day) < (second_value->day) ) { return FirstValueIsLess; }
	if ( (first_value->day) > (second_value->day) ) { return FirstValueIsGreater; }

	// �������� ���
	if ( (first_value->hour) < (second_value->hour) ) { return FirstValueIsLess; }
	if ( (first_value->hour) > (second_value->hour) ) { return FirstValueIsGreater; }

	// �������� ������
	if ( (first_value->minute) < (second_value->minute) ) { return FirstValueIsLess; }
	if ( (first_value->minute) > (second_value->minute) ) { return FirstValueIsGreater; }

	// �������� �������
	if ( (first_value->second) < (second_value->second) ) { return FirstValueIsLess; }
	if ( (first_value->second) > (second_value->second) ) { return FirstValueIsGreater; }

	// ���� �� ��������� ����� �� ������� �� ������ �� ������� ����, ��� �������� �����:
	return Equal;
}
