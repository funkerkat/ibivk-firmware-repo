/*
 * xIrqmp.h
 *
 *  Created on: 22.10.2014
 *      Author: Ludmila
 */

#ifndef XIRQMP_H_
#define XIRQMP_H_

// ----- Interrupt Controller Registers -----
#define IRQMP_BASE_ADDRESS			0x80000200

#define IRQMP_REG_INTERRUPT_LEVEL	0x00
#define IRQMP_REG_INTERRUPT_PENDING	0x04
#define IRQMP_REG_INTERRUPT_FORCE	0x08
#define IRQMP_REG_INTERRUPT_CLEAR	0x0C
#define IRQMP_REG_INTERRUPT_MASK	0x40

// ----- Allowed Interrupts (IRQMP_REG_INTERRUPT_MASK) -----
#define FORBIDDEN		0
#define ALLOWED			1

#define NOTUSED			FORBIDDEN

// internal interrupts:
#define IRQMP_UART1		FORBIDDEN
#define IRQMP_UART2		FORBIDDEN //ALLOWED
#define IRQMP_TIMER1	FORBIDDEN //ALLOWED
#define IRQMP_TIMER2	FORBIDDEN //ALLOWED
#define IRQMP_TIMER3	FORBIDDEN
#define IRQMP_B1553BRM	ALLOWED
// external interrupts:
#define IRQMP_1HZ		FORBIDDEN

// interrupts map:
#define IRQMP_1			(NOTUSED			<< 1)
#define IRQMP_2			(IRQMP_UART1		<< 2)
#define IRQMP_3			(IRQMP_UART2		<< 3)
#define IRQMP_4			(NOTUSED			<< 4)
#define IRQMP_5			(NOTUSED			<< 5)
#define IRQMP_6			(IRQMP_TIMER1		<< 6)
#define IRQMP_7			(IRQMP_TIMER2		<< 7)
#define IRQMP_8			(IRQMP_TIMER3		<< 8)
#define IRQMP_9			(NOTUSED			<< 9)
#define IRQMP_10		(NOTUSED			<< 10)
#define IRQMP_11		(NOTUSED			<< 11)
#define IRQMP_12		(IRQMP_1HZ			<< 12)
#define IRQMP_13		(NOTUSED			<< 13)
#define IRQMP_14		(IRQMP_B1553BRM		<< 14)
#define IRQMP_15		(NOTUSED			<< 15)
#define IRQMP_MASK (IRQMP_1|IRQMP_2|IRQMP_3|IRQMP_4|IRQMP_5|IRQMP_6|IRQMP_7|IRQMP_8|IRQMP_9|IRQMP_10|IRQMP_11|IRQMP_12|IRQMP_13|IRQMP_14|IRQMP_15)

// ----- IRQMP API -----
#define IRQMP_CLEAR()																	\
{																						\
	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_LEVEL)) 	= 0;					\
	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_PENDING)) = 0;					\
	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_FORCE)) 	= 0;					\
	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_CLEAR)) 	= 0;					\
	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_MASK))	= 0;					\
}

#define IRQMP_ENABLE()																	\
{																						\
	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_MASK)) = IRQMP_MASK;				\
}

#define IRQMP_DISABLE()																	\
{																						\
	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_MASK)) = 0;						\
}

#define IRQMP_INIT()																	\
{																						\
	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_MASK)) = IRQMP_MASK;				\
	*((int*)(IRQMP_BASE_ADDRESS + IRQMP_REG_INTERRUPT_PENDING)) = 0;					\
}

#endif /* XIRQMP_H_ */
