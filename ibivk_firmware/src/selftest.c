/*
 * selftest.c
 *
 *  Created on: 07.11.2014
 *      Author: Ludmila
 */

// страндартная библиотека Си
#include <stdlib.h>

// библиотеки для работы с периферией
#include "xUart.h"

// библиотеки текущего проекта
#include "list_transmit_struct.h"
#include "ibivk_uart_packets.h"
#include "tmi_struct.h"

// прототипы функций
#include "tmi.h"


static void SendTmiNotNormal()
{
	UpdateIntegralParams();
	if (ibivk_tmi.integral_params.norma_ibivk == NOT_NORMAL)
	{
		// Выдать телеметрический пакет в УАРТ:
		// 1. выделить память для хранения данных для последующей выдачи в УАРТ
		ListTransmit* item = (ListTransmit*) malloc(sizeof(ListTransmit));

		// 2. скопировать данные
		item->packet_id = ID_PACKET_IBIVK_TO_PC_TM;
		item->data.data_tmi.p_tmi = &ibivk_tmi;

		// 3. отправить на хранения данные (до наступления очереди выдачи в УАРТ)
		AddItemToListTransmit(item);
	}

	int t = 1;
	t++;
}

void Uart1_Selftest(unsigned int error_detected)
{
	// есть ошибки
	if (error_detected)
	{
		// очистить биты неисправности из регистра УАРТ
		UART1_STATUS_RESET();
		// занести отчет о неисправности в телеметрию
		ibivk_tmi.selftest_uart.uart1_error_code = error_detected;
		// выдать телеметрию в ПК
		SendTmiNotNormal();
	}
}

void Uart2_Selftest(unsigned int error_detected)
{
	// есть ошибки
	if (error_detected)
	{
		// очистить биты неисправности из регистра УАРТ
		UART2_STATUS_RESET();
		// занести отчет о неисправности в телеметрию
		ibivk_tmi.selftest_uart.uart2_error_code = error_detected;
		// выдать телеметрию в ПК
		SendTmiNotNormal();
	}
}

void Core1553_Selftest(unsigned short core1553_pending)
{
	if (core1553_pending != 0)
	{
		ibivk_tmi.selftest_core1553.core1553_error_code = core1553_pending;
		SendTmiNotNormal();
	}

}
