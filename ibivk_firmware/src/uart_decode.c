/*
 * uart_decode.c
 *
 *  Created on: 27.11.2014
 *      Author: Ludmila
 */
#include "xSystem.h"
#include "shift_reg.h"
#include "ibivk_uart_packets.h"
#include "bshv.h"
#include "mil1553.h"

static void GetBshvFromPacket(unsigned int data[], Bshv* packet_bshv)
{
	// декодировать номер четырехлетия
	packet_bshv->fouryears = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE+1) + 0 - 1];

	// декодировать номер суток
	unsigned int temp1, temp2;
	temp1 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE+1) + 1 - 1];
	temp2 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE+1) + 2 - 1];
	packet_bshv->day = (temp1 << 8) | (temp2 << 0);

	// декодировать номер часа
	packet_bshv->hour = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE+1) + 3 - 1];

	// декодировать номер минуты
	packet_bshv->minute = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE+1) + 4 - 1];

	// декодировать номер секунды
	packet_bshv->second = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE+1) + 5 - 1];


	// декодировать номер микросекунды
	unsigned int t1, t2, t3;

	t1 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE+1) + 6 - 1];
	t2 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE+1) + 7 - 1];
	t3 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE+1) + 8 - 1];

	packet_bshv->microsecond = ((t1 << 8*2)|(t2 << 8*1)|(t3 << 8*0));
}

static int ValidateBshvBoundaries(Bshv* b)
{
	if ((b->fouryears < BSHV_FOURYEARS_LOWER_BOUNDARY) ||
		(b->fouryears > BSHV_FOURYEARS_UPPER_BOUNDARY))
	{ return EXIT_FAILURE; }

	if ((b->day < BSHV_DAY_LOWER_BOUNDARY) ||
		(b->day > BSHV_DAY_UPPER_BOUNDARY))
	{ return EXIT_FAILURE; }

	if ((b->hour < BSHV_HOUR_LOWER_BOUNDARY) ||
		(b->hour > BSHV_HOUR_UPPER_BOUNDARY))
	{ return EXIT_FAILURE; }

	if ((b->minute < BSHV_MINUTE_LOWER_BOUNDARY) ||
		(b->minute > BSHV_MINUTE_UPPER_BOUNDARY))
	{ return EXIT_FAILURE; }

	if ((b->second < BSHV_SECOND_LOWER_BOUNDARY) ||
		(b->second > BSHV_SECOND_UPPER_BOUNDARY))
	{ return EXIT_FAILURE; }

	return EXIT_SUCCESS;
}

