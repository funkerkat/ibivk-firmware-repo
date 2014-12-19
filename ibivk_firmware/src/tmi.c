/*
 * tmi.c
 *
 *  Created on: 19.12.2014
 *      Author: Ludmila
 */

#include "tmi_struct.h"
#include "algorithm_error_codes.h"

Tmi ibivk_tmi;

void InitTmi()
{
	// установить версию ПМО
	ibivk_tmi.ver_pmo.pmo_mk_year 					= VER_PMO_MK_YEAR;
	ibivk_tmi.ver_pmo.pmo_mk_month 					= VER_PMO_MK_MONTH;
	ibivk_tmi.ver_pmo.pmo_mk_day 					= VER_PMO_MK_DAY;

	// установить признаки самотестирования ИБИВК в отрицательные значения
	ibivk_tmi.ibivk_selftest.norma_ibivk 			= NOT_NORMAL;
	ibivk_tmi.ibivk_selftest.norma_uart 			= NOT_NORMAL;
	ibivk_tmi.ibivk_selftest.norma_1hz 				= NOT_NORMAL;
	ibivk_tmi.ibivk_selftest.norma_320ms 			= NOT_NORMAL;
	ibivk_tmi.ibivk_selftest.norma_mil1553 			= NOT_NORMAL;
	ibivk_tmi.ibivk_selftest.norma_system_bshv 		= NOT_NORMAL;
	ibivk_tmi.ibivk_selftest.algorithm_error_code 	= NO_ALGORITHM_ERROR;

	// установить указатель в телеметрии на системное время БШВ
	ibivk_tmi.sys_bshv = &system_bshv;

	// установить значения использованных ресурсов ИБИВК
	ibivk_tmi.ibivk_res.n_loaded_messages = 0;
	ibivk_tmi.ibivk_res.load_percent = 0;

	// установить значения самодиагностики УАРТ
	ibivk_tmi.uart_selftest.uart1_error_code = 0;
	ibivk_tmi.uart_selftest.uart2_error_code = 0;

	// установить значения самодиагностики ядра МКИО
	ibivk_tmi.core1553_selftest.core1553_error_code = 0;
}


void UpdateTmi()
{

}
