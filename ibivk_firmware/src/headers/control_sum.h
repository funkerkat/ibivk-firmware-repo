/*
 * control_sum.h
 *
 *  Created on: 06.11.2014
 *      Author: Ludmila
 */

#ifndef CONTROL_SUM_H_
#define CONTROL_SUM_H_

unsigned int CountControlSum(int data[], unsigned int n);
unsigned int ControlSum(unsigned int n_start, int data[], unsigned int n);

#define CYCLESHIFTLEFT(x)	(((x << 1) + ((x >> 7)&1)) & 0xFF)

#endif /* CONTROL_SUM_H_ */
