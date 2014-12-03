/*
 * hertz.c
 *
 *  Created on: 03.12.2014
 *      Author: Ludmila
 */

#include "xSystem.h"
#include "bshv.h"
#include "node_bshv.h"


void Execute1553()
{


}

void HertzHandler()
{
	static int needToClear = FALSE;

	NodeBshv* this = node_bshv_start;	// ��������� � ������ �������� ���
	if (this==NULL){ return; }			// ��������� �����
	this = this->next;					// ���������� ������ (����������) ������� ������



	while(this)
	{
		result_type res = CompareBshv(&(this->myBshv), &system_bshv);

		if (res == Equal)
		{
			// ������ ��������� ����, ��������������� �� ������� �������
			Execute1553();
			break;
		}
		else if(res == FirstValueIsLess)
		{
			// ������ �������� ���������� ������, ���������� ������� ����� ������ ���������
			needToClear = TRUE;
			// ���������� ����� ����������� �������� � ������
			this = this->next;
		}
		else if(res == FirstValueIsGreater)
		{
			// �� ������� ������� �� ������������� ������ ���������
			// �������� ��������� �������
			break;
		}
		else
		{
			return; // ��������� �����
		}

	}

	// ������� ���������� ������ (���������� ��������, �������� ��� �������� ���������� �������)
	if (needToClear == TRUE)
	{
		// �������� ���������� ��������
	}





	// ����� �� ������, ����������� ������� ���
	/*
	NodeBshv* this = node_bshv_start;		// ��������� �� ������� ������� ������
	while(this)
	{
		this = this->next;
	}
*/

}
