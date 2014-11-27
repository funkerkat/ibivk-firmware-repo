/*
 * bshv.h
 *
 *  Created on: 22.11.2014
 *      Author: Ludmila
 */

#ifndef BSHV_H_
#define BSHV_H_

/*
typedef struct Bshv
{
	unsigned int fouryears;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
	unsigned int microsecond;
}Bshv;
*/

typedef struct Bshv
{
	signed int fouryears;
	signed int day;
	signed int hour;
	signed int minute;
	signed int second;
	signed int microsecond;
}Bshv;

extern Bshv system_bshv;

int CompareWithSystemBshv(Bshv*, Bshv*);

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
	 BSHV_SECOND_UPPER_BOUNDARY 		= 59,
 };

#endif /* BSHV_H_ */
