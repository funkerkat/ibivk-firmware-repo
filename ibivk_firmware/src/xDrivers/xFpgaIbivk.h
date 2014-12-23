/*
 * xFpgaIbivk.h
 *
 *  Created on: 06.12.2014
 *      Author: Ludmila
 */

#ifndef XFPGAIBIVK_H_
#define XFPGAIBIVK_H_

#define FPGA_IBIVK_BASE_ADDRESS		0x20000000

#define FPGA_IBIVK_TEST				0x00
#define FPGA_IBIVK_IBSU_READ		0x04
#define FPGA_IBIVK_IBSU_SET			0x04
#define FPGA_IBIVK_IBSU_TM			0x08
#define FPGA_IBIVK_IBSU_COM			0x08
#define FPGA_IBIVK_IBSU_TM2			0x0c
#define FPGA_IBIVK_IBSU_PRESCALER	0x0c

// Регистр FPGA_IBIVK_IBSU_COM
#define COM_NF_1		(0			<< 5)		// Команда недостоверности фазы опорного генератора БСУ резервного.
#define COM_NF_0		(0			<< 4)		// Команда недостоверности фазы опорного генератора БСУ основного.
#define COM_IMIT_5MHz	(0			<< 3)		// Команда выбора источника опорного сигнала 5МГц блока ИБСУ (в проекте для БЦУ БИНК-К2 не используется)
#define COM_IMIT		(0			<< 2)		// Команда выбора источника сигналов 1Гц, ОЦ0, ОЦ1 (внешняя/ внутренняя синхронизация)  1 - внутренний,  0 - внешний
#define COM_RST			(0			<< 1)		// Программный сброс ИБСУ. Переводит блок ИБСУ в начальное состояние
#define COM_CLK_EN		(1			<< 0)		// Команда разрешения счета значения БШВ блоком ИБСУ
#define IBSU_COM (COM_CLK_EN|COM_RST|COM_IMIT|COM_IMIT_5MHz|COM_NF_0|COM_NF_1)



#define FPGA_IBIVK_SOFTWARE_RESET()										\
{																		\
	*((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_IBSU_COM)) = 0x02;	\
	asm("nop");															\
	asm("nop");															\
	asm("nop");															\
	*((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_IBSU_COM)) = 0x00;	\
	asm("nop");															\
	asm("nop");															\
	asm("nop");															\
}



#define FPGA_IBIVK_INIT()													\
{																			\
	FPGA_IBIVK_SOFTWARE_RESET();											\
	*((int*)(FPGA_IBIVK_BASE_ADDRESS + FPGA_IBIVK_IBSU_COM)) = IBSU_COM;	\
}


#endif /* XFPGAIBIVK_H_ */
