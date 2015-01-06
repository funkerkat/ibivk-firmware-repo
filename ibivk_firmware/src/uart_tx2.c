/*
 * uart_tx2.c
 *
 *  Created on: 25.12.2014
 *      Author: Ludmila
 */

// библиотеки для работы с периферией
#include "xSystem.h"
#include "xUart.h"

// библиотеки текущего проекта
#include "ibivk_uart_packets.h"
#include "bshv_struct.h"
#include "mil1553.h"
#include "tmi_struct.h"

// прототипы функций
#include "control_sum.h"
#include "selftest.h"

#define ID_and_N (PACKET_ID_SIZE + PACKETLENGTH_SIZE)

static void SendPacket(unsigned int data[], unsigned int n)
{
	// Выдать в UART:
	// Рассчитать КС
	unsigned int cs = CountControlSum(data, n);

	// Выдать пакет в UART:
	unsigned int error = 0;
	// 1. Выдать заголовок
	UART2_SEND_BYTE_POLLING(HEAD_0, &error);
	Uart2_Selftest(error);

	UART2_SEND_BYTE_POLLING(HEAD_1, &error);
	Uart2_Selftest(error);

	UART2_SEND_BYTE_POLLING(HEAD_2, &error);
	Uart2_Selftest(error);

	UART2_SEND_BYTE_POLLING(HEAD_3, &error);
	Uart2_Selftest(error);

	// 2. Выдать идентификатор (ID), кол-во байт данных в пакете (N) и байты данных
	int i; for (i=0; i<n; i++) { UART2_SEND_BYTE_POLLING(data[i], &error); Uart2_Selftest(error); }

	// 5. Выдать контрольную сумму
	UART2_SEND_BYTE_POLLING(cs, &error);
	Uart2_Selftest(error);
}

void Uart_send_diagnostic_answer(unsigned int received_id, unsigned int received_cs, unsigned int error_code)
{
	#define DATA_UNDER_CS_LENGH		(ID_and_N + BYTECOUNT_DIAGNOSTIC_ANSWER)

	// создать массив для данных, подвергающихся защите контрольной суммой
	unsigned int data[DATA_UNDER_CS_LENGH];

	// заполнить ID и N
	data[0] = ID_PACKET_IBIVK_TO_PC_DIAGNOSTIC_ANSWER;
	data[1] = BYTECOUNT_DIAGNOSTIC_ANSWER;

	// Заполнить информационные байты
	data[ID_and_N + 0] = received_cs;
	data[ID_and_N + 1] = received_id;
	data[ID_and_N + 2] = error_code;

	// Выдать пакет в UART
	SendPacket(data, DATA_UNDER_CS_LENGH);
	#undef DATA_UNDER_CS_LENGH
}

void Uart_send_ibivk_to_pc_f1(BshvExtention bshv_ext, unsigned short cw, unsigned short sw)
{
	#define BYTECOUNT_PACKET_IBIVK_TO_PC_F1		(bshv_byte_size + commandword_byte_size + statusword_byte_size)
	#define DATA_UNDER_CS_LENGH 	(ID_and_N + BYTECOUNT_PACKET_IBIVK_TO_PC_F1)

	// создать массив для данных, подвергающихся защите контрольной суммой
	unsigned int data[DATA_UNDER_CS_LENGH];

	// заполнить ID и N
	data[0] = ID_PACKET_IBIVK_TO_PC_F1;
	data[1] = BYTECOUNT_PACKET_IBIVK_TO_PC_F1;

	// Заполнить информационные байты
	data[ID_and_N + 0] = bshv_ext.myBshv.fouryears;
	data[ID_and_N + 1] = bshv_ext.myBshv.day >> (8*1);
	data[ID_and_N + 2] = bshv_ext.myBshv.day >> (8*0);
	data[ID_and_N + 3] = bshv_ext.myBshv.hour;
	data[ID_and_N + 4] = bshv_ext.myBshv.minute;
	data[ID_and_N + 5] = bshv_ext.myBshv.second;
	data[ID_and_N + 6] = bshv_ext.microsecond >> (8*2);
	data[ID_and_N + 7] = bshv_ext.microsecond >> (8*1);
	data[ID_and_N + 8] = bshv_ext.microsecond >> (8*0);

	data[ID_and_N + 9]  = cw >> (8*1);
	data[ID_and_N + 10] = cw >> (8*0);

	data[ID_and_N + 11] = sw >> (8*1);
	data[ID_and_N + 12] = sw >> (8*0);

	// Выдать пакет в UART
	SendPacket(data, DATA_UNDER_CS_LENGH);
	#undef DATA_UNDER_CS_LENGH
	#undef BYTECOUNT_PACKET_IBIVK_TO_PC_F1
}

