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


void LoadCommandBlockAndData()
{
	// Load Data
	int* address_data;
	address_data = (int*)0xA0000400;
	int i;
	int value;
	for (i = 0; i < 32; i++) {
		value = (((2*i)+1)<<16)|(2*i+2);
		*(address_data + 1*i) = value;
	}


	// Load CommandBlock
	#define R		1	// Receive
	#define T		0	// Transmit

	int ValueControlWord	= 0;
	int ValueCommandWord1	= 0;
	int ValueCommandWord2	= 0;
	int ValueDataPointer	= 0;
	int ValueStatusWord1	= 0;
	int ValueStatusWord2	= 0;
	int ValueBranchAddress	= 0;
	int ValueTimer			= 0;

	int* address;
	address = (int*)0xA0000100;


	ValueControlWord	= ResultConrolWord(4);
	ValueCommandWord1	= ResultCommandWord(10, T, 1, 2);
	ValueCommandWord2	= 0;
	ValueDataPointer	= (0x100*2);
	ValueStatusWord1	= 0;
	ValueStatusWord2	= 0;
	ValueBranchAddress	= 0;
	ValueTimer			= 0;
	*(address + 0)  = (ValueControlWord		<<16)|(ValueCommandWord1);
	*(address + 1)  = (ValueCommandWord2	<<16)|(ValueDataPointer);
	*(address + 2)  = (ValueStatusWord1		<<16)|(ValueStatusWord2);
	*(address + 3)  = (ValueBranchAddress	<<16)|(ValueTimer);

	ValueControlWord	= ResultConrolWord(0);
	ValueCommandWord1	= ResultCommandWord(10, T, 1, 2);
	ValueCommandWord2	= 0;
	ValueDataPointer	= (0x100*2);
	ValueStatusWord1	= 0;
	ValueStatusWord2	= 0;
	ValueBranchAddress	= 0;
	ValueTimer			= 0xFFFF;
	*(address + 4)  = (ValueControlWord		<<16)|(ValueCommandWord1);
	*(address + 5)  = (ValueCommandWord2	<<16)|(ValueDataPointer);
	*(address + 6)  = (ValueStatusWord1		<<16)|(ValueStatusWord2);
	*(address + 7)  = (ValueBranchAddress	<<16)|(ValueTimer);

}


void Test1553Core()
{

	CORE1553_INIT();

	EntryCore1553 entry;

	unsigned int p_cmd = (unsigned int)(&(entry.cmd_block_1.ControlWord));
	unsigned int p_cmd_masked = p_cmd & 0xFFFE0000;

	unsigned int p_data = (unsigned int)(&(entry.data_words));

	entry.cmd_block_1.ControlWord = (EXECUTE_CONTINUE<<12)|(Retry<<10)|(CHAB<<9)|(RTRT<<8)|(ConditionCode<<1)|(BAME<<0);
	entry.cmd_block_1.CommandWord1 = 0x0822;
	entry.cmd_block_1.CommandWord2 = 0;
	entry.cmd_block_1.DataPointer = (p_data - p_cmd_masked)/2;
	entry.cmd_block_1.StatusWord1 = 0;
	entry.cmd_block_1.StatusWord2 = 0;
	entry.cmd_block_1.BranchAddress = 0;
	entry.cmd_block_1.Timer = 0;

	entry.cmd_block_2.ControlWord = (END_OF_LIST<<12)|(Retry<<10)|(CHAB<<9)|(RTRT<<8)|(ConditionCode<<1)|(BAME<<0);
	entry.cmd_block_2.CommandWord1 = 0;
	entry.cmd_block_2.CommandWord2 = 0;
	entry.cmd_block_2.DataPointer = 0;
	entry.cmd_block_2.StatusWord1 = 0;
	entry.cmd_block_2.StatusWord2 = 0;
	entry.cmd_block_2.BranchAddress = 0;
	entry.cmd_block_2.Timer = 0;


	int i;
	for (i=0; i<32; i++)
	{
		entry.data_words[i] = (0xAB<<8) + i;
	}

	*((int*)(MIL1553_BASE_ADDRESS + MIL1553_AHB_PAGE_ADDRESS)) 				= p_cmd_masked;
	*((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG08_COMMAND_BLOCK_POINTER)) 	= (p_cmd - p_cmd_masked)/2;


	START_EXECUTION();




	int t = 0;
	t++;


}
