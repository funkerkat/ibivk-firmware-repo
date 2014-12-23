/*
 * selftest.c
 *
 *  Created on: 07.11.2014
 *      Author: Ludmila
 */

// ������������ ���������� ��
#include <stdlib.h>

// ���������� ��� ������ � ����������
#include "xUart.h"

// ���������� �������� �������
#include "list_transmit_struct.h"
#include "ibivk_uart_packets.h"
#include "tmi_struct.h"

// ��������� �������
#include "tmi.h"
#include "fpga_ibivk.h"

void InitSelftest()
{
	// ��������� ���������������� ����
	unsigned int fpga_selftest_result = InitSelftestFpga();
	if (fpga_selftest_result == EXIT_SUCCESS) 	{ ibivk_tmi.integral_params.norma_ibivk = NORMAL; }
	else 										{ ibivk_tmi.integral_params.norma_ibivk = NOT_NORMAL; }
}

void SendTmi()
{
    // ������ ��������������� ����� � ����:
    // 1. �������� ������ ��� �������� ������ ��� ����������� ������ � ����
    ListTransmit *item = (ListTransmit*)malloc(sizeof (ListTransmit));
    // 2. ����������� ������
    item->packet_id = ID_PACKET_IBIVK_TO_PC_TM;
    item->data.data_tmi.p_tmi = &ibivk_tmi;
    // 3. ��������� �� �������� ������ (�� ����������� ������� ������ � ����)
    AddItemToListTransmit(item);
}

static void SendTmiNotNormal()
{
	UpdateIntegralParams();
	if (ibivk_tmi.integral_params.norma_ibivk == NOT_NORMAL)
	{
	    SendTmi();
	}
}

void Uart1_Selftest(unsigned int error_detected)
{
	// ���� ������
	if (error_detected)
	{
		// �������� ���� ������������� �� �������� ����
		UART1_STATUS_RESET();
		// ������� ����� � ������������� � ����������
		ibivk_tmi.selftest_uart.uart1_error_code = error_detected;
		// ������ ���������� � ��
		SendTmiNotNormal();
	}
}

void Uart2_Selftest(unsigned int error_detected)
{
	// ���� ������
	if (error_detected)
	{
		// �������� ���� ������������� �� �������� ����
		UART2_STATUS_RESET();
		// ������� ����� � ������������� � ����������
		ibivk_tmi.selftest_uart.uart2_error_code = error_detected;
		// ������ ���������� � ��
		SendTmiNotNormal();
	}
}

void Core1553_Selftest(unsigned short core1553_pending)
{
	ibivk_tmi.selftest_core1553.core1553_error_code = core1553_pending;
	SendTmiNotNormal();
}
