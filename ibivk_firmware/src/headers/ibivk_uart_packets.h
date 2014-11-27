/*
 * ibivk_uart_packets.h
 *
 *  Created on: 29.10.2014
 *      Author: Ludmila
 */

#ifndef IBIVK_UART_PACKETS_H_
#define IBIVK_UART_PACKETS_H_

#include "shift_reg.h"
#include "bshv.h"

#define PACKET_LENGTH_MIN_VALUE		(2)			// ����������� ���-�� ���� � ������ = 2  (�� � ��)

void SendDiagnosticAnswer(unsigned int);
//void SendDiagnosticAnswer(unsigned int error_code, unsigned int received_cs, unsigned int received_id);
void UartDecodePackets(Byte*, unsigned int);
int LoadPacketF1(Bshv* b, unsigned int cw, unsigned int dw[], unsigned int n_data);
int LoadPacketF2(Bshv* b, unsigned int cw);

void DiagnosticAnswer(unsigned int received_cs, unsigned int received_id, unsigned int error_code);
void UartDecode(unsigned int data[], unsigned int n);

enum HEAD_VALUES
 {
	 HEAD_0 	= 0x42,
	 HEAD_1 	= 0x49,
	 HEAD_2 	= 0x56,
	 HEAD_3 	= 0x4b,
 };

enum PacketFields
 {
	 HEAD_SIZE 						= 4,
	 PACKETLENGTH_SIZE 				= 1,
	 SERVICE_BYTES_SIZE 			= HEAD_SIZE + PACKETLENGTH_SIZE,
	 PACKET_ID_SIZE 				= 1,
	 CONTROLSUM_SIZE 				= 1,
 };

enum PacketIDs_PC_to_IBIVK
 {
	 ID_PACKET_BC_TO_RT 		= 0x20,
	 ID_PACKET_RT_TO_BC 		= 0x51,
 };

enum PacketIDs_IBIVK_to_PC
 {
	 ID_DIAGNOSTIC_ANSWER 		= 0x01,
 };

enum Packets_Data_Bytecount
 {
	 bshv_byte_size						= (9),
	 commandword_byte_size				= (1*2),
	 BYTECOUNT_PACKET_BC_TO_RT_LOWER 	= (bshv_byte_size + commandword_byte_size + 1*2),
	 BYTECOUNT_PACKET_BC_TO_RT_UPPER 	= (bshv_byte_size + commandword_byte_size + 32*2),
	 BYTECOUNT_PACKET_RT_TO_BC 			= (bshv_byte_size + 1*2),
	// BYTECOUNT_DIAGNOSTIC_ANSWER 		= (1),
	 BYTECOUNT_DIAGNOSTIC_ANSWER 		= (3),
 };

enum DiagnosticAnswerPacketLength
 {
	 DIAGNOSTIC_ANSWER_TOTAL_LENGTH = (HEAD_SIZE + PACKETLENGTH_SIZE + PACKET_ID_SIZE + BYTECOUNT_DIAGNOSTIC_ANSWER + CONTROLSUM_SIZE),
 };

enum DiagnosticAnswerErrorCodes
 {
	 DIAGNOSTIC_ANSWER_NO_ERRORS			= 0x00,			// "����� ������� �������� � �����"
	 DIAGNOSTIC_ANSWER_ERROR_CS				= 0x01,			// "������: ������������ ����������� �����"
	 DIAGNOSTIC_ANSWER_ERROR_ID				= 0x02,			// "������: �������� ������������� ������"
	 DIAGNOSTIC_ANSWER_ERROR_ALGORITHM 		= 0x03,			// "������: �������� �������� ���������� ����"
	 DIAGNOSTIC_ANSWER_ERROR_DIRECTION		= 0x04,			// "������: �������� ����������� �������� ������"
	 DIAGNOSTIC_ANSWER_ERROR_CW_WORDCOUNT	= 0x05,			// "������: ������������ ���������� ��, ��������� � ��, � ����������� � �����"
	 DIAGNOSTIC_ANSWER_ERROR_BSHV_BOUNDARY	= 0x06,			// "������: �������� ��� ������� �� ���������� �������"
	 DIAGNOSTIC_ANSWER_ERROR_BSHV_TOO_LATE	= 0x07,			// "������: �����, ��������� � ������, ��������� �������� ��������"
	 DIAGNOSTIC_ANSWER_ERROR_POINTERS		= 0x08,			// "������ ���������"
 };

#endif /* IBIVK_UART_PACKETS_H_ */
