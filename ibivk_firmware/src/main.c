/*
 ============================================================================
 Name        : ibivk_firmware.c
 Author      : Ludmila
 Version     : 1
 Copyright   : 
 Description : IBIVK project in C, Ansi-style
 ============================================================================
 */

// ���������� ��� ������ � ����������
#include "xSystem.h"
#include "xIrqmp.h"

// ���������� �������� �������
#include "list_transmit_struct.h"

// ��������� �������
#include "list_transmit.h"
#include "init.h"
#include "selftest.h"

int main(void)
{
	// ������������� ����������������
	InitIbivk();

	// ��������� ����������������
	InitSelftest();

	// ���������� ����������
	IRQMP_ENABLE();

	while(1)
	{
		// ���� � ������� �� ������ � ���� ���� ��������, ����������� ������
		if (head_list_transmit != NULL) { SendItemToUart(); }
	}

	return EXIT_SUCCESS;
}



/*
DisableCache();
CORE1553_SOFTWARE_RESET();
IRQMP_CLEAR();
InitInterruptHandlers();
// ------------------------
InitSystemBshv();		// ?

CORE1553_INIT();
TIMER_INIT();
UART_INIT(115200);
GRGPIO_INIT();
IRQMP_ENABLE();


InitListBshv();

// ������������� ������� ��������� �� ������ � ����
InitListTransmit();

// ���������������� ��������� �������� ���������� �����
InitTmi();

//TestFpga();
*/
