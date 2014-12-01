/*
 ============================================================================
 Name        : ibivk_firmware.c
 Author      : Ludmila
 Version     : 1
 Copyright   : 
 Description : IBIVK project in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "xIrqmp.h"
#include "xMil1553BC.h"

void InitInterruptHandlers(void);
void LinkedListDemo(void);
void InitSystemBshv(void);
void UartSettings(void);




extern core1553_irqs_cnt;
extern unsigned int core1553_irqs[32];
int main(void)
{
	CORE1553_SOFTWARE_RESET();
	IRQMP_CLEAR();
	InitInterruptHandlers();
	// ------------------------

	LinkedListDemo();
	InitSystemBshv();
	//InitUart();
//	UartSettings();
	// ------------------------

	TestRam();
	core1553_irqs_cnt = 0;
	LoadCommandBlockAndData();
	CORE1553_INIT();


	IRQMP_ENABLE();

	int k = 1000; while(k) { k--; }

	int i = 0;
	while(1)
	{
		;
		Test1553Core();
		//Mil1553BC_Start(i);


		/*
		CleanLogList(core1553_irqs, 32);

		*((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG08_COMMAND_BLOCK_POINTER)) 	= 0x80;
		START_EXECUTION();

		int k = 1000000; while(k) { k--; }

		int t = 1;
		t++;

		*/

	}




	return EXIT_SUCCESS;
}
