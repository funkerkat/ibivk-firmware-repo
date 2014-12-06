/*
 * control_sum.c
 *
 *  Created on: 06.11.2014
 *      Author: Ludmila
 */

#define CYCLESHIFTLEFT(x)	(((x << 1) + ((x >> 7)&1)) & 0xFF)

unsigned int CountControlSum(int data[], unsigned int n)
{
	unsigned int cs_calc = 0x55;
	int i;
	for(i=0; i<n; i++)
	{
		cs_calc = (CYCLESHIFTLEFT(cs_calc) + data[i]) & 0xFF;
	}
	return cs_calc;
}


unsigned int ControlSum(unsigned int n_start, int data[], unsigned int n)
{
	unsigned int cs_calc = 0x55;
	int i;
	for(i=0; i<n; i++)
	{
		cs_calc = (CYCLESHIFTLEFT(cs_calc) + data[n_start + i]) & 0xFF;
	}
	return cs_calc;
}
