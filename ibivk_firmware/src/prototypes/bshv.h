/*
 * bshv.h
 *
 *  Created on: 06.12.2014
 *      Author: Ludmila
 */

#ifndef BSHV_H_
#define BSHV_H_

void InitSystemBshv();
result_type CompareBshv(Bshv* first_value, Bshv* second_value);
void CopyBshv(Bshv* copyFrom, Bshv* copyTo);
void IncrementBshv(Bshv* myBshv, unsigned int second_value);
unsigned int ValidateBshvBoundaries(Bshv* myBshv);
void PredictNextBshvValue(Bshv* myBshv);

#endif /* BSHV_H_ */
