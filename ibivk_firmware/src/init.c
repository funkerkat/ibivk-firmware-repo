/*
 * init.c
 *
 *  Created on: 23.12.2014
 *      Author: Ludmila
 */

// ���������� ��� ������ � ����������
#include "xMil1553BC.h"
#include "xIrqmp.h"
#include "xUart.h"
#include "xTimer.h"
#include "xGrgpio.h"
#include "xBshvtransceiver.h"

// ��������� �������
#include "list_bshv.h"
#include "list_transmit.h"
#include "bshv.h"
#include "tmi.h"
#include "interrupts.h"

static void DisableCache()
{
	asm("set 0, %g0");
	asm("lda [%g0]2, %g1");
	asm("and 0xFFFFFFF0, %g1, %g2");
	asm("set 0x400000, %g4");
	asm("or %g4, %g2, %g3");
	asm("sta %g3, [%g0]2");
}

void InitIbivk()
{
	// reset ���� 1553
	CORE1553_SOFTWARE_RESET();

	// �������� �������� ����������
	IRQMP_CLEAR();

	// ���������� ����
	DisableCache();

	// ������������� ������������ ����������
	InitInterruptHandlers();

	// ������������ ���������� �������� ���������� ���
	InitSystemBshv();

	// ������������� ���� 1553
	CORE1553_INIT();

	// ������������� �������
	TIMER_INIT();

	// ������������� UART
	UART_INIT(115200);

	// ������������� GPIO
	GRGPIO_INIT();

	// ������������� ����
	BSHV_TRANSCEIVER_INIT();

	// ������������� �������� ������, ����������� ������� (���������� ��������)
	InitListBshv();

	// ������������� ������� ��������� �� ������ � ����
	InitListTransmit();

	// ������������� ����������
	InitTimerType();

	// ���������������� ��������� �������� ���������� �����
	InitTmi();
}
