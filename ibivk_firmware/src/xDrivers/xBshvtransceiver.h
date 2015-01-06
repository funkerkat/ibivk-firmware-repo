/*
 * xBshvtransceiver.h
 *
 *  Created on: 06.01.2015
 *      Author: Ludmila
 */

#ifndef XBSHVTRANSCEIVER_H_
#define XBSHVTRANSCEIVER_H_

#define INTERNAL	0
#define EXTERNAL	1


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// #################### ��������� ��������� ������������ IP CORE: ####################
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define FPGA_BASE_ADDRESS			0x20000000		// �������� ������������ ����
#define BSHV_TRANSCEIVER_ADDRESS	0x0000100		// ������� ����� IP CORE BSHV_TRANSCEIVER
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// #########################  ������������ IP CORE (CONFIG): #########################
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ��������������� �������� �������� ������� IP Core
#define BSHV_TRANSCEIVER_CONFIG_PRESCALER_VALUE		23

// ����� ��������� ������������ �������� 1��, ��0, ��1, ��-���, ��-���
#define BSHV_TRANSCEIVER_CONFIG_SOURCE_VALUE		INTERNAL	// (INTERNAL / EXTERNAL)

//����� ��������� �������� ������� IP Core (����������/�������)
#define BSHV_TRANSCEIVER_CONFIG_REF_CLK_VALUE		INTERNAL	// (INTERNAL / EXTERNAL)

// �������������� �������� ��� ������ � ������� CONFIG:
#define BSHV_TRANSCEIVER_CONFIG_VALUE 		( (BSHV_TRANSCEIVER_CONFIG_PRESCALER_VALUE<<8) | (BSHV_TRANSCEIVER_CONFIG_SOURCE_VALUE<<5) | (BSHV_TRANSCEIVER_CONFIG_REF_CLK_VALUE<<4) )
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// #######################  �������� IP CORE BSHV_TRANSCEIVER: #######################
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define BSHVTRANSCEIVER_BASE_ADDRESS		(FPGA_BASE_ADDRESS + BSHV_TRANSCEIVER_ADDRESS)

#define REG_BSHVTRANSCEIVER_TEST			(BSHVTRANSCEIVER_BASE_ADDRESS + 0x00)	// ������� ������������
#define REG_BSHVTRANSCEIVER_CONFIG			(BSHVTRANSCEIVER_BASE_ADDRESS + 0x04)	// ���������������� �������
#define REG_BSHVTRANSCEIVER_COM				(BSHVTRANSCEIVER_BASE_ADDRESS + 0x08)	// ��������� �������
#define REG_BSHVTRANSCEIVER_SET_TIME		(BSHVTRANSCEIVER_BASE_ADDRESS + 0x0C)	// ������� ���������� �������� ���� ���
#define REG_BSHVTRANSCEIVER_GET_TIME		(BSHVTRANSCEIVER_BASE_ADDRESS + 0x10)	// ������� �������� �������� ���� ���
#define REG_BSHVTRANSCEIVER_TM				(BSHVTRANSCEIVER_BASE_ADDRESS + 0x14)	// ��������������� �������
#define REG_BSHVTRANSCEIVER_VERSION			(BSHVTRANSCEIVER_BASE_ADDRESS + 0x18)	// ������� ������ ���
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// #######################  ������� IP CORE BSHV_TRANSCEIVER: #######################
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define BSHV_TRANSCEIVER_SOFTWARE_RESET()	{ *((int*)(REG_BSHVTRANSCEIVER_CONFIG)) = 0x01; asm("nop"); asm("nop"); asm("nop"); }

// ----- ������� (������� COM) -----
#define COMMAND_MASK_NF_SEC		13
#define COMMAND_MASK_NF_PRI		12
#define COMMAND_MASK_OC1		11
#define COMMAND_MASK_OC0		10
#define COMMAND_MASK_320MS		9
#define COMMAND_MASK_1HZ		8
#define COMMAND_IMIT_NF_SEC		5
#define COMMAND_IMIT_NF_PRI		4
#define COMMAND_EN_CM			1
#define COMMAND_EN_BSHV			0

#define BSHV_TRANSCEIVER_SET_COMMAND(command_name)						\
{																		\
	unsigned int val = *((int*)(REG_BSHVTRANSCEIVER_COM));				\
	*((int*)(REG_BSHVTRANSCEIVER_COM)) = (val | (1 << command_name)) ;	\
}

#define BSHV_TRANSCEIVER_CLEAR_COMMAND(value)							\
{																		\
	unsigned int val = *((int*)(REG_BSHVTRANSCEIVER_COM));				\
	*((int*)(REG_BSHVTRANSCEIVER_COM)) = (val & (~(1 << value)));		\
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#endif /* XBSHVTRANSCEIVER_H_ */
