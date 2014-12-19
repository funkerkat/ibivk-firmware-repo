/*
 * uart.c
 *
 *  Created on: 23.10.2014
 *      Author: Ludmila
 */

// библиотеки для работы с периферией
#include "xSystem.h"

// библиотеки текущего проекта
#include "ibivk_uart_packets.h"
#include "control_sum.h"
#include "uart.h"

// прототипы функций
#include "uart_decode.h"
#include "list_transmit.h"

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
    else if ((params->buffer_cnt) == (HEAD_SIZE + PACKETLENGTH_SIZE))
    {
    	// определить длину пакета N (байт)
    	info_bytes_size = buffer[(HEAD_SIZE + PACKETLENGTH_SIZE) - 1];

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
    else if ((params->buffer_cnt) < (HEAD_SIZE + PACKETLENGTH_SIZE + info_bytes_size))
	{
    	(params->validate_cnt)++;
	}
	// ----- Набор данных завершен: -----
	else if ((params->buffer_cnt) == (HEAD_SIZE + PACKETLENGTH_SIZE + info_bytes_size))
	{
		(params->validate_cnt)++;

    	// 1. посчитать контрольную сумму
		unsigned int cs_r = ControlSum(0, buffer, (params->buffer_cnt)-1);
		unsigned int cs_p = buffer[(params->buffer_cnt)-1];

    	// 2. по результату контрольной суммы:
		if(cs_r == cs_p)
		{
			UartDecode(buffer, (HEAD_SIZE + PACKETLENGTH_SIZE + info_bytes_size));
    		BufferClear(params);
		}
		else
		{
			// "Ошибка: несовпадение контрольной суммы"
			unsigned int id = buffer[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE - 1];
			//DiagnosticAnswer(cs_p, id, DIAGNOSTIC_ANSWER_ERROR_CS);
			MakeDiagnosticAnswer(cs_p, id, DIAGNOSTIC_ANSWER_ERROR_CS);
    		BufferShift(params);
		}
	}
	// ----- Ошибка: -----
	else
	{
		//DiagnosticAnswer(0, 0, DIAGNOSTIC_ANSWER_ERROR_ALARM);
		// алгоритмическая ошибка
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
