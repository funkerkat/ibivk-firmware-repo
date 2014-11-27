/*
 * grgpio.h
 *
 *  Created on: 16.10.2014
 *      Author: Ludmila
 */

#ifndef GRGPIO_H_
#define GRGPIO_H_

// ----- General Purpose I/O Port Registers -----
#define GPIO_BASE_ADDRESS			0x80000800

#define GPIO_REG_GPIO_OUTPUT		0x04
#define GPIO_REG_GPIO_DIRECTION		0x08
#define GPIO_REG_GPIO_IRQ_MASK		0x0C
#define GPIO_REG_GPIO_IRQ_POLARITY	0x10
#define GPIO_REG_GPIO_IRQ_EDGE		0x14

// ----- REG_GPIO_DIRECTION -----
#define INPUT						0
#define OUTPUT						1

#define DIR_GPIO0				(OUTPUT << 0)
#define DIR_GPIO1				(OUTPUT << 1)
#define DIR_GPIO2				(OUTPUT << 2)
#define DIR_GPIO3				(OUTPUT << 3)
#define DIR_GPIO4				(OUTPUT << 4)
#define DIR_GPIO5				(OUTPUT << 5)
#define DIR_GPIO6				(OUTPUT << 6)
#define DIR_GPIO7				(OUTPUT << 7)
#define DIR_GPIO8				(OUTPUT << 8)
#define DIR_GPIO9				(OUTPUT << 9)
#define DIR_GPIO10				(OUTPUT << 10)
#define DIR_GPIO11				(OUTPUT << 11)
#define DIR_GPIO12				(INPUT  << 12)
#define DIR_GPIO13				(OUTPUT << 13)
#define DIR_GPIO14				(OUTPUT << 14)
#define DIR_GPIO15				(OUTPUT << 15)
#define DIRECTION_GPIO (DIR_GPIO0|DIR_GPIO1|DIR_GPIO2|DIR_GPIO3|DIR_GPIO4|DIR_GPIO5|DIR_GPIO6|DIR_GPIO7|DIR_GPIO8|DIR_GPIO9|DIR_GPIO10|DIR_GPIO11|DIR_GPIO12|DIR_GPIO13|DIR_GPIO14|DIR_GPIO15)

// ----- GRGPIO API -----
// Polarity: 		rising
// Edge/level: 	 	edge
#define GRGPIO_INIT()																	\
{																						\
	*((int*)(GPIO_BASE_ADDRESS + GPIO_REG_GPIO_DIRECTION)) = DIRECTION_GPIO;			\
	*((int*)(GPIO_BASE_ADDRESS + GPIO_REG_GPIO_IRQ_MASK)) = 0xFFFF;						\
	*((int*)(GPIO_BASE_ADDRESS + GPIO_REG_GPIO_IRQ_POLARITY)) = 0xFFFF;					\
	*((int*)(GPIO_BASE_ADDRESS + GPIO_REG_GPIO_IRQ_EDGE)) = 0xFFFF;						\
}

#define GRGPIO_TOGGLE(nGpio)															\
{																						\
	*((int*)(GPIO_BASE_ADDRESS + GPIO_REG_GPIO_OUTPUT)) =  1 << nGpio;					\
	*((int*)(GPIO_BASE_ADDRESS + GPIO_REG_GPIO_OUTPUT)) = 0x0000;						\
}

#endif /* GRGPIO_H_ */
