/*
 * uart_tx.c
 *
 *  Created on: 27.11.2014
 *      Author: Ludmila
 */

#include "xSystem.h"
#include "xUart.h"
#include "ibivk_uart_packets.h"
#include "control_sum.h"
#include "selftest.h"

static void AddServiceBytes(unsigned int data[], unsigned int info_bytes_size, unsigned int packet_id)
{
	// ��������� ����-�� ���� (��������� � ��������������) � ������
	unsigned int n = SERVICE_BYTES_SIZE + info_bytes_size;

	// �������� ��������� ������
	data[0] = HEAD_0;
	data[1] = HEAD_1;
	data[2] = HEAD_2;
	data[3] = HEAD_3;

	// �������� ����� ������ N (����)
	data[4] = info_bytes_size;

	// �������� ID ������
	data[5] = packet_id;

	// ���������� � �������� ����������� ����� ������ (��������� ����)
	data[n-1] = CountControlSum(data, n-1);

	// ������ � UART
	int error=0;
	int i;
	for (i=0; i<n; i++)
	{
		UART2_SEND_BYTE_POLLING(data[i], &error);
		if (error) { Uart2_ErrorDetected(error); }
	}
}


void DiagnosticAnswer(unsigned int received_cs, unsigned int received_id, unsigned int error_code)
{
	unsigned int data[DIAGNOSTIC_ANSWER_TOTAL_LENGTH];

	// ��������� �������������� �����
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 0] = received_cs;
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 1] = received_id;
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 2] = error_code;

	// �������� ��������� �����
	AddServiceBytes(data, DIAGNOSTIC_ANSWER_TOTAL_LENGTH-SERVICE_BYTES_SIZE, ID_DIAGNOSTIC_ANSWER);

}