static unsigned int GetCommandWordFromPacket(unsigned int data[])
{
	unsigned int temp1, temp2;
	temp1 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE + bshv_byte_size + 1) + 0 - 1];
	temp2 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE + bshv_byte_size + 1) + 1 - 1];

	unsigned int cw = (temp1 << 8) | (temp2 << 0);
	return cw;
}
/*
static unsigned int GetDataWordsFromPacket2(unsigned int data[], unsigned int n_datawords, unsigned int data_longs[])
{
	unsigned int n_datalongs = n_datawords / 2;
	unsigned int remainder = n_datawords - n_datalongs*2;

	int i;
	for (i=0; i<n_datalongs; i++)
	{
		unsigned int temp1, temp2, temp3, temp4;

		temp1 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE + bshv_byte_size + commandword_byte_size + 1) + (4*i + 0) - 1];
		temp2 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE + bshv_byte_size + commandword_byte_size + 1) + (4*i + 1) - 1];
		temp3 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE + bshv_byte_size + commandword_byte_size + 1) + (4*i + 2) - 1];
		temp4 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE + bshv_byte_size + commandword_byte_size + 1) + (4*i + 3) - 1];

		unsigned int d = (temp1 << (3*8)) | (temp2 << (2*8)) | (temp3 << (1*8)) | (temp4 << (0*8));
		data_longs[i] = d;
	}

	for (i=0; i<remainder; i++)
	{
		unsigned int temp1, temp2, temp3, temp4;

		temp1 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE + bshv_byte_size + commandword_byte_size + 1) + (n_datalongs*4) + (4*i + 0) - 1];
		temp2 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE + bshv_byte_size + commandword_byte_size + 1) + (n_datalongs*4) + (4*i + 1) - 1];
		temp3 = 0;
		temp4 = 0;

		unsigned int d = (temp1 << (3*8)) | (temp2 << (2*8)) | (temp3 << (1*8)) | (temp4 << (0*8));
		data_longs[n_datalongs + remainder - 1] = d;
	}

	if (remainder != 0)
	{
		return (n_datalongs + 1);
	}
	else
	{
		return (n_datalongs);
	}

}
*/
static void GetDataWordsFromPacket(unsigned int data[], unsigned int n_data_shorts, unsigned short data_shorts[])
{
	int i;
	for (i=0; i<n_data_shorts; i++)
	{
		unsigned int temp1, temp2;
		temp1 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE + bshv_byte_size + commandword_byte_size + 1) + (2*i + 0) - 1];
		temp2 = data[(HEAD_SIZE+PACKETLENGTH_SIZE+PACKET_ID_SIZE + bshv_byte_size + commandword_byte_size + 1) + (2*i + 1) - 1];
		data_shorts[i] = (temp1 << (1*8)) | (temp2 << (0*8));
	}
}
static unsigned int GetDataWordCount(unsigned int cw)
{
	unsigned int wordcount = (cw & 0x1F);
	if (wordcount == 0) { wordcount = 32; }
	return wordcount;
}

static unsigned int GetDirection(unsigned int cw)
{
	unsigned int direction = (cw >> 10) & 0x01;
	return direction;
}

static void Packet_BC_to_RT(unsigned int data[], unsigned int n)
{
	// определить контрольнцю сумму, принятую в пакете
	unsigned int cs = data[n - 1];

	// определить идентификатор пакета
	unsigned int id = data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE - 1];

	// 1. Проверить количество байт в пакете
	unsigned int n_info_bytes = n - (HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + CONTROLSUM_SIZE);
	if ((n_info_bytes < BYTECOUNT_PACKET_BC_TO_RT_LOWER) || (n_info_bytes > BYTECOUNT_PACKET_BC_TO_RT_UPPER))
	{
		DiagnosticAnswer(cs, id, DIAGNOSTIC_ANSWER_ERROR_BYTECOUNT);
		return;
	}

	// 2. Декодировать значение БШВ и проверить граничные значения БШВ
	Bshv packet_bshv;
	GetBshvFromPacket(data, &packet_bshv);
	int bshv_result = ValidateBshvBoundaries(&packet_bshv);
	if(bshv_result == EXIT_FAILURE)
	{
		DiagnosticAnswer(cs, id, DIAGNOSTIC_ANSWER_ERROR_BSHV_BOUNDARY);
		return;
	}

	// 3. Сравнить БШВ с системным БШВ
	int bshv_guard_interval = CompareWithSystemBshv(&system_bshv, &packet_bshv);
	if(bshv_guard_interval == EXIT_FAILURE)
	{
		DiagnosticAnswer(cs, id, DIAGNOSTIC_ANSWER_ERROR_BSHV_TOO_LATE);
		return;
	}

	// 3. Декодировать значение командного слова и
	unsigned int cw = GetCommandWordFromPacket(data);

	// проверить значение бита "Направление передачи"
	unsigned int dir = GetDirection(cw);
	if(dir != BCtoRT)
	{
		DiagnosticAnswer(cs, id, DIAGNOSTIC_ANSWER_ERROR_DIRECTION);
		return;
	}

	// проверить соответствие командного слова количеству слов данных
	unsigned int n_datawords = GetDataWordCount(cw);
	unsigned int n_databytes = n - (HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE) - bshv_byte_size - commandword_byte_size - CONTROLSUM_SIZE;
	if((n_datawords*2) != n_databytes)
	{
		DiagnosticAnswer(cs, id, DIAGNOSTIC_ANSWER_ERROR_CW_WORDCOUNT);
		return;
	}

	// 4. Декодировать слова данных
	/*
	unsigned int data_longs[16];
	unsigned int n_data_longs = GetDataWordsFromPacket(data, n_datawords, data_longs);
	*/
	unsigned short data_shorts[32];
	GetDataWordsFromPacket(data, n_datawords, data_shorts);

	// 5. Загрузить в память ИБИВК
	/*
	int load_result = LoadPacketF1(&packet_bshv, cw, data_longs, n_data_longs);
	*/
	int load_result = LoadPacketF1(&packet_bshv, cw, data_shorts);
	if(load_result == EXIT_SUCCESS)
	{
		DiagnosticAnswer(cs, id, DIAGNOSTIC_ANSWER_NO_ERRORS);
	}

}

