/*
 * uart_decode_packets.c
 *
 *  Created on: 22.11.2014
 *      Author: Ludmila
 */
#include "xSystem.h"
#include "shift_reg.h"
#include "ibivk_uart_packets.h"
#include "bshv.h"
#include "mil1553.h"

static void GetBshvFromPacket(Byte** p_adr_data, Bshv* packet_bshv)
{
	// ������������ ����� ������������
	packet_bshv->fouryears = (*p_adr_data)->value;
	(*p_adr_data) = (*p_adr_data)->next;

	// ������������ ����� �����
	unsigned int temp1, temp2;
	temp1 = (*p_adr_data)->value;
	(*p_adr_data) = (*p_adr_data)->next;

	temp2 = (*p_adr_data)->value;
	(*p_adr_data) = (*p_adr_data)->next;

	packet_bshv->day = (temp1 << 8) | (temp2 << 0);

	// ������������ ����� ����
	packet_bshv->hour = (*p_adr_data)->value;
	(*p_adr_data) = (*p_adr_data)->next;

	// ������������ ����� ������
	packet_bshv->minute = (*p_adr_data)->value;
	(*p_adr_data) = (*p_adr_data)->next;

	// ������������ ����� �������
	packet_bshv->second = (*p_adr_data)->value;
	(*p_adr_data) = (*p_adr_data)->next;

	// ������������ ����� ������������
	unsigned int t1, t2, t3;

	t1 = (*p_adr_data)->value;
	(*p_adr_data) = (*p_adr_data)->next;

	t2 = (*p_adr_data)->value;
	(*p_adr_data) = (*p_adr_data)->next;

	t3 = (*p_adr_data)->value;
	(*p_adr_data) = (*p_adr_data)->next;

	packet_bshv->microsecond = ((t1 << 8*2)|(t2 << 8*1)|(t3 << 8*0));

}

static int ValidateBshvBoundaries(Bshv* b)
{
	if ((b->fouryears < BSHV_FOURYEARS_LOWER_BOUNDARY) || (b->fouryears > BSHV_FOURYEARS_UPPER_BOUNDARY))
	{
		return EXIT_FAILURE;
	}

	if ((b->day < BSHV_DAY_LOWER_BOUNDARY) || (b->day > BSHV_DAY_UPPER_BOUNDARY))
	{
		return EXIT_FAILURE;
	}

	if ((b->hour < BSHV_HOUR_LOWER_BOUNDARY) || (b->hour > BSHV_HOUR_UPPER_BOUNDARY))
	{
		return EXIT_FAILURE;
	}

	if ((b->minute < BSHV_MINUTE_LOWER_BOUNDARY) || (b->minute > BSHV_MINUTE_UPPER_BOUNDARY))
	{
		return EXIT_FAILURE;
	}

	if ((b->second < BSHV_SECOND_LOWER_BOUNDARY) || (b->second > BSHV_SECOND_UPPER_BOUNDARY))
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

static void GetCommandWordFromPacket(Byte** p_adr_data, unsigned int* cw)
{
	unsigned int temp1 = (*p_adr_data)->value;
	(*p_adr_data) = (*p_adr_data)->next;

	unsigned int temp2 = (*p_adr_data)->value;
	(*p_adr_data) = (*p_adr_data)->next;

	*cw = (temp1 << 8) | (temp2 << 0);
}

static void GetDataWordsFromPacket(Byte** p_adr_data, unsigned int* dw, unsigned int n)
{
	int i;
	for (i=0; i<n; i++)
	{
		dw[i] = (*p_adr_data)->value;
		(*p_adr_data) = (*p_adr_data)->next;
	}
}

static void Packet_BC_to_RT(Byte* adr_data, unsigned int n)
{
	if ((n < BYTECOUNT_PACKET_BC_TO_RT_LOWER) || (n > BYTECOUNT_PACKET_BC_TO_RT_UPPER))
	{
		// ������ ���������������� ������ � ����� "������ c ����������� ���� � ������"
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_BYTECOUNT);
		return;
	}

	// ������������ �������� ���
	Bshv packet_bshv;
	GetBshvFromPacket(&adr_data, &packet_bshv);
	n = n - bshv_byte_size;

	// ��������� ��������� �������� ���
	int bshv_result = ValidateBshvBoundaries(&packet_bshv);
	if(bshv_result == EXIT_FAILURE)
	{
		// ������ ���������������� ������ � ����� "������: �������� ��� ������� �� ���������� �������"
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_BSHV_BOUNDARY);
		return;
	}

	// �������� ��� � ��������� ���
	int bshv_guard_interval = CompareWithSystemBshv(&system_bshv, &packet_bshv);
	if(bshv_guard_interval == EXIT_FAILURE)
	{
		// ������ ���������������� ������ � ����� "������: �����, ��������� � ������, ��������� �������� ��������"
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_BSHV_TOO_LATE);
		return;
	}

	// ������������ �������� ���������� �����
	unsigned int cw;
	GetCommandWordFromPacket(&adr_data, &cw);
	n = n - commandword_byte_size;

	// ��������� �� (������������ ���-�� ��, ������������ � ������, ���-�� ��, ���������� � ��)
	unsigned int n_datawords;
	MIL1553_GET_WORDCOUNT(cw, &n_datawords);

	if((n_datawords*2) != n)
	{
		// ������ ���������������� ������ � ����� "������: ������������ ���������� ��, ��������� � ��, � ����������� � �����"
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_CW_WORDCOUNT);
		return;
	}

	// ��������� �� (�����-��������)
	unsigned int dir;
	MIL1553_GET_DIRECTION_BIT(cw, &dir);

	if(dir != BCtoRT)
	{
		// ������ ���������������� ������ � ����� "������: �������� ����������� �������� ������"
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_DIRECTION);
		return;
	}

	// ������������ ��
	unsigned int dw[32];
	GetDataWordsFromPacket(&adr_data, dw, n);

	// ��������� � ������ �����
	/*
	int load_result = LoadPacketF1(&packet_bshv, cw, dw, n/2);
	if(load_result == EXIT_SUCCESS)
	{
		// ������ ���������������� ������ � ����� "����� ������� �������� � �����"
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_NO_ERRORS);
	}
*/

	int t = 1;
	t++;
}

