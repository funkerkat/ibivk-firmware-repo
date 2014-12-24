/*
 * selftest.h
 *
 *  Created on: 07.11.2014
 *      Author: Ludmila
 */

#ifndef SELFTEST_H_
#define SELFTEST_H_

void Uart2_ErrorDetected(unsigned int);
void InitSelftest();
void PmoSelftest(unsigned int error_code);
void SetBshvRangeError();
void SetFpga32impulseError();

#endif /* SELFTEST_H_ */
