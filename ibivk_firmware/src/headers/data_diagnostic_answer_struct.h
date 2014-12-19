/*
 * data_diagnostic_answer_struct.h
 *
 *  Created on: 17.12.2014
 *      Author: Ludmila
 */

#ifndef DATA_DIAGNOSTIC_ANSWER_STRUCT_H_
#define DATA_DIAGNOSTIC_ANSWER_STRUCT_H_

typedef struct _DataDiagnosticAnswer
{
	unsigned short receiver_id;
	unsigned short receiver_cs;
	unsigned short code_error;
}DataDiagnosticAnswer;

#endif /* DATA_DIAGNOSTIC_ANSWER_STRUCT_H_ */
