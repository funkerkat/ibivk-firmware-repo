/*
 * bus_controller.c
 *
 *  Created on: 28.11.2014
 *      Author: Ludmila
 */

// ���������� ��� ������ � ����������
#include "xMIL1553BC.h"

// ���������� �������� �������
#include "bus_controller_struct.h"

void Load1553MessageF1(EntryCore1553* entry, unsigned short cw, unsigned short data_words[])
{
	// ��������� ���������
	unsigned int p_cmd = (unsigned int)(&(entry->cmd_block_1.ControlWord));
	unsigned int p_cmd_masked = p_cmd & 0xFFFE0000;
	unsigned int p_data = (unsigned int)(&(entry->data_words));

	// ��������� ���������
	entry->reg_leon = p_cmd_masked;
	entry->reg_core1553 = (p_cmd - p_cmd_masked)/2;

	// �������� ��������� ����� � ������
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

	// ��������� �� ���������� ����� ���������� ���� ������
	unsigned int n_data_words = cw & 0x1F;
	if (n_data_words == 0) { n_data_words = 32; }

	// �������� ������ � ������
	int i;
	for (i=0; i<n_data_words; i++)
	{
		entry->data_words[i] = data_words[i];
	}
}


void Load1553MessageF2(EntryCore1553* entry, unsigned short cw)
{
	// ��������� ���������
	unsigned int p_cmd = (unsigned int)(&(entry->cmd_block_1.ControlWord));
	unsigned int p_cmd_masked = p_cmd & 0xFFFE0000;
	unsigned int p_data = (unsigned int)(&(entry->data_words));

	// ��������� ���������
	entry->reg_leon = p_cmd_masked;
	entry->reg_core1553 = (p_cmd - p_cmd_masked)/2;

	// �������� ��������� ����� � ������
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

}
