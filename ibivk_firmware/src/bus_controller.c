/*
 * bus_controller.c
 *
 *  Created on: 28.11.2014
 *      Author: Ludmila
 */

#include "xMIL1553BC.h"
#include "bus_controller.h"

unsigned int command_block[COMMAND_BLOCK_SIZE_ULONG*2];

unsigned int data_test[16];

static unsigned int MakeControlWord(unsigned int opcode)
{
	ControlWordStruct ControlWord;

	ControlWord.Opcode			= opcode;	// Opcode
	ControlWord.Retry			= 0x01;		// Retry Number
	ControlWord.CHAB			= 0x1;		// Channel A/B:			Selects bus A as the primary bus
	ControlWord.RTRT 			= 0;		// RT-to-RT Transfer	Should transmit CommandWord2
	ControlWord.ConditionCode 	= 0;		// Read Only			Response from RT.
	ControlWord.BAME 			= 0;		// Read-only			Protocol Message Error in RT response
	return (ControlWord.Opcode<<12)|(ControlWord.Retry<<10)|(ControlWord.CHAB<<9)|(ControlWord.RTRT<<8)|(ControlWord.ConditionCode<<1)|(ControlWord.BAME<<0);
}

void CommandBlockInit()
{
	BusControllerCommandBlock BC_command_block_1;
	BC_command_block_1.ValueControlWord	= MakeControlWord(EXECUTE_CONTINUE);
	BC_command_block_1.ValueCommandWord1 = 0;
	BC_command_block_1.ValueCommandWord2 = 0;
	BC_command_block_1.ValueDataPointer	= 0;
	BC_command_block_1.ValueStatusWord1	= 0;
	BC_command_block_1.ValueStatusWord2	= 0;
	BC_command_block_1.ValueBranchAddress = 0;
	BC_command_block_1.ValueTimer = 0;

	BusControllerCommandBlock BC_command_block_2;
	BC_command_block_2.ValueControlWord	= MakeControlWord(END_OF_LIST);
	BC_command_block_2.ValueCommandWord1 = 0;
	BC_command_block_2.ValueCommandWord2 = 0;
	BC_command_block_2.ValueDataPointer	= 0;
	BC_command_block_2.ValueStatusWord1	= 0;
	BC_command_block_2.ValueStatusWord2	= 0;
	BC_command_block_2.ValueBranchAddress = 0;
	BC_command_block_2.ValueTimer = 0;

	command_block[0]  = (BC_command_block_1.ValueControlWord	<<16) | (BC_command_block_1.ValueCommandWord1	<<0);
	command_block[1]  = (BC_command_block_1.ValueCommandWord2	<<16) | (BC_command_block_1.ValueDataPointer	<<0);
	command_block[2]  = (BC_command_block_1.ValueStatusWord1	<<16) | (BC_command_block_1.ValueStatusWord2	<<0);
	command_block[3]  = (BC_command_block_1.ValueBranchAddress	<<16) | (BC_command_block_1.ValueTimer			<<0);

	command_block[4]  = (BC_command_block_2.ValueControlWord	<<16) | (BC_command_block_2.ValueCommandWord1	<<0);
	command_block[5]  = (BC_command_block_2.ValueCommandWord2	<<16) | (BC_command_block_2.ValueDataPointer	<<0);
	command_block[6]  = (BC_command_block_2.ValueStatusWord1	<<16) | (BC_command_block_2.ValueStatusWord2	<<0);
	command_block[7]  = (BC_command_block_2.ValueBranchAddress	<<16) | (BC_command_block_2.ValueTimer			<<0);

}

void CommandBlockUpdate(unsigned int cw, unsigned int data_pointer)
{
	command_block[0] = command_block[0] | cw;
	command_block[1] = data_pointer;
}


void Test1553Core()
{
	CommandBlockInit();

	unsigned int x1 = (unsigned int)command_block;
	unsigned int x2 = (unsigned int)(&command_block);


	CORE1553_INIT(command_block);

	CommandBlockUpdate(0x0821, (unsigned int)data_test);



}
