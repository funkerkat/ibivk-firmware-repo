/*
 * bus_controller_struct.h
 *
 *  Created on: 28.11.2014
 *      Author: Ludmila
 */

#ifndef BUS_CONTROLLER_STRUCT_H_
#define BUS_CONTROLLER_STRUCT_H_

typedef struct ControlWordStruct {
	unsigned int Opcode;
	unsigned int Retry;
	unsigned int CHAB;
	unsigned int RTRT;
	unsigned int ConditionCode;
	unsigned int BAME;
}ControlWordStruct;

enum Core1553_Opcodes
 {
	 END_OF_LIST		= 0x00,
	 EXECUTE_CONTINUE	= 0x04,
 };

enum Core1553_ControlWord
 {
	Retry	= 0x03,		// Retry Number
	CHAB	= 0x1,		// Channel A/B:			Selects bus A as the primary bus
	RTRT = 0,			// RT-to-RT Transfer	Should transmit CommandWord2
	ConditionCode = 0,	// Read Only			Response from RT.
	BAME = 0,			// Read-only			Protocol Message Error in RT response
 };

typedef struct CommandBlock
{
	volatile unsigned short ControlWord;
	volatile unsigned short CommandWord1;
	volatile unsigned short CommandWord2;
	volatile unsigned short DataPointer;
	volatile unsigned short StatusWord1;
	volatile unsigned short StatusWord2;
	volatile unsigned short BranchAddress;
	volatile unsigned short Timer;
}CommandBlock;

typedef struct EntryCore1553
{
	CommandBlock cmd_block_1;
	CommandBlock cmd_block_2;
	unsigned short data_words[32];
	unsigned int reg_leon;
	unsigned int reg_core1553;
}EntryCore1553;

void Load1553MessageF1(EntryCore1553* entry, unsigned short cw, unsigned short data_words[]);
void Load1553MessageF2(EntryCore1553* entry, unsigned short cw);

#endif /* BUS_CONTROLLER_STRUCT_H_ */
