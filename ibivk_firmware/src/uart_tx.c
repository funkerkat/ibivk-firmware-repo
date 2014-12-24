/*
 * uart_tx.c
 *
 *  Created on: 27.11.2014
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

static void AddServiceBytes(unsigned int data[], unsigned int n_data, unsigned int packet_id)
{
	// Добавить заголовок пакета
	data[0] = HEAD_0;
	data[1] = HEAD_1;
	data[2] = HEAD_2;
	data[3] = HEAD_3;

	// Добавить длину пакета N (байт)
	data[4] = n_data - (HEAD_SIZE + PACKETLENGTH_SIZE);

	// Добавить ID пакета
	data[5] = packet_id;

	// Рассчитать и добавить контрольную сумму пакета (последний байт)
	data[n_data-1] = CountControlSum(data, n_data-1);

	// Выдать в UART
	int error=0;
	int i;
	for (i=0; i<n_data; i++)
	{
		UART2_SEND_BYTE_POLLING(data[i], &error);
		//if (error) { Uart2_ErrorDetected(error); }
		Uart2_Selftest(error);
	}
}

void DiagnosticAnswer(unsigned int received_id, unsigned int received_cs, unsigned int error_code)
{
	#define DATA_LENGH 	(SERVICE_BYTES_LENGTH + BYTECOUNT_DIAGNOSTIC_ANSWER)
	unsigned int data[DATA_LENGH];

	// Заполнить информационные байты
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 0] = received_cs;
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 1] = received_id;
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 2] = error_code;

	// Добавить слежубные байты
	AddServiceBytes(data, DATA_LENGH, ID_PACKET_IBIVK_TO_PC_DIAGNOSTIC_ANSWER);
	#undef DATA_LENGH
}

void IbivkToPcMessageF1(BshvExtention bshv_ext, unsigned short cw, unsigned short sw)
{
	#define DATA_LENGH 	(SERVICE_BYTES_LENGTH + bshv_byte_size + commandword_byte_size + statusword_byte_size)
	unsigned int data[DATA_LENGH];

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

	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 9] = cw  >> (8*1);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 10] = cw >> (8*0);

	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 11] = sw >> (8*1);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 12] = sw >> (8*0);

	// Добавить слежубные байты
	AddServiceBytes(data, (DATA_LENGH), ID_PACKET_IBIVK_TO_PC_F2);
	#undef DATA_LENGH
}

void IbivkToPcMessageF2(BshvExtention bshv_ext, unsigned short cw, unsigned short sw, unsigned short dw[])
{
	#define DATA_LENGH 	(SERVICE_BYTES_LENGTH + bshv_byte_size + commandword_byte_size + statusword_byte_size)
	unsigned int n_dw;
	MIL1553_GET_WORDCOUNT(cw, &n_dw);
	if (n_dw == 0) { n_dw = 32; }
	unsigned int data[DATA_LENGH + n_dw*2];

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
	for (i=0; i<n_dw; i++)
	{
		data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 13 + (2*i + 0)] = dw[i] >> (8*1);
		data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 13 + (2*i + 1)] = dw[i] >> (8*0);
	}

	// Добавить слежубные байты
	AddServiceBytes(data, (DATA_LENGH + n_dw*2), ID_PACKET_IBIVK_TO_PC_F2);
	#undef DATA_LENGH
}


void RS485_send_tmi(Tmi* this_tmi)
{
	#define DATA_LENGH 	(SERVICE_BYTES_LENGTH + BYTECOUNT_TMI)
	unsigned int data[DATA_LENGH];

	// - - - - - - - - - - - - - - - -
	// Заполнить информационные байты:

	// Версия ПМО МК
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 0]  = (this_tmi->ver_mk_pmo.pmo_year);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 1]  = (this_tmi->ver_mk_pmo.pmo_month);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 2]  = (this_tmi->ver_mk_pmo.pmo_day);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 3]  = (this_tmi->ver_mk_pmo.pmo_revision);

	// Версия ПМО ПЛИС
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 4]  = (this_tmi->ver_fpga_pmo.pmo_year);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 5]  = (this_tmi->ver_fpga_pmo.pmo_month);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 6]  = (this_tmi->ver_fpga_pmo.pmo_day);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 7]  = (this_tmi->ver_fpga_pmo.pmo_revision);

	// Системное время ИБИВК
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 8]  = (this_tmi->sys_bshv->fouryears);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 9]  = (this_tmi->sys_bshv->day)  >> 8;
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 10]  = (this_tmi->sys_bshv->day) >> 0;
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 11]  = (this_tmi->sys_bshv->hour);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 12]  = (this_tmi->sys_bshv->minute);
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 13]  = (this_tmi->sys_bshv->second);

	// Интегральные параметры самодиагностики ИБИВК
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 14]  = (this_tmi->integral_params.norma_ibivk 		<< 7)	|
															    (this_tmi->integral_params.norma_uart 			<< 6)	|
															    (this_tmi->integral_params.norma_mil1553 		<< 5)	|
															    (this_tmi->integral_params.norma_input_signals 	<< 4)	|
															    (this_tmi->integral_params.norma_software 		<< 3)	|
															    (this_tmi->integral_params.norma_resources 		<< 2)	|
    															(this_tmi->integral_params.norma_fpga 			<< 1);

	// Самодиагностика УАРТ
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 15] = (this_tmi->selftest_uart.uart1_error_code 		<< 5)	|
															    (this_tmi->selftest_uart.uart2_error_code 		<< 2);

	// Самодиагностика ядра МКИО
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 16] = (this_tmi->selftest_core1553.core1553_error_code) >> 8;
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 17] = (this_tmi->selftest_core1553.core1553_error_code) >> 0;

	// Самодиагностика входных сигналов
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 18] = (this_tmi->selftest_input_signals.norma_1hz		 	<< 7) |
															    (this_tmi->selftest_input_signals.norma_320ms	 	<< 6) |
    															(this_tmi->selftest_input_signals.norma_range_bshv	<< 5) |
																(this_tmi->selftest_input_signals.norma_32_impulse	<< 4);
	// Самодиагностика программного обеспечения
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 19] = (this_tmi->selftest_software.pmo_error_code);

	// Самодиагностика ресурсов
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 20] = (this_tmi->selftest_resources.n_loaded_messages)  >> 8;
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 21] = (this_tmi->selftest_resources.n_loaded_messages)  >> 0;
	data[HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + 22] = (this_tmi->selftest_resources.load_percent);

	// - - - - - - - - - - - - - - - -

	// Добавить слежубные байты
	AddServiceBytes(data, DATA_LENGH, ID_PACKET_IBIVK_TO_PC_TM);
	#undef DATA_LENGH
}
