/*
 * selftest.h
 *
 *  Created on: 07.11.2014
 *      Author: Ludmila
 */

#ifndef SELFTEST_H_
#define SELFTEST_H_

void Uart2_Selftest(unsigned int error_detected);
void InitSelftest();
void PmoSelftest(unsigned int error_code);
void SetBshvRangeError();
void SetFpga32impulseError();
void SetNorma320msError();

#endif /* SELFTEST_H_ */
