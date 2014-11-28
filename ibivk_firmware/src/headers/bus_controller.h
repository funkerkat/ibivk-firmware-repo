/*
 * bus_controller.h
 *
 *  Created on: 28.11.2014
 *      Author: Ludmila
 */

#ifndef BUS_CONTROLLER_H_
#define BUS_CONTROLLER_H_

#define COMMAND_BLOCK_SIZE_USHORT	(8)
#define COMMAND_BLOCK_SIZE_ULONG	(COMMAND_BLOCK_SIZE_USHORT/2)

enum BusControllerOpcodes
 {
	 END_OF_LIST		= 0x00,
	 EXECUTE_CONTINUE	= 0x04,
 };

typedef struct BusControllerCommandBlock
{
	unsigned int ValueControlWord;
	unsigned int ValueCommandWord1;
	unsigned int ValueCommandWord2;
	unsigned int ValueDataPointer;
	unsigned int ValueStatusWord1;
	unsigned int ValueStatusWord2;
	unsigned int ValueBranchAddress;
	unsigned int ValueTimer;
}BusControllerCommandBlock;

typedef struct ControlWordStruct {
	unsigned int Opcode;
	unsigned int Retry;
	unsigned int CHAB;
	unsigned int RTRT;
	unsigned int ConditionCode;
	unsigned int BAME;
}ControlWordStruct;


#endif /* BUS_CONTROLLER_H_ */
