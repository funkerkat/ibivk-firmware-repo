/*
 * bus_controller.c
 *
 *  Created on: 28.11.2014
 *      Author: Ludmila
 */

#include "xMIL1553BC.h"
#include "bus_controller.h"

/*
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
*/



void Load1553MessageF1(EntryCore1553* entry, unsigned short cw, unsigned short data_words[])
{
	// Вычислить указатели
	unsigned int p_cmd = (unsigned int)(&(entry->cmd_block_1.ControlWord));
	unsigned int p_cmd_masked = p_cmd & 0xFFFE0000;
	unsigned int p_data = (unsigned int)(&(entry->data_words));

	// Сохранить указатели
	entry->reg_leon = p_cmd_masked;
	entry->reg_core1553 = (p_cmd - p_cmd_masked)/2;

	// Заложить командные блоки в память
	entry->cmd_block_1.ControlWord = (EXECUTE_CONTINUE<<12)|(Retry<<10)|(CHAB<<9)|(RTRT<<8)|(ConditionCode<<1)|(BAME<<0);
	entry->cmd_block_1.CommandWord1 = cw;
	entry->cmd_block_1.CommandWord2 = 0;
	entry->cmd_block_1.DataPointer = (p_data - p_cmd_masked)/2;
	entry->cmd_block_1.StatusWord1 = 0;
	entry->cmd_block_1.StatusWord2 = 0;
	entry->cmd_block_1.BranchAddress = 0;
	entry->cmd_block_1.Timer = 0;

	entry->cmd_block_2.ControlWord = (END_OF_LIST<<12)|(Retry<<10)|(CHAB<<9)|(RTRT<<8)|(ConditionCode<<1)|(BAME<<0);
	entry->cmd_block_2.CommandWord1 = 0;
	entry->cmd_block_2.CommandWord2 = 0;
	entry->cmd_block_2.DataPointer = 0;
	entry->cmd_block_2.StatusWord1 = 0;
	entry->cmd_block_2.StatusWord2 = 0;
	entry->cmd_block_2.BranchAddress = 0;
	entry->cmd_block_2.Timer = 0;

	// Вычислить из командного слова количество слов данных
	unsigned int n_data_words = cw & 0x1F;

	// Заложить данные в память
	int i;
	for (i=0; i<n_data_words; i++)
	{
		entry->data_words[i] = data_words[i];
	}
}


void Test1553Core()
{

	CORE1553_INIT();


	unsigned short cw = 0x0823;
	int i;
	unsigned short dw[32];
	for (i=0; i<32; i++)
	{
		dw[i] = (0xFF<<8) + i;
	}


	EntryCore1553 entry;
	Load1553MessageF1(&entry, cw, dw);

	/*
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
*/

	START_EXECUTION();




	int t = 0;
	t++;


}
