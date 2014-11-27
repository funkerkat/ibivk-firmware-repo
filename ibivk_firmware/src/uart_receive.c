/*
 * uart_receive.c
 *
 *  Created on: 24.10.2014
 *      Author: Ludmila
 */
#include "xSystem.h"
#include "xUart.h"
#include "xIrqmp.h"
#include "ibivk_uart_packets.h"
#include "shift_reg.h"
#include "control_sum.h"

Byte byteArray[SHIFTREG_MAX_LENGTH];

static int ControlSumValidation(Byte* adrData, unsigned int n)
{
	if (n > SHIFTREG_MAX_LENGTH)
	{
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_POINTERS);
		return EXIT_FAILURE;
	}	// ��������� �����

	// ������������ ������� ������ ��� �������� � ������� ������� ����������� ����� CountControlSum()
	int dataArray[SHIFTREG_MAX_LENGTH];
	int i;
	for(i=0; i<n; i++)
	{
		dataArray[i] = adrData->value;
		adrData = adrData->next;
		if (adrData == NULL)
		{
			SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_POINTERS);
			return EXIT_FAILURE;
		}		// ��������� �����
	}

	// ���������� �������� � ������ ����������� �����
	unsigned int cs_received = adrData->value;

	// ������ ����������� �����
	unsigned int cs_calc = CountControlSum(dataArray, n);

	if (cs_calc == cs_received)
	{
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}


static void IncrementAddress(Byte **p_this)
{
	Byte* start = (Byte*)(&byteArray);
	Byte* end = start + (SHIFTREG_MAX_LENGTH-1);
	if (*p_this < end)
	{
		(*p_this)++;
	}
	else
	{
		(*p_this) = start;
	}
}

static int CountBytesInShiftReg(Byte* adrHead)
{
	Byte* this = adrHead;
	int n = 0;
	while(this)
	{
		n++;
		this = this->next;
	}
	return n;
}


static void ShiftLeft(ShiftRegPointers* pointer, unsigned int n)
{
	// ���������� � ������� ������ ������: ��������� ��������� �� ������ ���������� ��������
	(*pointer).reg_pointer = NULL;

	// ������ ������� ���������� ��������
	if (n >= CountBytesInShiftReg((*pointer).adrHead))
	{
		(*pointer).adrHead = NULL;
		(*pointer).adrThis = NULL;
		(*pointer).adrPrev = NULL;
		return;
	}

	// �������� n ���� �� ���������� ��������:
	Byte* adr = (*pointer).adrHead;

	// 1. ��������� n ������ ���� � ��������� ��������
	int i;
	for(i=0; i<n; i++) { adr = adr->next; }

	// 2. ����������� ��������� ������ ���������� �������� �� n-� ����
	(*pointer).adrHead = adr;
}

static int SearchHead(Byte* adr_head, unsigned int n)
{
	if (n > HEAD_SIZE)
	{
		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_POINTERS);
		return EXIT_FAILURE;
	}	// ��������� �����

	const unsigned int headValue[HEAD_SIZE] = {HEAD_0, HEAD_1, HEAD_2, HEAD_3};
	unsigned int head[HEAD_SIZE];

	Byte* adrScroll = adr_head;

	int i;
	for (i=0; i<n; i++)
	{
		head[i] = adrScroll->value;
		adrScroll = adrScroll->next;
		if (head[i] != headValue[i]) { return EXIT_FAILURE; }
	}

	return EXIT_SUCCESS;
}


