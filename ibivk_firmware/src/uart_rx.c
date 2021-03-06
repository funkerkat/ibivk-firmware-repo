/*
 * uart.c
 *
 *  Created on: 23.10.2014
 *      Author: Ludmila
 */

// ���������� ��� ������ � ����������
#include "xSystem.h"

// ���������� �������� �������
#include "ibivk_uart_packets.h"
#include "control_sum.h"
#include "uart.h"
#include "programm_errors.h"

// ��������� �������
#include "uart_decode.h"
#include "list_transmit.h"
#include "selftest.h"

unsigned int buffer[BUFFER_LENGTH];


static void BufferClear(ReceiveBufferParams* params )
{
	params->validate_cnt=0;
	params->buffer_cnt=0;

	int i;
	for (i=0; i < BUFFER_LENGTH; i++)
	{
		buffer[i] = 0;
	}
}

static void BufferShift(ReceiveBufferParams* params )
{
	int i;
	for (i=0; i < (	(params->buffer_cnt)-1); i++)
	{
		buffer[i] = buffer[i+1];
	}

	buffer[(params->buffer_cnt)-1] = 0;

	(params->validate_cnt) = 0;
	(params->buffer_cnt)--;
}

static int SearchHead(unsigned int n)
{
	const unsigned int headValue[HEAD_SIZE] = {HEAD_0, HEAD_1, HEAD_2, HEAD_3};
	int i;
	for (i=0; i<n; i++) { if (buffer[i] != headValue[i]) { return EXIT_FAILURE; } }
	return EXIT_SUCCESS;
}


static void SearchInBuffer2(ReceiveBufferParams* params )
{

	unsigned int info_bytes_size;

    // ----- ����� ���������: -----
    if ((params->buffer_cnt) <= HEAD_SIZE)
	{
    	int result_head_search = SearchHead((params->buffer_cnt));
    	if (result_head_search == EXIT_FAILURE) { BufferShift(params); }
    	else { (params->validate_cnt)++; }
	}
    // ----- ���������� ���-�� ���� �������� ���������� N � ������: -----
    else if ((params->buffer_cnt) == (HEAD_SIZE + PACKETLENGTH_SIZE))
    {
    	// ���������� ����� ������ N (����)
    	info_bytes_size = buffer[(HEAD_SIZE + PACKETLENGTH_SIZE) - 1];

    	// �������� ����������� �������� N
    	if (info_bytes_size < PACKET_LENGTH_MIN_VALUE)
    	{
    		BufferShift(params);
    	}
    	else
    	{
    		(params->validate_cnt)++;
    	}
    }
	// ----- ���������� ����� ���� ������: -----
    else if ((params->buffer_cnt) < (HEAD_SIZE + PACKETLENGTH_SIZE + info_bytes_size))
	{
    	(params->validate_cnt)++;
	}
	// ----- ����� ������ ��������: -----
	else if ((params->buffer_cnt) == (HEAD_SIZE + PACKETLENGTH_SIZE + info_bytes_size))
	{
		(params->validate_cnt)++;

    	// 1. ��������� ����������� �����
		unsigned int cs_r = ControlSum(0, buffer, (params->buffer_cnt)-1);
		unsigned int cs_p = buffer[(params->buffer_cnt)-1];

    	// 2. �� ���������� ����������� �����:
		if(cs_r == cs_p)
		{
			UartDecode(buffer, (HEAD_SIZE + PACKETLENGTH_SIZE + info_bytes_size));
    		BufferClear(params);
		}
		else
		{
			// "������: ������������ ����������� �����"
			unsigned int id = buffer[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE - 1];
			//DiagnosticAnswer(cs_p, id, DIAGNOSTIC_ANSWER_ERROR_CS);
			MakeDiagnosticAnswer(cs_p, id, DIAGNOSTIC_ANSWER_ERROR_CS);
    		BufferShift(params);
		}
	}
	// ----- ������: -----
	else
	{
		//DiagnosticAnswer(0, 0, DIAGNOSTIC_ANSWER_ERROR_ALARM);
		// ��������������� ������
	}

}



static void SearchInBuffer(ReceiveBufferParams* params )
{
	unsigned int info_bytes_size;

    // ----- ����� ���������: -----
    if ((params->buffer_cnt) <= HEAD_SIZE)
	{
    	int result_head_search = SearchHead((params->buffer_cnt));
    	if (result_head_search == EXIT_FAILURE) { BufferShift(params); }
    	else { (params->validate_cnt)++; }
	}
    // ----- ������� ID: -----
    else if ((params->buffer_cnt) == (HEAD_SIZE + PACKET_ID_SIZE))
    {
    	(params->validate_cnt)++;
    }
    // ----- ������� N: -----
    else if ((params->buffer_cnt) == (HEAD_SIZE + PACKET_ID_SIZE + PACKETLENGTH_SIZE))
    {
    	// ���������� ���������� ���� ������ N (����)
    	info_bytes_size = buffer[(HEAD_SIZE + PACKET_ID_SIZE + PACKETLENGTH_SIZE) - 1];
    	(params->validate_cnt)++;
    }
	// ----- ���������� ����� ���� ������: -----
    else if ((params->buffer_cnt) < (HEAD_SIZE + PACKET_ID_SIZE + PACKETLENGTH_SIZE + info_bytes_size + CONTROLSUM_SIZE))
	{
    	(params->validate_cnt)++;
	}
	// ----- ����� ������ ��������: -----
	else if ((params->buffer_cnt) == (HEAD_SIZE + PACKET_ID_SIZE + PACKETLENGTH_SIZE + info_bytes_size + CONTROLSUM_SIZE))
	{
		(params->validate_cnt)++;

    	// 1. ��������� ����������� �����
		unsigned int cs_r = ControlSum(4, buffer, (PACKET_ID_SIZE + PACKETLENGTH_SIZE + info_bytes_size));
		unsigned int cs_p = buffer[(params->buffer_cnt)-1];

		cs_r = cs_p;

    	// 2. �� ���������� ����������� �����:
		if(cs_r == cs_p)
		{
			UartDecode(buffer, (HEAD_SIZE + PACKET_ID_SIZE + PACKETLENGTH_SIZE + info_bytes_size + CONTROLSUM_SIZE));
    		BufferClear(params);
		}
		else
		{
			// "������: ������������ ����������� �����"
			unsigned int id = buffer[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE - 1];
			MakeDiagnosticAnswer(cs_p, id, DIAGNOSTIC_ANSWER_ERROR_CS);
    		BufferShift(params);
		}
	}
	// ----- ������: -----
	else
	{
		// ��������������� ������
		PmoSelftest(ALGORITHM_ERROR_CODE_3);
		int t = 0;
		t++;
	}

}


void AddByteToBuffer(unsigned int val)
{
	static ReceiveBufferParams params = {0, 0};

	buffer[params.buffer_cnt] = val;
	(params.buffer_cnt)++;

    while ((params.validate_cnt) < (params.buffer_cnt))
    {
    	SearchInBuffer(&params);
    }

}
