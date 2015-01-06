/*
 * tmi.c
 *
 *  Created on: 19.12.2014
 *      Author: Ludmila
 */

// библиотеки для работы с периферией

// библиотеки текущего проекта
#include "tmi_struct.h"
#include "programm_errors.h"
#include "lib_bshv_transceiver.h"

// прототипы функций
///#include "fpga_ibivk.h"

Tmi ibivk_tmi;

void SetNumberOfMessages(unsigned int n)
{
	ibivk_tmi.selftest_resources.n_loaded_messages = n;
}

void UpdateIntegralParams()
{
	// формирование интегрального признака Норма УАРТ
	if ((ibivk_tmi.selftest_uart.uart1_error_code == 0) && (ibivk_tmi.selftest_uart.uart2_error_code == 0))
	{ ibivk_tmi.integral_params.norma_uart = NORMAL; }
	else
	{ ibivk_tmi.integral_params.norma_uart = NOT_NORMAL; }

	// формирование интегрального признака Норма ядра 1553
	if (ibivk_tmi.selftest_core1553.core1553_error_code == 0)
	{ ibivk_tmi.integral_params.norma_mil1553 = NORMAL; }
	else
	{ ibivk_tmi.integral_params.norma_mil1553 = NOT_NORMAL; }

	// формирование интегрального признака Норма входных сигналов
	if ((ibivk_tmi.selftest_input_signals.norma_1hz == 0) && (ibivk_tmi.selftest_input_signals.norma_320ms == 0) && (ibivk_tmi.selftest_input_signals.norma_range_bshv == 0) && (ibivk_tmi.selftest_input_signals.norma_32_impulse == 0))
	{ ibivk_tmi.integral_params.norma_input_signals = NORMAL; }
	else
	{ ibivk_tmi.integral_params.norma_input_signals = NOT_NORMAL; }

	// формирование интегрального признака Норма программного обеспечения
	if (ibivk_tmi.selftest_software.pmo_error_code == 0)
	{ ibivk_tmi.integral_params.norma_software = NORMAL; }
	else
	{ ibivk_tmi.integral_params.norma_software = NOT_NORMAL; }

	// формирование интегрального признака Норма использования ресурсов
	if ((ibivk_tmi.selftest_resources.load_percent < MAX_PERCENT_LOADED) && (ibivk_tmi.selftest_resources.n_loaded_messages <= MAX_NUMBER_OF_MESSAGES_LOADED))
	{ ibivk_tmi.integral_params.norma_resources = NORMAL; }
	else
	{ ibivk_tmi.integral_params.norma_resources = NOT_NORMAL; }

	// формирование интегрального признака Норма ИБИВК
	ibivk_tmi.integral_params.norma_ibivk  = (ibivk_tmi.integral_params.norma_uart) 			||
			 	 	 	 	 	 	 	 	 (ibivk_tmi.integral_params.norma_mil1553)			||
			 	 	 	 	 	 	 	 	 (ibivk_tmi.integral_params.norma_input_signals)	||
			 	 	 	 	 	 	 	 	 (ibivk_tmi.integral_params.norma_software)			||
			 	 	 	 	 	 	 	 	 (ibivk_tmi.integral_params.norma_resources)		||
	 	 	 	 	 	 	 	 	 	 	 (ibivk_tmi.integral_params.norma_fpga);
}

void InitTmi()
{
	// установить версию ПМО МК
	ibivk_tmi.ver_mk_pmo.pmo_year 			= VER_PMO_MK_YEAR;
	ibivk_tmi.ver_mk_pmo.pmo_month 			= VER_PMO_MK_MONTH;
	ibivk_tmi.ver_mk_pmo.pmo_day 			= VER_PMO_MK_DAY;
	ibivk_tmi.ver_mk_pmo.pmo_revision 		= VER_PMO_MK_REVISION;

	// установить версию ПМО ПЛИС (?)
	VersionPmo pmo_fpga;
	BshvTransceiver_Get_VersionPmo(&pmo_fpga);
	ibivk_tmi.ver_fpga_pmo.pmo_year 		= pmo_fpga.pmo_year;
	ibivk_tmi.ver_fpga_pmo.pmo_month 		= pmo_fpga.pmo_month;
	ibivk_tmi.ver_fpga_pmo.pmo_day 			= pmo_fpga.pmo_day;
	ibivk_tmi.ver_fpga_pmo.pmo_revision 	= pmo_fpga.pmo_revision;

	// установить указатель в телеметрии на системное время БШВ
	ibivk_tmi.sys_bshv = &system_bshv;

	// норма ПЛИС
	ibivk_tmi.integral_params.norma_fpga = NORMAL;

	// инициализировать параметры самотестирвоания УАРТ
	ibivk_tmi.selftest_uart.uart1_error_code = 0;
	ibivk_tmi.selftest_uart.uart2_error_code = 0;

	// инициализировать параметры самотестирвоания ядра 1553
	ibivk_tmi.selftest_core1553.core1553_error_code = 0;

	// инициализировать параметры самотестирвоания входных сигналов
	ibivk_tmi.selftest_input_signals.norma_1hz = NORMAL;
	ibivk_tmi.selftest_input_signals.norma_320ms = NORMAL;
	ibivk_tmi.selftest_input_signals.norma_range_bshv = NORMAL;
	ibivk_tmi.selftest_input_signals.norma_32_impulse = NORMAL;

	// инициализировать параметры самотестирвоания программного обеспечения
	ibivk_tmi.selftest_software.pmo_error_code = NO_PMO_ERROR;

	// инициализировать параметры использования ресурсов
	ibivk_tmi.selftest_resources.load_percent = 0;
	ibivk_tmi.selftest_resources.n_loaded_messages = 0;

	// установить интергальные признаки
	UpdateIntegralParams();
}

void CleanTmi()
{
	// очистить параметры самотестирвоания УАРТ
	ibivk_tmi.selftest_uart.uart1_error_code = 0;
	ibivk_tmi.selftest_uart.uart2_error_code = 0;

	// очистить параметры самотестирвоания ядра 1553
	ibivk_tmi.selftest_core1553.core1553_error_code = 0;

	// инициализировать параметры самотестирвоания входных сигналов
	ibivk_tmi.selftest_input_signals.norma_1hz = NORMAL;
	ibivk_tmi.selftest_input_signals.norma_320ms = NORMAL;
	ibivk_tmi.selftest_input_signals.norma_range_bshv = NORMAL;
	ibivk_tmi.selftest_input_signals.norma_32_impulse = NORMAL;

	// инициализировать параметры самотестирвоания программного обеспечения
	ibivk_tmi.selftest_software.pmo_error_code = 0;

	// инициализировать параметры использования ресурсов
	ibivk_tmi.selftest_resources.load_percent = 0;
	ibivk_tmi.selftest_resources.n_loaded_messages = 0;

	// установить интергальные признаки
	UpdateIntegralParams();
}

