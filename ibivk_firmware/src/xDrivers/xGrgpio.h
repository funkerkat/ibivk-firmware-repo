/*
 * xGrgpio.h
 *
 *  Created on: 16.10.2014
 *      Author: Ludmila
 */

#ifndef XGRGPIO_H_
#define XGRGPIO_H_

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
#define DIR_GPIO4				(INPUT << 4)
#define DIR_GPIO5				(OUTPUT << 5)
#define DIR_GPIO6				(OUTPUT << 6)
#define DIR_GPIO7				(OUTPUT << 7)
#define DIR_GPIO8				(OUTPUT << 8)
#define DIR_GPIO9				(OUTPUT << 9)
#define DIR_GPIO10				(OUTPUT << 10)
#define DIR_GPIO11				(OUTPUT << 11)
#define DIR_GPIO12				(OUTPUT  << 12)
#define DIR_GPIO13				(OUTPUT << 13)
#define DIR_GPIO14				(OUTPUT << 14)
#define DIR_GPIO15				(INPUT << 15)
#define DIRECTION_GPIO (DIR_GPIO0|DIR_GPIO1|DIR_GPIO2|DIR_GPIO3|		\
						DIR_GPIO4|DIR_GPIO5|DIR_GPIO6|DIR_GPIO7|		\
						DIR_GPIO8|DIR_GPIO9|DIR_GPIO10|DIR_GPIO11|		\
						DIR_GPIO12|DIR_GPIO13|DIR_GPIO14|DIR_GPIO15)


// ----- GPIO_REG_GPIO_IRQ_MASK -----
#define MASKED						0
#define ENABLED						1

#define MASK_GPIO0				(MASKED << 0)
#define MASK_GPIO1				(MASKED << 1)
#define MASK_GPIO2				(MASKED << 2)
#define MASK_GPIO3				(MASKED << 3)
#define MASK_GPIO4				(ENABLED << 4)		// 320 ms
#define MASK_GPIO5				(MASKED << 5)
#define MASK_GPIO6				(MASKED << 6)
#define MASK_GPIO7				(MASKED << 7)
#define MASK_GPIO8				(MASKED << 8)
#define MASK_GPIO9				(MASKED << 9)
#define MASK_GPIO10				(MASKED << 10)
#define MASK_GPIO11				(MASKED << 11)
#define MASK_GPIO12				(MASKED << 12)
#define MASK_GPIO13				(MASKED << 13)
#define MASK_GPIO14				(MASKED << 14)
#define MASK_GPIO15				(ENABLED << 15)		// 1 Hz
#define MASK_GPIO (MASK_GPIO0|MASK_GPIO1|MASK_GPIO2|MASK_GPIO3|			\
				   MASK_GPIO4|MASK_GPIO5|MASK_GPIO6|MASK_GPIO7|			\
				   MASK_GPIO8|MASK_GPIO9|MASK_GPIO10|MASK_GPIO11|		\
				   MASK_GPIO12|MASK_GPIO13|MASK_GPIO14|MASK_GPIO15)


// ----- GPIO_REG_GPIO_IRQ_POLARITY -----
#define LOW_FALLING					0
#define HIGH_RISING					1

#define POLARITY_GPIO0				(HIGH_RISING << 0)
#define POLARITY_GPIO1				(HIGH_RISING << 1)
#define POLARITY_GPIO2				(HIGH_RISING << 2)
#define POLARITY_GPIO3				(HIGH_RISING << 3)
#define POLARITY_GPIO4				(LOW_FALLING << 4)		// 320 ms
#define POLARITY_GPIO5				(HIGH_RISING << 5)
#define POLARITY_GPIO6				(HIGH_RISING << 6)
#define POLARITY_GPIO7				(HIGH_RISING << 7)
#define POLARITY_GPIO8				(HIGH_RISING << 8)
#define POLARITY_GPIO9				(HIGH_RISING << 9)
#define POLARITY_GPIO10				(HIGH_RISING << 10)
#define POLARITY_GPIO11				(HIGH_RISING << 11)
#define POLARITY_GPIO12				(HIGH_RISING << 12)
#define POLARITY_GPIO13				(HIGH_RISING << 13)
#define POLARITY_GPIO14				(HIGH_RISING << 14)
#define POLARITY_GPIO15				(HIGH_RISING << 15)		// 1 Hz
#define POLARITY_GPIO (POLARITY_GPIO0|POLARITY_GPIO1|POLARITY_GPIO2|POLARITY_GPIO3|			\
					   POLARITY_GPIO4|POLARITY_GPIO5|POLARITY_GPIO6|POLARITY_GPIO7|			\
					   POLARITY_GPIO8|POLARITY_GPIO9|POLARITY_GPIO10|POLARITY_GPIO11|		\
					   POLARITY_GPIO12|POLARITY_GPIO13|POLARITY_GPIO14|POLARITY_GPIO15)


// ----- GPIO_REG_GPIO_IRQ_EDGE -----
#define LEVEL						0
#define EDGE						1

#define EDGE_GPIO0				(EDGE << 0)
#define EDGE_GPIO1				(EDGE << 1)
#define EDGE_GPIO2				(EDGE << 2)
#define EDGE_GPIO3				(EDGE << 3)
#define EDGE_GPIO4				(EDGE << 4)		// 320 ms
#define EDGE_GPIO5				(EDGE << 5)
#define EDGE_GPIO6				(EDGE << 6)
#define EDGE_GPIO7				(EDGE << 7)
#define EDGE_GPIO8				(EDGE << 8)
#define EDGE_GPIO9				(EDGE << 9)
#define EDGE_GPIO10				(EDGE << 10)
#define EDGE_GPIO11				(EDGE << 11)
#define EDGE_GPIO12				(EDGE << 12)
#define EDGE_GPIO13				(EDGE << 13)
#define EDGE_GPIO14				(EDGE << 14)
#define EDGE_GPIO15				(EDGE << 15)		// 1 Hz
#define EDGE_GPIO 	  (EDGE_GPIO0|EDGE_GPIO1|EDGE_GPIO2|EDGE_GPIO3|			\
					   EDGE_GPIO4|EDGE_GPIO5|EDGE_GPIO6|EDGE_GPIO7|			\
					   EDGE_GPIO8|EDGE_GPIO9|EDGE_GPIO10|EDGE_GPIO11|		\
					   EDGE_GPIO12|EDGE_GPIO13|EDGE_GPIO14|EDGE_GPIO15)



// ----- GRGPIO API -----
#define GRGPIO_INIT()																	\
{																						\
	*((int*)(GPIO_BASE_ADDRESS + GPIO_REG_GPIO_DIRECTION)) 		= DIRECTION_GPIO;		\
	*((int*)(GPIO_BASE_ADDRESS + GPIO_REG_GPIO_IRQ_MASK)) 		= MASK_GPIO;			\
	*((int*)(GPIO_BASE_ADDRESS + GPIO_REG_GPIO_IRQ_POLARITY)) 	= POLARITY_GPIO;		\
	*((int*)(GPIO_BASE_ADDRESS + GPIO_REG_GPIO_IRQ_EDGE)) 		= EDGE_GPIO;			\
}

#define GRGPIO_TOGGLE(nGpio)															\
{																						\
	*((int*)(GPIO_BASE_ADDRESS + GPIO_REG_GPIO_OUTPUT)) =  1 << nGpio;					\
	*((int*)(GPIO_BASE_ADDRESS + GPIO_REG_GPIO_OUTPUT)) = 0x0000;						\
}

#endif /* XGRGPIO_H_ */
