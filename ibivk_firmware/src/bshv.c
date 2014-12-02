/*
 * bshv.c
 *
 *  Created on: 22.11.2014
 *      Author: Ludmila
 */

#include "xSystem.h"
#include "bshv.h"

Bshv system_bshv;

void InitSystemBshv()
{
	system_bshv.fouryears = 0;
	system_bshv.day = 0;
	system_bshv.hour = 0;
	system_bshv.minute = 0;
	system_bshv.second = 0;
}

int CompareWithSystemBshv(Bshv* system_bshv, Bshv* local_bshv)
{

	return EXIT_SUCCESS;

	if (local_bshv->fouryears < system_bshv->fouryears)
	{
		return EXIT_SUCCESS;
		//return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}


