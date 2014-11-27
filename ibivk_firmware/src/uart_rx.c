/*
 * uart.c
 *
 *  Created on: 23.10.2014
 *      Author: Ludmila
 */
#include "xSystem.h"
#include "ibivk_uart_packets.h"
#include "control_sum.h"
#include "uart.h"

unsigned int buffer[BUFFER_LENGTH];


void BufferClear(ReceiveBufferParams* params )
{

	params->validate_cnt=0;
	params->buffer_cnt=0;

	int i;
	for (i=0; i < BUFFER_LENGTH; i++)
	{
		buffer[i] = 0;
	}
}

void BufferShift(ReceiveBufferParams* params )
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

static void SearchInBuffer(ReceiveBufferParams* params )
{

	unsigned int info_bytes_size;

    // ----- Поиск заголовка: -----
    if ((params->buffer_cnt) <= HEAD_SIZE)
	{
    	int result_head_search = SearchHead((params->buffer_cnt));
    	if (result_head_search == EXIT_FAILURE) { BufferShift(params); }
    	else { (params->validate_cnt)++; }
	}
    // ----- Определить кол-во байт полезной информации N в пакете: -----
    else if ((params->buffer_cnt) == SERVICE_BYTES_SIZE)
    {
    	// определить длину пакета N (байт)
    	info_bytes_size = buffer[SERVICE_BYTES_SIZE-1];

    	// проверка допустимого значения N
    	if (info_bytes_size < PACKET_LENGTH_MIN_VALUE)
    	{
    		BufferShift(params);
    	}
    	else
    	{
    		(params->validate_cnt)++;
    	}
    }
	// ----- Продолжить набор байт данных: -----
    else if ((params->buffer_cnt) < (SERVICE_BYTES_SIZE + info_bytes_size))
	{
    	(params->validate_cnt)++;
	}
	// ----- Набор данных завершен: -----
	else if ((params->buffer_cnt) == (SERVICE_BYTES_SIZE + info_bytes_size))
	{
		(params->validate_cnt)++;

    	// 1. посчитать контрольную сумму
		unsigned int cs_r = ControlSum(0, buffer, (params->buffer_cnt)-1);
		unsigned int cs_p = buffer[(params->buffer_cnt)-1];

    	// 2. по результату контрольной суммы:
		if(cs_r == cs_p)
		{
    		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_NO_ERRORS);
    		BufferClear(params);
		}
		else
		{
    		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_CS);
    		BufferShift(params);
		}
	}
	// ----- Ошибка: -----
	else
	{
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_ALGORITHM);
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

        int t = 0;
        t++;
    }

}
