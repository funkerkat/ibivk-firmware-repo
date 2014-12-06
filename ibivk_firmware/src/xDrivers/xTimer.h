/*
 * xTimer.h
 *
 *  Created on: 22.10.2014
 *      Author: Ludmila
 */

#ifndef XTIMER_H_
#define XTIMER_H_

// ----- General Purpose Timer Unit Registers -----
#define TIMER_BASE_ADDRESS			0x80000300

#define REG_SCALER_VALUE			0x00
#define REG_SCALER_RELOAD_VALUE		0x04
#define REG_CONFIGURATION			0x08

#define REG_TIMER1_COUNTER_VALUE	0x10
#define REG_TIMER1_RELOAD_VALUE		0x14
#define REG_TIMER1_CONTROL			0x18

#define REG_TIMER2_COUNTER_VALUE	0x20
#define REG_TIMER2_RELOAD_VALUE		0x24
#define REG_TIMER2_CONTROL			0x28

#define REG_TIMER3_COUNTER_VALUE	0x30
#define REG_TIMER3_RELOAD_VALUE		0x34
#define REG_TIMER3_CONTROL			0x38

// ----- Scaler Value (REG_SCALER_VALUE) -----
#define TIMER_SCALER_VALUE 			24

// ----- Timer Control Register (REG_TIMER_CONTROL) -----
// Debug Halt (DH): read-only.
#define TIMER_CONTROL_DH					(0 << 6)

// Chain (CH): Chain with preceding timer.
// If set for timer n, decrementing timer n begins when timer (n-1) underflows.
#define TIMER_CONTROL_CH					(0 << 5)

// Interrupt Pending (IP): Sets when an interrupt is signalled.
// Remains ‘1’ until cleared by writing ‘0’ to this bit.
#define TIMER_CONTROL_IP					(0 << 4)

// Interrupt Enable (IE): If set the timer signals interrupt when it underflows.
#define TIMER_CONTROL_IE					(1 << 3)

// Load (LD): Load value from the timer reload register to the timer counter value register.
#define TIMER_CONTROL_LD					(0 << 2)		// !!!

// Restart (RS): If set, the timer counter value register is reloaded
// with the value of the reload register when the timer underflows
#define TIMER_CONTROL_RS					(0 << 1)

// Enable (EN): Enable the timer.
#define TIMER_CONTROL_EN					(1 << 0)

#define TIMER_CONTROL_VALUE (TIMER_CONTROL_DH|TIMER_CONTROL_CH|TIMER_CONTROL_IP|TIMER_CONTROL_IE|TIMER_CONTROL_LD|TIMER_CONTROL_RS|TIMER_CONTROL_EN)


// ----- Timer API -----
#define TIMER_INIT()																	\
{																						\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER1_CONTROL)) 			= 0;					\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER2_CONTROL)) 			= 0;					\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER3_CONTROL)) 			= 0;					\
	*((int*)(TIMER_BASE_ADDRESS + REG_SCALER_VALUE)) 			= TIMER_SCALER_VALUE;	\
	*((int*)(TIMER_BASE_ADDRESS + REG_SCALER_RELOAD_VALUE)) 	= TIMER_SCALER_VALUE;	\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER1_COUNTER_VALUE)) 	= 0;					\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER1_RELOAD_VALUE)) 	= 0;					\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER2_COUNTER_VALUE)) 	= 0;					\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER2_RELOAD_VALUE)) 	= 0;					\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER3_COUNTER_VALUE)) 	= 0;					\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER3_RELOAD_VALUE)) 	= 0;					\
}

#define TIMER1_START(value)																\
{																						\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER1_RELOAD_VALUE)) 	= 0;					\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER1_COUNTER_VALUE)) 	= value;				\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER1_CONTROL)) 			= TIMER_CONTROL_VALUE;	\
}

#define TIMER2_START(value)																\
{																						\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER2_RELOAD_VALUE)) 	= 0;					\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER2_COUNTER_VALUE)) 	= value;				\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER2_CONTROL)) 			= TIMER_CONTROL_VALUE;	\
}

#define TIMER3_START(value)																\
{																						\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER3_RELOAD_VALUE)) 	= 0;					\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER3_COUNTER_VALUE)) 	= value;				\
	*((int*)(TIMER_BASE_ADDRESS + REG_TIMER3_CONTROL)) 			= TIMER_CONTROL_VALUE;	\
}

#endif /* XTIMER_H_ */
