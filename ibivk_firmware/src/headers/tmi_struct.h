/*
 * tmi_struct.h
 *
 *  Created on: 19.12.2014
 *      Author: Ludmila
 */

#include "xVersionPmo.h"
#include "bshv_struct.h"

#ifndef TMI_STRUCT_H_
#define TMI_STRUCT_H_

#define MAX_NUMBER_OF_MESSAGES_LOADED		30000
#define MAX_PERCENT_LOADED					99

enum _VersionPmoData
 {
	 VER_PMO_MK_YEAR 		= 14,
	 VER_PMO_MK_MONTH 		= 12,
	 VER_PMO_MK_DAY 		= 17,
	 VER_PMO_MK_REVISION	= 1,
 };

enum _NormaTmi
 {
	 NORMAL 			= 0,
	 NOT_NORMAL 		= 1,
 };

typedef struct _IntegralParams
{
	unsigned char norma_ibivk;
	unsigned char norma_uart;
	unsigned char norma_mil1553;
	unsigned char norma_input_signals;
	unsigned char norma_software;
	unsigned char norma_resources;
	unsigned char norma_fpga;
}IntegralParams;

typedef struct _SelftestUart
{
	unsigned char uart1_error_code;
	unsigned char uart2_error_code;
}SelftestUart;

typedef struct _SelftestCore1553
{
	unsigned short core1553_error_code;
}SelftestCore1553;

typedef struct _InputSignals
{
	unsigned char norma_1hz;
	unsigned char norma_320ms;
	unsigned char norma_range_bshv;
	unsigned char norma_32_impulse;
}SelftestSignals;

typedef struct _SelftestSoftware
{
	unsigned char pmo_error_code;
}SelftestSoftware;


typedef struct _IbivkResources
{
	unsigned short n_loaded_messages;
	unsigned char load_percent;
}SelftestResources;

typedef struct _Tmi
{
	VersionPmo ver_mk_pmo;
	VersionPmo ver_fpga_pmo;
	Bshv* sys_bshv;
	IntegralParams integral_params;
	SelftestUart selftest_uart;
	SelftestCore1553 selftest_core1553;
	SelftestSignals selftest_input_signals;
	SelftestSoftware selftest_software;
	SelftestResources selftest_resources;
}Tmi;

extern Tmi ibivk_tmi;

#endif /* TMI_STRUCT_H_ */
