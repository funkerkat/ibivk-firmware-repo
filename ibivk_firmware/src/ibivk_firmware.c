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

void InitInterruptHandlers(void);
void LinkedListDemo(void);
void InitSystemBshv(void);
void UartSettings(void);

int main(void)
{

	InitInterruptHandlers();
	LinkedListDemo();
	InitSystemBshv();
	//InitUart();
//	UartSettings();

	int i = 0;
	while(1)
	{
		;
		Test1553Core();
		//Mil1553BC_Start(i);
	}

	return EXIT_SUCCESS;
}