static void SearchPacket(ShiftRegPointers* pointer)
{

    static unsigned int info_bytes_size = 0;
    unsigned int n_pointer = (*pointer).reg_pointer + 1;

    // ----- ����� ���������: -----
    if (n_pointer <= HEAD_SIZE)
	{
		int result = SearchHead((*pointer).adrHead, n_pointer);
		if (result == EXIT_FAILURE)
		{
			ShiftLeft(pointer, 1);
		}
		else
		{
    		(*pointer).reg_pointer++;
		}
	}
    // ----- ���������� ���-�� ���� �������� ���������� N � ������: -----
    else if (n_pointer == SERVICE_BYTES_SIZE)
    {
    	// ���������� ����� ������ N (����)
    	Byte* adr_nbytes = (*pointer).adrHead; int i; for (i=0; i<HEAD_SIZE; i++) { adr_nbytes = adr_nbytes->next; }
    	info_bytes_size = adr_nbytes->value;

    	// �������� ����������� �������� N
    	if (info_bytes_size < PACKET_LENGTH_MIN_VALUE)
    	{
			ShiftLeft(pointer, SERVICE_BYTES_SIZE);
    	}
    	else
    	{
    		(*pointer).reg_pointer++;
    	}
    }
    // ----- ����� ���� �������� ���������� ������: -----
    else
    {
    	unsigned int n = CountBytesInShiftReg((*pointer).adrHead);
    	if (n <= SERVICE_BYTES_SIZE)
    	{
    		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_POINTERS);
    		return;
    	}	// ��������� ����� (��������� �� ������)

		// ���������� ����� ���� ������:
    	if ((n-SERVICE_BYTES_SIZE) < info_bytes_size)
    	{
    		(*pointer).reg_pointer++;
    	}
		// ����� ������ ��������:
    	else if ((n-SERVICE_BYTES_SIZE) >= info_bytes_size)
    	{
        	// 1. ��������� ����������� �����
        	int result_cs = ControlSumValidation((*pointer).adrHead, (SERVICE_BYTES_SIZE + info_bytes_size - 1));

        	// 2. �� ���������� ����������� �����:
        	if (result_cs == EXIT_SUCCESS)
        	{
        		// ���������� ����� ������ �������������� ����
            	Byte* adr_data = (*pointer).adrHead; int i; for (i=0; i<SERVICE_BYTES_SIZE; i++) { adr_data = adr_data->next; }

        		// ������������ �������������� ����� (����� 1 ����� ����������� �����)
        		UartDecodePackets(adr_data, info_bytes_size-1);

        		// ��������� �������� ��������� ������� �� ������ ��������� ������
    			//ShiftLeft(pointer, (SERVICE_BYTES_SIZE+info_bytes_size));
        		ShiftLeft(pointer, n);
        	}
        	else
        	{
        		// ������ ���������������� ������ � ����� "������ ����������� �����"
        		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_CS);

        		// ���������� ����� ��������� � ��������� ������� ����
    			ShiftLeft(pointer, 1);
        	}
    	}
		// ���-�� �������� �������������� ���� ��������� ��������� ���-�� ������ � ������� ������ (������):
    	else
    	{
    		/*
    		// ������ ���������������� ������ � ����� "������: �������� �������� ���������� ����"
    		SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_BYTECOUNT);

    		// ���������� ����� ��������� � ��������� ������� ����
			ShiftLeft(pointer, 1);
			*/

    	}
    }
}



void AddByteToShiftReg(unsigned int value)
{
	static ShiftRegPointers pointers = {NULL, NULL, NULL, NULL};

	// ��������� ������� ����
	// (��� ������ ���������� ����� � ������ �������, ����� ������� ������ ������ ������� ��� �����)
	if (pointers.adrHead == NULL)
	{
		pointers.adrHead = (Byte*)&byteArray;
		pointers.adrThis = pointers.adrHead;
	}

	// �������� ���� � ������� ������
	pointers.adrThis->value = value;
	pointers.adrThis->next = NULL;

	// ������� �������������� ������ (���� �������) � �������
	if (pointers.adrPrev != NULL) { pointers.adrPrev->next = pointers.adrThis; }
	pointers.adrPrev = pointers.adrThis;

	// ��������� ����� ��� ���������� �����
	IncrementAddress(&pointers.adrThis);

	// ������������� ������ � ��������� ��������:
    int n = CountBytesInShiftReg(pointers.adrHead);
    if (n == NULL)
    {
    	SendDiagnosticAnswer(DIAGNOSTIC_ANSWER_ERROR_POINTERS);
    	return;
    }		// ��������� �����

    while (pointers.reg_pointer < n)
    {
    	SearchPacket(&pointers);
        n = CountBytesInShiftReg(pointers.adrHead);

        int t = 0;
        t++;
    }

    int t = 0;
    t++;
}



void UartSettings()
{
	UART2_SET_BAUDRATE(115200);
	UART2_SET_CONTROL_REG();

    // ���������� ����������
	IRQMP_INIT();
int i = 1;

	while(1)
	{
		int wait = 2000;
		while (wait) { wait--; }

		//UART2_SEND_BYTE(i);
		//SendDiagnosticAnswer(i);
		i++;
	}

}


