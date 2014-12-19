/*
 * list_transmit_struct.h
 *
 *  Created on: 18.12.2014
 *      Author: Ludmila
 */

//#include "data1553_struct.h"
#include "data_ibivk_to_pc_f1_struct.h"
#include "data_ibivk_to_pc_f2_struct.h"
#include "data_diagnostic_answer_struct.h"

typedef union _UartTransmitDataTypes
{
	//Data1553* data1553_obj;
	Ibivk_to_pc_f1 data_ibivk_to_pc_f1;
	Ibivk_to_pc_f2 data_ibivk_to_pc_f2;
	DataDiagnosticAnswer data_diagnostic_answer;
}UartTransmitDataTypes;

typedef struct _ListTransmit
{
	unsigned short packet_id;
	UartTransmitDataTypes data;
	struct _ListTransmit* next;
}ListTransmit;

extern ListTransmit* head_list_transmit;
