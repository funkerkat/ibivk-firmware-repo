/*
 * fpga_ibivk.c
 *
 *  Created on: 06.12.2014
 *      Author: Ludmila
 */

// библиотеки для работы с периферией
#include "xSystem.h"
#include "xFpgaIbivk.h"

// библиотеки текущего проекта
#include "Bshv_struct.h"
#include "tmi_struct.h"

// прототипы функций
#include "selftest.h"

Bshv bshv_prev;

unsigned int InitSelftestFpga()
{
	unsigned int write_value = 0x12345678;
	*((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_TEST)) = write_value;

	unsigned int read_value;
	read_value = *((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_TEST));

	if (write_value == read_value) { return EXIT_SUCCESS; }
	else { return EXIT_FAILURE; }
}

void GetVersionFpga(VersionPmo* ver)
{
	unsigned int x = *((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_IBSU_TM));
	/*
	ver->pmo_year 		= (x >> (8*3)) & 0xFF;
	ver->pmo_month 		= (x >> (8*2)) & 0xFF;
	ver->pmo_day 		= (x >> (8*1)) & 0xFF;
	ver->pmo_revision 	= (x >> (8*0)) & 0xFF;
	*/

	ver->pmo_year 		= (x >> 25) & 0x7F;
	ver->pmo_month 		= (x >> 21) & 0xF;
	ver->pmo_day 		= (x >> 16) & 0x1F;
	ver->pmo_revision 	= 1;
}

void ReadBshvFromFPGA()
{
	// Чтение значения БШВ из ПЛИС
	int x = *((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_IBSU_READ));

	// Декодирование значений БШВ
	bshv_prev.second 		= (x >> 0) 			& 0x3F;
	bshv_prev.minute 		= (x >> 6) 			& 0x3F;
	bshv_prev.hour 			= (x >> (6+6)) 		& 0x1F;
	bshv_prev.day 			= (x >> (6+6+5)) 	& 0x7FF;
	bshv_prev.fouryears 	= (x >> (6+6+5+11)) & 0xF;

	// заплатка до рефакторинга программы ПК
	(bshv_prev.day)++;
	(bshv_prev.fouryears)++;

	// Проверка значений на диапазон
	if ((bshv_prev.second 		< BSHV_SECOND_LOWER_BOUNDARY) 	||
		(bshv_prev.second 		> BSHV_SECOND_UPPER_BOUNDARY)) 			{ SetBshvRangeError(); return;}

	if ((bshv_prev.minute 		< BSHV_MINUTE_LOWER_BOUNDARY) 	||
		(bshv_prev.minute 		> BSHV_MINUTE_UPPER_BOUNDARY)) 			{ SetBshvRangeError(); return;}

	if ((bshv_prev.hour 		< BSHV_HOUR_LOWER_BOUNDARY) 	||
		(bshv_prev.hour 		> BSHV_HOUR_UPPER_BOUNDARY)) 			{ SetBshvRangeError(); return;}

	if ((bshv_prev.day 			< BSHV_DAY_LOWER_BOUNDARY) 		||
		(bshv_prev.day 			> BSHV_DAY_UPPER_BOUNDARY)) 			{ SetBshvRangeError(); return;}

	if ((bshv_prev.fouryears 	< BSHV_FOURYEARS_LOWER_BOUNDARY)||
		(bshv_prev.fouryears 	> BSHV_FOURYEARS_UPPER_BOUNDARY)) 		{ SetBshvRangeError(); return;}

}
