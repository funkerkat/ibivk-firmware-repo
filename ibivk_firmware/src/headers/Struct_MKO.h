
	/* 1553BRM Registers */				// 0x80000D00
	struct Rg1553   {
		unsigned int  MIL1553RG00; // B1553BRM status/control
		unsigned int  MIL1553RG01; // B1553BRM interrupt settings
		unsigned int  MIL1553RG02; // AHB page address register
	};
	extern volatile struct Rg1553 mko_rg;



	/* Core 1553BRM Registers */		// 0x80000C00
	struct RgCORE1553   {
		unsigned int  CORE1553RG00; // Control
		unsigned int  CORE1553RG01; // Operation and Status
		unsigned int  CORE1553RG02; // Current Command
		unsigned int  CORE1553RG03; // Interrupt Mask
		unsigned int  CORE1553RG04; // Pending Interrupt
		unsigned int  CORE1553RG05; // Interrupt Pointer
		unsigned int  CORE1553RG06; // Built-In Test register
		unsigned int  CORE1553RG07; // BC: Minor Frame Timer Register	/ RT: Time Tag
		unsigned int  CORE1553RG08; // BC: Command Block Pointer		/ RT: Descriptor Pointer
		unsigned int  CORE1553RG09; // 1553A/B Status Word
		unsigned int  rsvd1[6];     // RG10 - RG15 reserved
		unsigned int  CORE1553RG16; // RT: illegalization Registers
		unsigned int  CORE1553RG17;
		unsigned int  CORE1553RG18;
		unsigned int  CORE1553RG19;
		unsigned int  CORE1553RG20;
		unsigned int  CORE1553RG21;
		unsigned int  CORE1553RG22;
		unsigned int  CORE1553RG23;
		unsigned int  CORE1553RG24;
		unsigned int  CORE1553RG25;
		unsigned int  CORE1553RG26;
		unsigned int  CORE1553RG27;
		unsigned int  CORE1553RG28;
		unsigned int  CORE1553RG29;
		unsigned int  CORE1553RG30;
		unsigned int  CORE1553RG31;

		unsigned int  CORE1553RG32; // Enhanced Features
	};
	extern volatile struct RgCORE1553 mko_cor_rg;



