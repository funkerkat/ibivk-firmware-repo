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

// прототипы функций


Bshv bshv_prev;

void TestFpga2()
{
	FPGA_IBIVK_SOFTWARE_RESET();
	int tm = *((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_IBSU_TM));
	// *((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_IBSU_COM)) = IBSU_COM;
}

unsigned int InitSelftestFpga()
{
	unsigned int write_value = 0x12345678;
	*((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_TEST)) = write_value;

	unsigned int read_value;
	read_value = *((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_TEST));

	if (write_value == read_value) { return EXIT_SUCCESS; }
	else { return EXIT_FAILURE; }
}

void ReadBshvFromFPGA()
{
	int x = *((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_IBSU_READ));

	bshv_prev.second 		= (x >> 0) 			& 0x3F;
	bshv_prev.minute 		= (x >> 6) 			& 0x3F;
	bshv_prev.hour 			= (x >> (6+6)) 		& 0x1F;
	bshv_prev.day 			= (x >> (6+6+5)) 	& 0x7FF;
	bshv_prev.fouryears 	= (x >> (6+6+5+11)) & 0xF;


	(bshv_prev.day)++;
	(bshv_prev.fouryears)++;

}