static void Packet_RT_to_BC(Byte* adr_data, unsigned int n)
{
	if (n != BYTECOUNT_PACKET_RT_TO_BC)
	{
		// ������ ���������������� ������ � ����� "������ ���������� ���� � ������"
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_BYTECOUNT);
		return;
	}

	// ������������ �������� ���
	Bshv packet_bshv;
	GetBshvFromPacket(&adr_data, &packet_bshv);

	// ��������� ��������� �������� ���
	int bshv_result = ValidateBshvBoundaries(&packet_bshv);
	if(bshv_result == EXIT_FAILURE)
	{
		// ������ ���������������� ������ � ����� "������: �������� ��� ������� �� ���������� �������"
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_BSHV_BOUNDARY);
		return;
	}

	// �������� ��� � ��������� ���
	int bshv_guard_interval = CompareWithSystemBshv(&system_bshv, &packet_bshv);
	if(bshv_guard_interval == EXIT_FAILURE)
	{
		// ������ ���������������� ������ � ����� "������: �����, ��������� � ������, ��������� �������� ��������"
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_BSHV_TOO_LATE);
		return;
	}

	// ������������ �������� ���������� �����
	unsigned int cw;
	GetCommandWordFromPacket(&adr_data, &cw);

	// ��������� �� (�����-��������)
	unsigned int dir;
	MIL1553_GET_DIRECTION_BIT(cw, &dir);

	if(dir != RTtoBC)
	{
		// ������ ���������������� ������ � ����� "������: �������� ����������� �������� ������"
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_DIRECTION);
		return;
	}

	// ��������� � ������ �����
	int load_result = LoadPacketF2(&packet_bshv, cw);
	if(load_result == EXIT_SUCCESS)
	{
		// ������ ���������������� ������ � ����� "����� ������� �������� � �����"
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_NO_ERRORS);
	}


	int t = 1;
	t++;
}

void UartDecodePackets(Byte* adr_data, unsigned int n)
{
	// ���������� ������������� ������
	unsigned int id = adr_data->value;
	adr_data = adr_data->next;

	switch(id)
	{
		case ID_PACKET_BC_TO_RT:
			Packet_BC_to_RT(adr_data, n-1);
			break;

		case ID_PACKET_RT_TO_BC:
			Packet_RT_to_BC(adr_data, n-1);
			break;

		default:
    		// ������ ���������������� ������ � ����� "������ �������������� ������"
    		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_ID);
			break;
	}
}



