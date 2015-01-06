/*
 * bshv.h
 *
 *  Created on: 22.11.2014
 *      Author: Ludmila
 */

#ifndef BSHV_STRUCT_H_
#define BSHV_STRUCT_H_

#include "compare.h"
#include "xBshv.h"

typedef struct _BshvExtention
{
	Bshv myBshv;
	signed int microsecond;
}BshvExtention;

// глобальная переменная -- системное время БШВ в приборе ИБИВК
extern Bshv system_bshv;

// глобальная переменная -- признак коррекции
extern unsigned int bshv_correction;

// глобальная переменная -- БШВ, которое после ближайшего импульса 1 Гц будет установлено как системное
extern Bshv predict_bshv;

enum BshvCorrectionCodes
 {
	 CORRECTIOM_DISABLED 	= 0,
	 CORRECTIOM_PLUS_1S 	= 1,
	 CORRECTIOM_MINUS_1S 	= 2,
 };

enum BshvCorrectionTime
 {
	 CORRECTION_HOUR 	= 2,
	 CORRECTION_MINUTE 	= 59,
 };

enum BshvBoundary
 {
	 BSHV_FOURYEARS_LOWER_BOUNDARY 		= 0,
	 BSHV_FOURYEARS_UPPER_BOUNDARY 		= 15,

	 BSHV_DAY_LOWER_BOUNDARY 			= 1,
	 BSHV_DAY_UPPER_BOUNDARY 			= 1461,

	 BSHV_HOUR_LOWER_BOUNDARY 			= 0,
	 BSHV_HOUR_UPPER_BOUNDARY 			= 23,

	 BSHV_MINUTE_LOWER_BOUNDARY 		= 0,
	 BSHV_MINUTE_UPPER_BOUNDARY 		= 59,

	 BSHV_SECOND_LOWER_BOUNDARY 		= 0,
	 //BSHV_SECOND_UPPER_BOUNDARY 		= 59,
	 BSHV_SECOND_UPPER_BOUNDARY 		= 60,

	 BSHV_MICROSECOND_LOWER_BOUNDARY 	= 0,
	 BSHV_MICROSECOND_UPPER_BOUNDARY 	= 999999,
 };

// Прототипы функций
result_type CompareBshv(Bshv* old_bshv, Bshv* new_bshv);

#endif /* BSHV_STRUCT_H_ */