static void Packet_RT_to_BC(unsigned int data[], unsigned int n)
{
	// определить контрольнцю сумму, принятую в пакете
	unsigned int cs = data[n - 1];

	// определить идентификатор пакета
	unsigned int id = data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE - 1];

	// 1. Проверить количество байт в пакете
	unsigned int n_info_bytes = n - (HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + CONTROLSUM_SIZE);
	if (n_info_bytes != BYTECOUNT_PACKET_RT_TO_BC)
	{
		DiagnosticAnswer(cs, id, DIAGNOSTIC_ANSWER_ERROR_BYTECOUNT);
		return;
	}

	// 2. Декодировать значение БШВ и проверить граничные значения БШВ
	Bshv packet_bshv;
	GetBshvFromPacket(data, &packet_bshv);
	int bshv_result = ValidateBshvBoundaries(&packet_bshv);
	if(bshv_result == EXIT_FAILURE)
	{
		DiagnosticAnswer(cs, id, DIAGNOSTIC_ANSWER_ERROR_BSHV_BOUNDARY);
		return;
	}

	// 3. Сравнить БШВ с системным БШВ
	int bshv_guard_interval = CompareWithSystemBshv(&system_bshv, &packet_bshv);
	if(bshv_guard_interval == EXIT_FAILURE)
	{
		DiagnosticAnswer(cs, id, DIAGNOSTIC_ANSWER_ERROR_BSHV_TOO_LATE);
		return;
	}

	// 3. Декодировать значение командного слова и
	unsigned int cw = GetCommandWordFromPacket(data);

	// проверить значение бита "Направление передачи"
	unsigned int dir = GetDirection(cw);
	if(dir != RTtoBC)
	{
		DiagnosticAnswer(cs, id, DIAGNOSTIC_ANSWER_ERROR_DIRECTION);
		return;
	}

	// 4. Загрузить в память ИБИВК
	int load_result = LoadPacketF2(&packet_bshv, cw);
	if(load_result == EXIT_SUCCESS)
	{
		DiagnosticAnswer(cs, id, DIAGNOSTIC_ANSWER_NO_ERRORS);
	}

}



void UartDecode(unsigned int data[], unsigned int n)
{
	// определить идентификатор пакета
	unsigned int id = data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE - 1];

	// определить контрольнцю сумму, принятую в пакете
	unsigned int cs = data[n - 1];

	switch(id)
	{
		case ID_PACKET_BC_TO_RT:
			Packet_BC_to_RT(data, n);
			break;

		case ID_PACKET_RT_TO_BC:
			Packet_RT_to_BC(data, n);
			break;

		default:
			// "Ошибка: неверный идентификатор пакета"
			DiagnosticAnswer(cs, id, DIAGNOSTIC_ANSWER_ERROR_ID);
			break;
	}
}



