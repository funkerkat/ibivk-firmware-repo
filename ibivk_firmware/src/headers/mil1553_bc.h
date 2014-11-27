/*
 * mil1553_bc.h
 *
 *  Created on: 16.10.2014
 *      Author: Ludmila
 */

#ifndef MIL1553_BC_H_
#define MIL1553_BC_H_

// ----- MIL-STD-1553B -----
#define MIL1553_BASE_ADDRESS				0x80000C00

#define MIL1553_STATUS_CONTROL				0x100
#define MIL1553_IRQ_SETTINGS				0x104
#define MIL1553_AHB_PAGE_ADDRESS			0x108

// ----- Core1553BRM All Registers -----
#define MIL1553_REG00_CONTROL				0x00
#define MIL1553_REG01_STATUS				0x04
#define MIL1553_REG02_CURRENT_COMMAND		0x08
#define MIL1553_REG03_INTERRUPT_MASK		0x0C
#define MIL1553_REG04_PENDING_INTERRUPT		0x10
#define MIL1553_REG05_INTERRUPT_POINTER		0x14
#define MIL1553_REG06_BUILD_IN_TEST			0x18
#define MIL1553_REG07_MINOR_FRAME_TIMER		0x1C
#define MIL1553_REG08_COMMAND_BLOCK_POINTER	0x20
#define MIL1553_REG09_STATUS_WORD			0x24
#define MIL1553_REG32_ENHANCED				0x80

// Core1553 Register Configuration in BC mode:
// Register 00 - Control (MIL1553_REG00_CONTROL)
#define CONTROL_STEX		(0			<< 15)		// Start Execution
#define CONTROL_SBIT		(0			<< 14) 		// Start BIT, not supported, must be 0
#define CONTROL_SRST		(0			<< 13)		// Software Reset
#define CONTROL_ETCE		(0			<< 10)		// External Timer Clock Enable
#define CONTROL_MSGTO		(0			<< 9)		// Message Timeout
#define CONTROL_BCEN		(0			<< 4)		// Broadcast Enable
#define CONTROL_PPEN		(0			<< 2)		// Ping Pong Enable. If PPEN is set HIGH, the core will alternate between Bus A and Bus B on message retries. If set LOW, the core will retry only on the programmed bus as defined in the command block Control Word.
#define CONTROL_INEN		(1			<< 1)		// Interrupt Log List Enabled
#define CONTROL (CONTROL_STEX|CONTROL_SBIT|CONTROL_SRST|CONTROL_ETCE|CONTROL_MSGTO|CONTROL_BCEN|CONTROL_PPEN|CONTROL_INEN)

// Register 01 - Operation and Status (MIL1553_REG01_STATUS)
#define STATUS_MSEL			(0			<< 8)		// Mode Select:			Bus Controller		0x00
#define STATUS_ABSTD		(1			<< 7)		// 1553A/1553B:			read-only			A/B STD is set LOW for MIL-STD-1553B operation and HIGH for MIL-STD-1553A.
#define STATUS_LOCK			(0			<< 6)		// LOCK Status:			read-only
#define STATUS_EX			(0			<< 3)		// Core Execute:		read-only			HIGH, the core is executing; LOW, the core is idle.
#define STATUS_READY		(0			<< 1)		// READY Status:		read-only
#define STATUS_TERACT		(0			<< 0)		// Terminal Active:		read-only
#define STATUS (STATUS_MSEL|STATUS_ABSTD|STATUS_LOCK|STATUS_EX|STATUS_READY|STATUS_TERACT)

// Register 32 - Enhanced (MIL1553_REG32_ENHANCED)
#define ENHANCED_VERSION	(0			<< 8)		// Version
#define ENHANCED_LOOPBACK	(0			<< 6) 		// Loopback Enable
#define ENHANCED_ASYNCMSG	(1			<< 5)		// Enabled Asynchronous Message
#define ENHANCED_FASTIMG	(0			<< 4)		// Fast Inter-Message Gap
#define ENHANCED_FORCEORUN	(0			<< 3)		// Force Overrun
#define ENHANCED_CLKFREQ	(3			<< 0) 		// 24 MHz
#define ENHANCED (ENHANCED_VERSION|ENHANCED_LOOPBACK|ENHANCED_ASYNCMSG|ENHANCED_FASTIMG|ENHANCED_FORCEORUN|ENHANCED_CLKFREQ)

#endif /* MIL1553_BC_H_ */
