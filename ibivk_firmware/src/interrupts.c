/*
 * interrupts.c
 *
 *  Created on: 17.10.2014
 *      Author: Ludmila
 */

// библиотеки для работы с периферией
#include "xUart.h"
#include "xIrqmp.h"
#include "xMil1553BC.h"
#include "xTimer.h"
#include "xGrgpio.h"

// библиотеки текущего проекта
#include "bshv_struct.h"
#include "programm_errors.h"
#include "lib_bshv_transceiver.h"

// прототипы функций
#include "bshv.h"
#include "timer.h"
#include "uart_rx.h"
#include "selftest.h"

extern void *catch_interrupt(void func(), int irq);
Bshv predict_bshv;
unsigned int timer1_type;
enum timer1_types
 {
	 NOT_USED	 	= 0,
	 CONTROL_1HZ 	= 1,
	 CONTROL_320MS 	= 2,
 };

// -- UART 1 --
void Handler_irq_2()
{

}

// -- UART 2 --
void Handler_irq_3()
{
	IRQMP_DISABLE();
	unsigned int value = (*UART2_READ_BYTE) & 0xFF;
	AddByteToBuffer(value);
	IRQMP_ENABLE();
}

// -- IRQMP_320MS --
void Handler_irq_4()
{
	// событие произошло -- установить таймер на контроль сигнала "1 Гц"
	//timer1_type = CONTROL_1HZ;
	//TIMER1_START(700);


	// чтение телеметрии IP CORE BSHV TRANSCEIVER
	BshvTransceiverTm tmi;
	BshvTransceiver_Get_TM(&tmi);

	// чтение значения БШВ
	Bshv b;
	BshvTransceiver_Get_Bshv(&b);

	// Проверка значений БШВ на диапазон
	unsigned int result = ValidateBshvBoundaries(&b);
	if (result == EXIT_FAILURE) { SetBshvRangeError(); }

	VersionPmo ver;
	BshvTransceiver_Get_VersionPmo(&ver);

	// спрогнозировать значение следующей секунды
	PredictNextBshvValue(&predict_bshv);
}

// -- TIMER 1 --
void Handler_irq_6()
{
	// контроль сигналов "1 Гц" и "320 мс"
	if (timer1_type == CONTROL_320MS)
	{
		// событие не произошло -- таймер не был сброшен в обработчике "320 мс"
		//SetNorma320msError();
		// установить таймер на контроль сигнала "1 Гц"
		//timer1_type = CONTROL_1HZ;
		//TIMER1_START(700);
	}
	else if (timer1_type == CONTROL_1HZ)
	{
		// событие не произошло -- таймер не был сброшен в обработчике "1 Гц"
		//SetNorma320msError();
		// установить таймер для контроля сигнала "320 мс"
		//timer1_type = CONTROL_320MS;
		//TIMER1_START(320);
	}
	else
	{
		// программный сбой
		PmoSelftest(ALGORITHM_ERROR_CODE_2);
	}

	int t = 1;
	t++;

}

// -- TIMER 2 --
void Handler_irq_7()
{
	IRQMP_DISABLE();
	ExecuteCore1553();
	IRQMP_ENABLE();
}

// -- TIMER 3 --
void Handler_irq_8()
{

}

// -- CORE 1553 --
void Handler_irq_14()
{
	unsigned short core1553_pending = *((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG04_PENDING_INTERRUPT));
	if (core1553_pending != 0)
	{
		Core1553_Selftest(core1553_pending);
	}
}

// -- IRQMP_1Hz --
void Handler_irq_15()
{
	// установить таймер для контроля сигнала "320 мс"
	//timer1_type = CONTROL_320MS;
	//TIMER1_START(320);

	// установить новое системное время
	CopyBshv(&predict_bshv, &system_bshv);

	// перейти к обработке сообщений
	HertzHandler();

	//SendTmi();
}

// -- Default (not used) --
void Handler_default()
{
	PmoSelftest(ALGORITHM_ERROR_CODE_1);
}

void InitInterruptHandlers()
{
	catch_interrupt(Handler_default, 	1);
	catch_interrupt(Handler_irq_2, 		2);
	catch_interrupt(Handler_irq_3, 		3);
	catch_interrupt(Handler_irq_4, 		4);
	catch_interrupt(Handler_default, 	5);
	catch_interrupt(Handler_irq_6, 		6);
	catch_interrupt(Handler_irq_7, 		7);
	catch_interrupt(Handler_irq_8, 		8);
	catch_interrupt(Handler_default, 	9);
	catch_interrupt(Handler_default, 	10);
	catch_interrupt(Handler_default, 	11);
	catch_interrupt(Handler_default, 	12);
	catch_interrupt(Handler_default, 	13);
	catch_interrupt(Handler_irq_14, 	14);
	catch_interrupt(Handler_irq_15, 	15);
}

void InitTimerType()
{
	timer1_type = NOT_USED;
}
