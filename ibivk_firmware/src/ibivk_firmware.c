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

#include "node.h"
#include "timer.h"
#include "irqmp.h"
#include "grgpio.h"

void InitIrqmp(void);
void InitGrgpio(void);
void InitTimer(unsigned int);
void TimerStart(unsigned int);
void AddItem(Node**, int);
int CountItems(Node**);
void Mil1553BC_Start(int);
void InitInterruptHandlers(void);
void InitUart(void);
void GpioToggle(int);


void handler_1Hz(int irq)
{
	int t1 = 1;
	t1++;
}


int main_mko(void)
{

	while(1)
	{
		Mil1553BC_Start(2);
	}

	return EXIT_SUCCESS;
}



#define DELTA 12


int main_toggle(void)
{
	GRGPIO_INIT();

	while(1)
	{
		GRGPIO_TOGGLE(3);
	}

}

int main(void)
{

	TIMER_INIT();

    InitInterruptHandlers();

    GRGPIO_INIT();
	InitIrqmp();
	InitUart();

//	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_FORCE)) =  1 << 12;


	TIMER1_START(150 - DELTA);
	GRGPIO_TOGGLE(0);

//	TIMER2_START(20 - DELTA);
//	GpioToggle(1);
//
	TIMER2_START(75 - DELTA);
	GRGPIO_TOGGLE(1);

	while(1)
	{
		GRGPIO_TOGGLE(2);
	}


	return EXIT_SUCCESS;

}


int main_timer(void)
{

    InitInterruptHandlers();

    GRGPIO_INIT();
	InitIrqmp();


	while(1)
	{
		;
	}

	return EXIT_SUCCESS;

}


int main_testlinkedlist(void) {
	//puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	int* adr;
	adr = (int*)(0x80000200 + 0x40);
	*adr = 0xffffffff;

	adr = (int*)(0x80000200 + 0x0c);
	*adr = 1 << 2;


	Node* start = NULL;
	Node** p_start = &start;

	AddItem(p_start, 0x41);
	AddItem(p_start, 0x10);
	AddItem(p_start, 0x5);
	AddItem(p_start, 0x100);
	AddItem(p_start, 0x2);
	AddItem(p_start, 0x43);
	AddItem(p_start, 0x99);
	AddItem(p_start, 0x200);

	int n = CountItems(p_start);
	n++;

	while(1)
	{
		;
	}

	return EXIT_SUCCESS;
}
