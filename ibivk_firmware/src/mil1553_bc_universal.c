
#include "xMil1553BC.h"
/*
static void TestRam(void)
{
	int i;
	int* address;
	address = (int*)0xA0000000;
	for (i = 0; i < 0x200; i++) {
		*address = i;
		*address = 0;
		address++;
	}
}
*/
//*********************************************************
/*
#define ADDRESS_DATA					0xA0000400
#define ADDRESS_COMMAND_BLOCK			0xA0000100

#define CORE1553_PAGE_ADDRESS			0xA0000000
#define CORE1553_ADDRESS_DATA			((ADDRESS_DATA&0xFFFF)/2)
#define CORE1553_ADDRESS_COMMAND_BLOCK	((ADDRESS_COMMAND_BLOCK&0xFFFF)/2)
*/

#define ADDRESS_DATA					0xA0000400
#define ADDRESS_COMMAND_BLOCK			0xA0000100

#define CORE1553_PAGE_ADDRESS			0xA0000000
#define CORE1553_ADDRESS_DATA			((ADDRESS_DATA - CORE1553_PAGE_ADDRESS)/2)
#define CORE1553_ADDRESS_COMMAND_BLOCK	((ADDRESS_COMMAND_BLOCK - CORE1553_PAGE_ADDRESS)/2)


static void LoadDataToTransmit(void)
{
	int* address;
	address = (int*)ADDRESS_DATA;
	int i;
	int value;
	for (i = 0; i < 32; i++) {
		value = (((2*i)+1)<<16)|(2*i+2);
		*(address + 1*i) = value;
	}
}


static int ResultConrolWord(int opcode)
{
	struct ControlWordStruct {
		unsigned int Opcode;
		unsigned int Retry;
		unsigned int CHAB;
		unsigned int RTRT;
		unsigned int ConditionCode;
		unsigned int BAME;
	};
	struct ControlWordStruct ControlWord;


			ControlWord.Opcode	= opcode;	// Opcode
			ControlWord.Retry	= 0x03;		// Retry Number
			ControlWord.CHAB	= 0x1;		// Channel A/B:			Selects bus A as the primary bus
			ControlWord.RTRT = 0;			// RT-to-RT Transfer	Should transmit CommandWord2
			ControlWord.ConditionCode = 0;	// Read Only			Response from RT.
			ControlWord.BAME = 0;			// Read-only			Protocol Message Error in RT response
	return (ControlWord.Opcode<<12)|(ControlWord.Retry<<10)|(ControlWord.CHAB<<9)|(ControlWord.RTRT<<8)|(ControlWord.ConditionCode<<1)|(ControlWord.BAME<<0);
}

static int ResultCommandWord(int RTaddress, int TR, int Subaddress, int WordCount)
{
	struct CommandWordStruct {
		unsigned int RTAddress;
		unsigned int TR;
		unsigned int Subaddress;
		unsigned int WordCount;
	};
	struct CommandWordStruct CommandWord;

	CommandWord.RTAddress	= RTaddress;
	CommandWord.TR			= TR;
	CommandWord.Subaddress	= Subaddress;
	CommandWord.WordCount	= WordCount;

	return (CommandWord.RTAddress<<11)|(CommandWord.TR<<10)|(CommandWord.Subaddress<<5)|(CommandWord.WordCount<<0);

}

//*********************************************************



