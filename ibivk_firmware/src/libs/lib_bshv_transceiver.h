/*
 * lib_bshv_transceiver.h
 *
 *  Created on: 27.12.2014
 *      Author: Ludmila
 */

#include "xBshvtransceiver.h"
#include "xBshv.h"
#include "xVersionPmo.h"

#ifndef LIB_BSHV_TRANSCEIVER_H_
#define LIB_BSHV_TRANSCEIVER_H_

#ifndef EXIT_FAILURE
#define	EXIT_FAILURE	1
#endif

#ifndef EXIT_SUCCESS
#define	EXIT_SUCCESS	0
#endif

typedef struct _BshvTransceiverTm
{
	unsigned short OC_COUNT;
	unsigned short OC_COUNT_32;
	unsigned short OC_EQUAL;
	unsigned short OUT_OC1;
	unsigned short OUT_OC0;
	unsigned short OUT_CM;
	unsigned short OUT_NF_SEC;
	unsigned short OUT_NF_PRI;
	unsigned short OUT_1HZ;
	unsigned short OUT_320MS;
}BshvTransceiverTm;

static unsigned int BshvTransceiver_Selftest()
{
	unsigned int write_value = 0x12345678;
	*((int*)(REG_BSHVTRANSCEIVER_TEST)) = write_value;

	unsigned int read_value;
	read_value = *((int*)(REG_BSHVTRANSCEIVER_TEST));

	if (write_value == read_value) { return EXIT_SUCCESS; }
	else { return EXIT_FAILURE; }
}

static void BshvTransceiver_Get_Bshv(Bshv* myBshv)
{
	// Чтение значения ТМ из IP CORE BSHV TRANSCEIVER
	unsigned int x = *((int*)(REG_BSHVTRANSCEIVER_GET_TIME));

	// Декодирование значений БШВ
	enum BSHV_VALUE_LENGTH
	 {
		 FOURYEARS_LENGTH 	= 4,
		 DAY_LENGTH 		= 11,
		 HOUR_LENGTH 		= 5,
		 MINUTE_LENGTH 		= 6,
		 SECOND_LENGTH 		= 6,
	 };

	//#define GET_MASK(value) { (~(0xFFFFFFFF << value)); }
	//int y = GET_MASK(FOURYEARS_LENGTH);

	myBshv->fouryears 	= (x >> (SECOND_LENGTH+MINUTE_LENGTH+HOUR_LENGTH+DAY_LENGTH)) 	& (~(0xFFFFFFFF << FOURYEARS_LENGTH));
	myBshv->day 		= (x >> (SECOND_LENGTH+MINUTE_LENGTH+HOUR_LENGTH)) 				& (~(0xFFFFFFFF << DAY_LENGTH));
	myBshv->hour 		= (x >> (SECOND_LENGTH+MINUTE_LENGTH)) 							& (~(0xFFFFFFFF << HOUR_LENGTH));
	myBshv->minute 		= (x >> (SECOND_LENGTH)) 										& (~(0xFFFFFFFF << MINUTE_LENGTH));
	myBshv->second 		= (x) 															& (~(0xFFFFFFFF << SECOND_LENGTH));
}

static void BshvTransceiver_Get_TM(BshvTransceiverTm* tm)
{
	// Чтение значения ТМ из IP CORE BSHV TRANSCEIVER
	unsigned int x = *((int*)(REG_BSHVTRANSCEIVER_TM));

	// Декодирование значений ТМ
	tm->OC_COUNT 		= (x >> 12)	& 0x3F;
	tm->OC_COUNT_32 	= (x >> 9)	& 0x01;
	tm->OC_EQUAL	 	= (x >> 8)	& 0x01;
	tm->OUT_OC1		 	= (x >> 6)	& 0x01;
	tm->OUT_OC0		 	= (x >> 5)	& 0x01;
	tm->OUT_CM		 	= (x >> 4)	& 0x01;
	tm->OUT_NF_SEC		= (x >> 3)	& 0x01;
	tm->OUT_NF_PRI		= (x >> 2)	& 0x01;
	tm->OUT_1HZ		 	= (x >> 1)	& 0x01;
	tm->OUT_320MS		= (x >> 0)	& 0x01;
}

static void BshvTransceiver_Get_VersionPmo(VersionPmo* pmo_bshv_transceiver)
{
	// Чтение значения ТМ из IP CORE BSHV TRANSCEIVER
	unsigned int x = *((int*)(REG_BSHVTRANSCEIVER_VERSION));

	// Декодирование значений версии ПМО IP CORE BSHV TRANSCEIVER
	enum BSHVTRANSCEIVER_VER_PMO_VALUE_LENGTH
	 {
		 YEAR_LENGTH 		= 8,
		 MONTH_LENGTH 		= 8,
		 DAY_LENGTH 		= 8,
		 REVISION_LENGTH 	= 8,
	 };

	pmo_bshv_transceiver->pmo_year 		= (x >> (MONTH_LENGTH+DAY_LENGTH+REVISION_LENGTH)) 	& (~(0xFFFFFFFF << YEAR_LENGTH));
	pmo_bshv_transceiver->pmo_month 	= (x >> (DAY_LENGTH+REVISION_LENGTH)) 				& (~(0xFFFFFFFF << MONTH_LENGTH));
	pmo_bshv_transceiver->pmo_day 		= (x >> (REVISION_LENGTH)) 							& (~(0xFFFFFFFF << DAY_LENGTH));
	pmo_bshv_transceiver->pmo_revision 	= (x) 												& (~(0xFFFFFFFF << REVISION_LENGTH));
}

#endif /* LIB_BSHV_TRANSCEIVER_H_ */
