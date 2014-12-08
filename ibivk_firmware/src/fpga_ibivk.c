/*
 * fpga_ibivk.c
 *
 *  Created on: 06.12.2014
 *      Author: Ludmila
 */

#include "xFpgaIbivk.h"
#include "Bshv_struct.h"

void TestFpga()
{
	FPGA_IBIVK_SOFTWARE_RESET();
	int tm = *((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_IBSU_TM));
	// *((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_IBSU_COM)) = IBSU_COM;
}


void ReadSystemBshv()
{
	int x = *((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_IBSU_READ));

	system_bshv.second 		= (x >> 0) 			& 0x3F;
	system_bshv.minute 		= (x >> 6) 			& 0x3F;
	system_bshv.hour 		= (x >> (6+6)) 		& 0x1F;
	system_bshv.day 		= (x >> (6+6+5)) 	& 0x7FF;
	system_bshv.fouryears 	= (x >> (6+6+5+11)) & 0xF;


	int t=0;
	t++;

}