void Uart_send_ibivk_to_pc_f2(BshvExtention bshv_ext, unsigned short cw, unsigned short sw, unsigned short dw[])
{
	#define BYTECOUNT_PACKET_IBIVK_TO_PC_F2_BASE		(bshv_byte_size + commandword_byte_size + statusword_byte_size)

	// вычистить количество байт переменной длины (n_data_bytes)
	unsigned int n_data_words;
	MIL1553_GET_WORDCOUNT(cw, &n_data_words);
	if (n_data_words == 0) { n_data_words = 32; }
	unsigned int n_data_bytes = n_data_words*2;

	// создать массив для данных, подвергающихся защите контрольной суммой
	unsigned int data_under_cs_length = (ID_and_N + BYTECOUNT_PACKET_IBIVK_TO_PC_F2_BASE + n_data_bytes);
	unsigned int data[data_under_cs_length];

	// Заполнить информационные байты
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 0] = bshv_ext.myBshv.fouryears;
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 1] = bshv_ext.myBshv.day >> (8*1);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 2] = bshv_ext.myBshv.day >> (8*0);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 3] = bshv_ext.myBshv.hour;
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 4] = bshv_ext.myBshv.minute;
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 5] = bshv_ext.myBshv.second;
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 6] = bshv_ext.microsecond >> (8*2);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 7] = bshv_ext.microsecond >> (8*1);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 8] = bshv_ext.microsecond >> (8*0);

	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 9]  = cw >> (8*1);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 10] = cw >> (8*0);

	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 11] = sw >> (8*1);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 12] = sw >> (8*0);

	int i;
	for (i=0; i<n_data_words; i++)
	{
		data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 13 + (2*i + 0)] = dw[i] >> (8*1);
		data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 13 + (2*i + 1)] = dw[i] >> (8*0);
	}

	// Выдать пакет в UART
	SendPacket(data, data_under_cs_length);
	#undef BYTECOUNT_PACKET_IBIVK_TO_PC_F2_BASE
}

void Uart_send_tmi(Tmi* this_tmi)
{
	#define DATA_UNDER_CS_LENGH 	(ID_and_N + BYTECOUNT_TMI)

	// создать массив для данных, подвергающихся защите контрольной суммой
	unsigned int data[DATA_UNDER_CS_LENGH];

	// заполнить ID и N
	data[0] = ID_PACKET_IBIVK_TO_PC_TM;
	data[1] = BYTECOUNT_TMI;

	// Заполнить информационные байты:
	// Версия ПМО МК
	data[ID_and_N + 0]  = (this_tmi->ver_mk_pmo.pmo_year);
	data[ID_and_N + 1]  = (this_tmi->ver_mk_pmo.pmo_month);
	data[ID_and_N + 2]  = (this_tmi->ver_mk_pmo.pmo_day);
	data[ID_and_N + 3]  = (this_tmi->ver_mk_pmo.pmo_revision);

	// Версия ПМО ПЛИС
	data[ID_and_N + 4]  = (this_tmi->ver_fpga_pmo.pmo_year);
	data[ID_and_N + 5]  = (this_tmi->ver_fpga_pmo.pmo_month);
	data[ID_and_N + 6]  = (this_tmi->ver_fpga_pmo.pmo_day);
	data[ID_and_N + 7]  = (this_tmi->ver_fpga_pmo.pmo_revision);

	// Системное время ИБИВК
	data[ID_and_N + 8]   = (this_tmi->sys_bshv->fouryears);
	data[ID_and_N + 9]   = (this_tmi->sys_bshv->day)  >> 8;
	data[ID_and_N + 10]  = (this_tmi->sys_bshv->day) >> 0;
	data[ID_and_N + 11]  = (this_tmi->sys_bshv->hour);
	data[ID_and_N + 12]  = (this_tmi->sys_bshv->minute);
	data[ID_and_N + 13]  = (this_tmi->sys_bshv->second);

	// Интегральные параметры самодиагностики ИБИВК
	data[ID_and_N + 14]  = (this_tmi->integral_params.norma_ibivk 			<< 7)	|
							(this_tmi->integral_params.norma_uart 			<< 6)	|
							(this_tmi->integral_params.norma_mil1553 		<< 5)	|
							(this_tmi->integral_params.norma_input_signals 	<< 4)	|
							(this_tmi->integral_params.norma_software 		<< 3)	|
							(this_tmi->integral_params.norma_resources 		<< 2)	|
    						(this_tmi->integral_params.norma_fpga 			<< 1);

	// Самодиагностика УАРТ
	data[ID_and_N + 15] = (this_tmi->selftest_uart.uart1_error_code 		<< 5)	|
						  (this_tmi->selftest_uart.uart2_error_code 		<< 2);

	// Самодиагностика ядра МКИО
	data[ID_and_N + 16] = (this_tmi->selftest_core1553.core1553_error_code) >> 8;
	data[ID_and_N + 17] = (this_tmi->selftest_core1553.core1553_error_code) >> 0;

	// Самодиагностика входных сигналов
	data[ID_and_N + 18] = (this_tmi->selftest_input_signals.norma_1hz		<< 7) |
						  (this_tmi->selftest_input_signals.norma_320ms	 	<< 6) |
    					  (this_tmi->selftest_input_signals.norma_range_bshv<< 5) |
						  (this_tmi->selftest_input_signals.norma_32_impulse<< 4);

	// Самодиагностика программного обеспечения
	data[ID_and_N + 19] = (this_tmi->selftest_software.pmo_error_code);

	// Самодиагностика ресурсов
	data[ID_and_N + 20] = (this_tmi->selftest_resources.n_loaded_messages)  >> 8;
	data[ID_and_N + 21] = (this_tmi->selftest_resources.n_loaded_messages)  >> 0;
	data[ID_and_N + 22] = (this_tmi->selftest_resources.load_percent);

	// Выдать пакет в UART
	SendPacket(data, DATA_UNDER_CS_LENGH);
	#undef DATA_UNDER_CS_LENGH
}