int Mil1553BC_Start(int nWord)
{

int* address_cmd_block = (int*)ADDRESS_COMMAND_BLOCK;

#define R		1	// Receive
#define T		0	// Transmit

int ValueControlWord	= 0;
int ValueCommandWord1	= 0;
int ValueCommandWord2	= 0;
int ValueDataPointer	= 0;
int ValueStatusWord1	= 0;
int ValueStatusWord2	= 0;
int ValueBranchAddress	= 0;
int ValueTimer			= 0;


// ***********************************
// Clean RAM and prepare Data
//TestRam();
LoadDataToTransmit();

// ***********************************

// I. Software Reset
//mko_cor_rg.CORE1553RG00 = 0x2000;
*((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG00_CONTROL)) = 0x2000;
asm("nop");
asm("nop");
asm("nop");
asm("nop");

// ***********************************
// II. Config AHB Page Address Register
//mko_rg.MIL1553RG02 = (0xA0000000);
*((int*)(MIL1553_BASE_ADDRESS + MIL1553_AHB_PAGE_ADDRESS)) = CORE1553_PAGE_ADDRESS;

// ***********************************
// III. Init Registers
//mko_cor_rg.CORE1553RG00 = CONTROL;		// 0.  CORE1553RG00 - Control register
*((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG00_CONTROL)) = CONTROL;

//mko_cor_rg.CORE1553RG01 = STATUS;		// 1.  CORE1553RG01 - Operation and Status
*((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG01_STATUS)) = STATUS;

//mko_cor_rg.CORE1553RG02 = 0;			// 2.  CORE1553RG02 - Current Command			read-only?
*((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG02_CURRENT_COMMAND)) = 0;

//mko_cor_rg.CORE1553RG03 = 0xFFFF;		// 3.  CORE1553RG03 - Interrupt mask			1 - allow, 0 - mask
*((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG03_INTERRUPT_MASK)) = 0xFFFF;

//mko_cor_rg.CORE1553RG04 = 0;			// 4.  CORE1553RG04 - Pending interrupt			read-only
*((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG04_PENDING_INTERRUPT)) = 0;

//mko_cor_rg.CORE1553RG05 = (0x10);		// 5.  CORE1553RG05 - Interrupt pointer
*((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG05_INTERRUPT_POINTER)) = 0x10;

//mko_cor_rg.CORE1553RG06 = 0;			// 6.  CORE1553RG06 - Build-in test register
*((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG06_BUILD_IN_TEST)) = 0;

//mko_cor_rg.CORE1553RG07 = 0;			// 7.  CORE1553RG07 - Minor Frame Timer
*((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG07_MINOR_FRAME_TIMER)) = 0;

//mko_cor_rg.CORE1553RG08 = 0x40*2;		// 8.  CORE1553RG08 - Command Block Pointer
*((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG08_COMMAND_BLOCK_POINTER)) = CORE1553_ADDRESS_COMMAND_BLOCK;

//mko_cor_rg.CORE1553RG32 = ENHANCED;		// 32. CORE1553RG032 - Enhanced Features
*((int*)(MIL1553_BASE_ADDRESS + MIL1553_REG32_ENHANCED)) = ENHANCED;



// ***********************************
// IV. Load COMMAND BLOCK table in memory - 1
ValueControlWord	= ResultConrolWord(4);
//ValueCommandWord1	= ResultCommandWord(10, T, 1, 0);
ValueCommandWord1	= ResultCommandWord(1, T, 1, nWord);
ValueCommandWord2	= 0;
ValueDataPointer	= CORE1553_ADDRESS_DATA;
ValueStatusWord1	= 0;
ValueStatusWord2	= 0;
ValueBranchAddress	= 0;
ValueTimer			= 0;


*(address_cmd_block + 0)  = (ValueControlWord		<<16)|(ValueCommandWord1);
*(address_cmd_block + 1)  = (ValueCommandWord2	<<16)|(ValueDataPointer);
*(address_cmd_block + 2)  = (ValueStatusWord1		<<16)|(ValueStatusWord2);
*(address_cmd_block + 3)  = (ValueBranchAddress	<<16)|(ValueTimer);


//ValueControlWord	= ResultConrolWord(14);
ValueControlWord	= ResultConrolWord(4);
//ValueControlWord	= ResultConrolWord(0);
ValueCommandWord1	= ResultCommandWord(10, T, 1, 2);
ValueCommandWord2	= 0;
ValueDataPointer	= CORE1553_ADDRESS_DATA;
ValueStatusWord1	= 0;
ValueStatusWord2	= 0;
ValueBranchAddress	= 0;
ValueTimer			= 0;


*(address_cmd_block + 4)  = (ValueControlWord		<<16)|(ValueCommandWord1);
*(address_cmd_block + 5)  = (ValueCommandWord2	<<16)|(ValueDataPointer);
*(address_cmd_block + 6)  = (ValueStatusWord1		<<16)|(ValueStatusWord2);
*(address_cmd_block + 7)  = (ValueBranchAddress	<<16)|(ValueTimer);


ValueControlWord	= ResultConrolWord(4);
ValueCommandWord1	= ResultCommandWord(10, T, 1, 3);
ValueCommandWord2	= 0;
ValueDataPointer	= CORE1553_ADDRESS_DATA;
ValueStatusWord1	= 0;
ValueStatusWord2	= 0;
ValueBranchAddress	= 0;
ValueTimer			= 0;


*(address_cmd_block + 8)  = (ValueControlWord		<<16)|(ValueCommandWord1);
*(address_cmd_block + 9)  = (ValueCommandWord2	<<16)|(ValueDataPointer);
*(address_cmd_block + 10)  = (ValueStatusWord1	<<16)|(ValueStatusWord2);
*(address_cmd_block + 11)  = (ValueBranchAddress	<<16)|(ValueTimer);


ValueControlWord	= ResultConrolWord(0);
ValueCommandWord1	= ResultCommandWord(10, T, 1, 4);
ValueCommandWord2	= 0;
ValueDataPointer	= CORE1553_ADDRESS_DATA;
ValueStatusWord1	= 0;
ValueStatusWord2	= 0;
ValueBranchAddress	= 0;
ValueTimer			= 0;


*(address_cmd_block + 12)  = (ValueControlWord	<<16)|(ValueCommandWord1);
*(address_cmd_block + 13)  = (ValueCommandWord2	<<16)|(ValueDataPointer);
*(address_cmd_block + 14)  = (ValueStatusWord1	<<16)|(ValueStatusWord2);
*(address_cmd_block + 15)  = (ValueBranchAddress	<<16)|(ValueTimer);



// ***********************************
// V. Start Execution
//mko_cor_rg.CORE1553RG00 |= mko_cor_rg.CORE1553RG00 | (1<<15);
START_EXECUTION();
return 0;
}

