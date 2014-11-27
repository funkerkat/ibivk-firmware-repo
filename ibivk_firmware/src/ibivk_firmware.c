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
	UartSettings();

	while(1)
	{
		;
	}

	return EXIT_SUCCESS;
}
