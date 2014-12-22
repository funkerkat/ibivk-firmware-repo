/*
 * tmi.c
 *
 *  Created on: 19.12.2014
 *      Author: Ludmila
 */

#include "tmi_struct.h"
#include "algorithm_error_codes.h"

Tmi ibivk_tmi;

void UpdateIntegralParams()
{
	// ������������ ������������� �������� ����� ����
	if ((ibivk_tmi.selftest_uart.uart1_error_code == 0) && (ibivk_tmi.selftest_uart.uart2_error_code == 0))
	{ ibivk_tmi.integral_params.norma_uart = NORMAL; }
	else
	{ ibivk_tmi.integral_params.norma_uart = NOT_NORMAL; }

	// ������������ ������������� �������� ����� ���� 1553
	if (ibivk_tmi.selftest_core1553.core1553_error_code == 0)
	{ ibivk_tmi.integral_params.norma_mil1553 = NORMAL; }
	else
	{ ibivk_tmi.integral_params.norma_mil1553 = NOT_NORMAL; }

	// ������������ ������������� �������� ����� ������� ��������
	if ((ibivk_tmi.selftest_input_signals.norma_1hz == 0) && (ibivk_tmi.selftest_input_signals.norma_320ms == 0) && (ibivk_tmi.selftest_input_signals.norma_digital_bshv == 0))
	{ ibivk_tmi.integral_params.norma_input_signals = NORMAL; }
	else
	{ ibivk_tmi.integral_params.norma_input_signals = NOT_NORMAL; }

	// ������������ ������������� �������� ����� ������������ �����������
	if (ibivk_tmi.selftest_software.algorithm_error_code == 0)
	{ ibivk_tmi.integral_params.norma_software = NORMAL; }
	else
	{ ibivk_tmi.integral_params.norma_software = NOT_NORMAL; }

	// ������������ ������������� �������� ����� ������������� ��������
	if ((ibivk_tmi.selftest_resources.load_percent < MAX_PERCENT_LOADED) && (ibivk_tmi.selftest_resources.n_loaded_messages <= MAX_NUMBER_OF_MESSAGES_LOADED))
	{ ibivk_tmi.integral_params.norma_resources = NORMAL; }
	else
	{ ibivk_tmi.integral_params.norma_resources = NOT_NORMAL; }

	// ������������ ������������� �������� ����� �����
	ibivk_tmi.integral_params.norma_ibivk  = ibivk_tmi.integral_params.norma_uart 			||
			 	 	 	 	 	 	 	 	 ibivk_tmi.integral_params.norma_mil1553		||
			 	 	 	 	 	 	 	 	 ibivk_tmi.integral_params.norma_input_signals	||
			 	 	 	 	 	 	 	 	 ibivk_tmi.integral_params.norma_software		||
			 	 	 	 	 	 	 	 	 ibivk_tmi.integral_params.norma_resources;
}

void InitTmi()
{
	// ���������� ������ ���
	ibivk_tmi.ver_pmo.pmo_mk_year 			= VER_PMO_MK_YEAR;
	ibivk_tmi.ver_pmo.pmo_mk_month 			= VER_PMO_MK_MONTH;
	ibivk_tmi.ver_pmo.pmo_mk_day 			= VER_PMO_MK_DAY;

	// ���������� ��������� � ���������� �� ��������� ����� ���
	ibivk_tmi.sys_bshv = &system_bshv;

	// ���������������� ��������� ���������������� ����
	ibivk_tmi.selftest_uart.uart1_error_code = 0;
	ibivk_tmi.selftest_uart.uart2_error_code = 0;

	// ���������������� ��������� ���������������� ���� 1553
	ibivk_tmi.selftest_core1553.core1553_error_code = 0;

	// ���������������� ��������� ���������������� ������� ��������
	ibivk_tmi.selftest_input_signals.norma_1hz = NORMAL;
	ibivk_tmi.selftest_input_signals.norma_320ms = NORMAL;
	ibivk_tmi.selftest_input_signals.norma_digital_bshv = NORMAL;

	// ���������������� ��������� ���������������� ������������ �����������
	ibivk_tmi.selftest_software.algorithm_error_code = 0;

	// ���������������� ��������� ������������� ��������
	ibivk_tmi.selftest_resources.load_percent = 0;
	ibivk_tmi.selftest_resources.n_loaded_messages = 0;

	// ���������� ������������ ��������
	UpdateIntegralParams();
}

void CleanTmi()
{
	// �������� ��������� ���������������� ����
	ibivk_tmi.selftest_uart.uart1_error_code = 0;
	ibivk_tmi.selftest_uart.uart2_error_code = 0;

	// �������� ��������� ���������������� ���� 1553
	ibivk_tmi.selftest_core1553.core1553_error_code = 0;

	// ���������������� ��������� ���������������� ������� ��������
	ibivk_tmi.selftest_input_signals.norma_1hz = NORMAL;
	ibivk_tmi.selftest_input_signals.norma_320ms = NORMAL;
	ibivk_tmi.selftest_input_signals.norma_digital_bshv = NORMAL;

	// ���������������� ��������� ���������������� ������������ �����������
	ibivk_tmi.selftest_software.algorithm_error_code = 0;

	// ���������������� ��������� ������������� ��������
	ibivk_tmi.selftest_resources.load_percent = 0;
	ibivk_tmi.selftest_resources.n_loaded_messages = 0;

	// ���������� ������������ ��������
	UpdateIntegralParams();
}

