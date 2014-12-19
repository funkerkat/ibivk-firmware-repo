/*
 * tmi_struct.h
 *
 *  Created on: 19.12.2014
 *      Author: Ludmila
 */

#ifndef TMI_STRUCT_H_
#define TMI_STRUCT_H_

#include "bshv_struct.h"

enum _VersionPmoData
 {
	 VER_PMO_MK_YEAR 	= 14,
	 VER_PMO_MK_MONTH 	= 12,
	 VER_PMO_MK_DAY 	= 17,
 };

enum _NormaTmi
 {
	 NORMAL 			= 1,
	 NOT_NORMAL 		= 0,
 };

typedef struct _VersionPmo
{
	unsigned char pmo_mk_year;
	unsigned char pmo_mk_month;
	unsigned char pmo_mk_day;
}VersionPmo;

typedef struct _IbivkSelftest
{
	unsigned char norma_ibivk;
	unsigned char norma_uart;
	unsigned char norma_mil1553;
	unsigned char norma_1hz;
	unsigned char norma_320ms;
	unsigned char norma_system_bshv;
	unsigned char algorithm_error_code;
}IbivkSelftest;

typedef struct _IbivkResources
{
	unsigned short n_loaded_messages;
	unsigned char load_percent;
}IbivkResources;

typedef struct _SelftestUart
{
	unsigned char uart1_error_code;
	unsigned char uart2_error_code;
}SelftestUart;

typedef struct _SelftestCore1553
{
	unsigned short core1553_error_code;
}SelftestCore1553;

typedef struct _Tmi
{
	VersionPmo ver_pmo;
	IbivkSelftest ibivk_selftest;
	Bshv* sys_bshv;
	IbivkResources ibivk_res;
	SelftestUart uart_selftest;
	SelftestCore1553 core1553_selftest;
}Tmi;

extern Tmi ibivk_tmi;

#endif /* TMI_STRUCT_H_ */
