/*
 * fpga_ibivk.c
 *
 *  Created on: 06.12.2014
 *      Author: Ludmila
 */

#include "xFpgaIbivk.h"
#include "Bshv_struct.h"

Bshv bshv_prev;

void TestFpga()
{
	FPGA_IBIVK_SOFTWARE_RESET();
	int tm = *((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_IBSU_TM));
	// *((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_IBSU_COM)) = IBSU_COM;
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
