#include "olc6502.h"
#include"Bus.h"


olc6502::olc6502() {

	using a = olc6502;

	// lookup table is from http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf  pg:- 10
	lookup_tbl =
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};




}

olc6502::~olc6502() {
}

// Reads an 8-bit from the bus, located at the specified 16-bit address
uint8_t olc6502::read(uint16_t a)
{
	// In normal operation "read only" is set to false. This may seem odd. Some
	// devices on the bus may change state when they are read from, and this 
	// is intentional under normal circumstances. However the disassembler will
	// want to read the data at an address without changing the state of the
	// devices on the bus
	return bus->cpuRead(a, false);
}

// Writes a byte to the bus at the specified address
void olc6502::write(uint16_t a, uint8_t d)
{
	bus->cpuWrite(a, d);
}


// defining the clock function 

void olc6502::clock()
{
	// Each instruction requires a variable number of clock cycles to execute.
	// To remain compliant with connected devices, it's important that the 
	// emulation also takes "time" in order to execute instructions,
	// implement that delay by simply counting down the cycles required by 
	// the instruction. When it reaches 0, the instruction is complete, and
	// the next one is ready to be executed.
	if (cycles == 0)
	{
		// Read next instruction byte. This 8-bit value is used to index
		// the translation table(lookup_tbl) to get the relevant information about
		// how to implement the instruction
		opcode = read(pc);

#ifdef LOGMODE
		uint16_t log_pc = pc;
#endif

		// Always set the unused status flag bit to 1
		setFlag(U, true);

		// Increment program counter, we read the opcode byte
		pc++;

		// Get Starting number of cycles
		cycles = lookup_tbl[opcode].cycles;

		// Perform fetch of intermmediate data using the
		// required addressing mode
		uint8_t additional_cycle1 = (this->*lookup_tbl[opcode].addrmode)();

		// Perform operation
		uint8_t additional_cycle2 = (this->*lookup_tbl[opcode].operate)();

		// The addressmode and opcode may have altered the number
		// of cycles this instruction requires before its completed
		cycles += (additional_cycle1 & additional_cycle2);

		// Always set the unused status flag bit to 1
		setFlag(U, true);

#ifdef LOGMODE
		// This logger dumps every cycle the entire processor state for analysis.
		// This can be used for debugging the emulation, but has little utility
		// during emulation. Its also very slow, so only use if you have to.
		if (logfile == nullptr)
		{
			errno_t err = fopen_s(&logfile, "olc6502.txt", "wt");
			if (err != 0)
			{
				// Handle the error (e.g., display an error message or take appropriate action)
			}
		}

		if (logfile != nullptr)
		{
			fprintf(logfile, "%10d:%02d PC:%04X %s A:%02X X:%02X Y:%02X %s%s%s%s%s%s%s%s STKP:%02X\n",
				clock_count, 0, log_pc, "XXX", accumulator, x, y,
				getFlag(N) ? "N" : ".", getFlag(V) ? "V" : ".", getFlag(U) ? "U" : ".",
				getFlag(B) ? "B" : ".", getFlag(D) ? "D" : ".", getFlag(I) ? "I" : ".",
				getFlag(Z) ? "Z" : ".", getFlag(C) ? "C" : ".", stack_ptr);
		}
#endif
	}

	// Increment global clock count - This is actually unused unless logging is enabled
	// but its a handy watch variable for debugging
	//clock_count++;

	// Decrement the number of cycles remaining for this instruction
	cycles--;
}

// FLAG FUNCTION

// Returns the value of a specific bit of the status register
uint8_t olc6502::getFlag(FLAGS f)
{
	return ((status_reg & f) > 0) ? 1 : 0;
}

// Sets or clears a specific bit of the status register where v is act as flag to clear and set
void olc6502::setFlag(FLAGS f, bool v)
{
	if (v)
		status_reg |= f;
	else
		status_reg &= ~f;
}

// ADDRESSING MODE

// Reference is http://www.emulator101.com/6502-addressing-modes.html

uint8_t olc6502::IMP() {
	fetched = accumulator;
	return 0;
}

uint8_t olc6502::IMM() {

	addr_abs = pc++;
	return 0;
}

uint8_t olc6502::ZP0() {
	addr_abs = read(pc);
	pc++;

	// line below ensure that it will in the range of  00 to FF as the addr_abs is of 16 bit ranging from 0000 to FFFF
	addr_abs &= 0x00FF;
	return 0;
}


// Use for iteration for a block of memory
// Zero Page indexed with Y
uint8_t olc6502::ZPX() {
	addr_abs = (read(pc) + x);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t olc6502::ZPY() {
	addr_abs = (read(pc) + y);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

/*
*  It reads two bytes from the program counter (PC) and combines them to form
*  a full 16-bit memory address.
*/

uint8_t olc6502::ABS() {
	uint16_t low = read(pc++);
	uint16_t high = read(pc++);

	addr_abs = (high << 8) | low;

	return 0;
}

uint8_t olc6502::ABX() {
	uint16_t low = read(pc++);
	uint16_t high = read(pc++);

	addr_abs = (high << 8) | low;

	addr_abs += x;

	// Afer adding x the addr_abs may change to a new page this will need an additional clock cycle to execute
	// The function updates the addr_abs member variable accordingly and returns 1 if a page boundary is crossed, or 0 otherwise
	if ((addr_abs & 0xFF00) != (high << 8))
		return 1;
	else
		return 0;
}


uint8_t olc6502::ABY() {
	uint16_t low = read(pc++);
	uint16_t high = read(pc++);

	addr_abs = (high << 8) | low;

	addr_abs += y;

	// Afer adding y the addr_abs may change to a new page this will need an additional clock cycle to execute
	if ((addr_abs & 0xFF00) != (high << 8))
		return 1;
	else
		return 0;
}

// Indirect Addressing

// The supplied 16-bit address is read to get the actual 16-bit address. This is
// instruction is unusual in that it has a bug in the hardware! To emulate its
// function accurately, we also need to emulate this bug. If the low byte of the
// supplied address is 0xFF, then to read the high byte of the actual address
// we need to cross a page boundary.

uint8_t olc6502::IND() {
	uint16_t ptr_low = read(pc++);
	uint16_t ptr_high = read(pc++);

	uint16_t ptr = ((ptr_high << 8) | ptr_low);

	// Simulate page boundary hardware bug

	/*
	* Inside the if statement, the addr_abs member variable is set differently to address the bug.
	* It reads the byte at the memory location (ptr & 0xFF00) (masking out the low byte)
	* and combines it with the byte at ptr to form the effective address. This ensures that the high byte
	* of the effective address comes from the next page, bypassing the bug.
	*/

	if (ptr_low == 0x00FF)
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr);
	else
		addr_abs = (read(ptr + 1) << 8) | read(ptr);

	return 0;
}

uint8_t olc6502::IZX() {

	uint16_t temp = read(pc++);

	uint16_t low = read((uint16_t)(temp + (uint16_t)x) & 0x00FF);
	uint16_t high = read((uint16_t)(temp + (uint16_t)x + 1) & 0x00FF);

	addr_abs = (high << 8) | low;

	return 0;
}

uint8_t olc6502::IZY()
{
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	addr_abs += y;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

// Relative Addressing Mode

uint8_t olc6502::REL() {

	addr_rel = read(pc++);
	if (addr_rel & 0x80)
		addr_rel |= 0xFF00;
	return 0;
}



// Instructions

uint8_t olc6502::fetch() {
	if (!(lookup_tbl[opcode].addrmode == &olc6502::IMP))
		fetched = read(addr_abs);
	return fetched;
}

uint8_t olc6502::AND() {
	fetch();

	accumulator &= fetched;
	setFlag(Z, accumulator == 0x00);
	setFlag(N, accumulator & 0x00);

	return 1;
}

// Branch Chnage instruction (Branch if Carry Set)

//The BCS instruction is typically followed by an 8-bit signed relative offset, which is a value between -128 and +127.

// BCS has relative addressing mode

uint8_t olc6502::BCS() {

	if (getFlag(C) == 1) {
		cycles++;
		addr_abs = pc + addr_rel;

		// Page Boundary crossed
		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Carry Clear
// Function:    if(C == 0) pc = address 
uint8_t olc6502::BCC()
{
	if (getFlag(C) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Equal
// Function:    if(Z == 1) pc = address
uint8_t olc6502::BEQ()
{
	if (getFlag(Z) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Not Equal
// Function:    if(Z == 0) pc = address
uint8_t olc6502::BNE()
{
	if (getFlag(Z) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}


// Instruction: Branch if Positive
// Function:    if(N == 0) pc = address
uint8_t olc6502::BPL()
{
	if (getFlag(N) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Overflow Clear
// Function:    if(V == 0) pc = address
uint8_t olc6502::BVC()
{
	if (getFlag(V) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}


// Instruction: Branch if Overflow Set
// Function:    if(V == 1) pc = address
uint8_t olc6502::BVS()
{
	if (getFlag(V) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Clear Carry Flag
// Function:    C = 0
uint8_t olc6502::CLC()
{
	setFlag(C, false);
	return 0;
}


// Instruction: Clear Decimal Flag
// Function:    D = 0
uint8_t olc6502::CLD()
{
	setFlag(D, false);
	return 0;
}


// Instruction: Disable Interrupts / Clear Interrupt Flag
// Function:    I = 0
uint8_t olc6502::CLI()
{
	setFlag(I, false);
	return 0;
}


// Instruction: Clear Overflow Flag
// Function:    V = 0
uint8_t olc6502::CLV()
{
	setFlag(V, false);
	return 0;
}


uint8_t olc6502::ADC() {
	fetch();
	uint16_t temp = (uint16_t)accumulator + (uint16_t)fetched + (uint16_t)getFlag(C);
	setFlag(C, temp > 255);
	setFlag(Z, (temp & 0x00FF) == 0);
	setFlag(N, temp & 0x80);
	setFlag(V, ((uint16_t)accumulator ^ (uint16_t)temp) & (~(uint16_t)accumulator ^ (uint16_t)fetched));
	accumulator = temp & 0x00FF;
	return -1;
}

// Instruction: Subtraction with Borrow In
// Function:    A = A - M - (1 - C)
// Flags Out:   C, V, N, Z

uint8_t olc6502::SBC() {
	fetch();

	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;

	uint16_t temp = (uint16_t)accumulator + value + (uint16_t)getFlag(C);
	setFlag(C, temp > 255);
	setFlag(Z, (temp & 0x00FF) == 0);
	setFlag(N, temp & 0x80);
	setFlag(V, (temp ^ (uint16_t)accumulator) & (temp ^ value) & 0x0080);
	accumulator = temp & 0x00FF;

	return -1;
}

// Push accumulator to stack

uint8_t olc6502::PHA() {


	// 6502 has hardcoded baselocation 0x0100 stack_ptr is the offset to the stack pointer
	write(0x0100 + stack_ptr, accumulator);
	stack_ptr--;
	return 0;
}

// pop something from the stack_ptr and store it to the accumulator
uint8_t olc6502::PLA() {
	stack_ptr++;
	accumulator = read(0x0100 + stack_ptr);
	setFlag(Z, accumulator == 0x00);
	setFlag(N, accumulator & 0x00);
	return 0;
}

// Instruction: Push Status Register to Stack
// Function:    status -> stack
// Note:        Break flag is set to 1 before push
uint8_t olc6502::PHP()
{
	write(0x0100 + stack_ptr, status_reg | B | U);
	setFlag(B, 0);
	setFlag(U, 0);
	stack_ptr--;
	return 0;
}

// Instruction: Compare Accumulator
// Function:    C <- A >= M      Z <- (A - M) == 0
// Flags Out:   N, C, Z
uint8_t olc6502::CMP()
{
	fetch();
	uint16_t temp = (uint16_t)accumulator - (uint16_t)fetched;
	setFlag(C, accumulator >= fetched);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 1;
}


// Instruction: Compare X Register
// Function:    C <- X >= M      Z <- (X - M) == 0
// Flags Out:   N, C, Z
uint8_t olc6502::CPX()
{
	fetch();
	uint16_t temp = (uint16_t)x - (uint16_t)fetched;
	setFlag(C, x >= fetched);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Compare Y Register
// Function:    C <- Y >= M      Z <- (Y - M) == 0
// Flags Out:   N, C, Z
uint8_t olc6502::CPY()
{
	fetch();
	uint16_t temp = (uint16_t)y - (uint16_t)fetched;
	setFlag(C, y >= fetched);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Decrement Value at Memory Location
// Function:    M = M - 1
// Flags Out:   N, Z
uint8_t olc6502::DEC()
{
	fetch();
	uint16_t temp = fetched - 1;
	write(addr_abs, temp & 0x00FF);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Decrement X Register
// Function:    X = X - 1
// Flags Out:   N, Z
uint8_t olc6502::DEX()
{
	x--;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 0;
}


// Instruction: Decrement Y Register
// Function:    Y = Y - 1
// Flags Out:   N, Z
uint8_t olc6502::DEY()
{
	y--;
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);
	return 0;
}


// Instruction: Bitwise Logic XOR
// Function:    A = A xor M
// Flags Out:   N, Z
uint8_t olc6502::EOR()
{
	fetch();
	accumulator = accumulator ^ fetched;
	setFlag(Z, accumulator == 0x00);
	setFlag(N, accumulator & 0x80);
	return 1;
}


// Instruction: Increment Value at Memory Location
// Function:    M = M + 1
// Flags Out:   N, Z
uint8_t olc6502::INC()
{
	fetch();
	uint16_t temp = fetched + 1;
	write(addr_abs, temp & 0x00FF);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Increment X Register
// Function:    X = X + 1
// Flags Out:   N, Z
uint8_t olc6502::INX()
{
	x++;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 0;
}


// Instruction: Increment Y Register
// Function:    Y = Y + 1
// Flags Out:   N, Z
uint8_t olc6502::INY()
{
	y++;
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);
	return 0;
}


// Instruction: Jump To Location
// Function:    pc = address
uint8_t olc6502::JMP()
{
	pc = addr_abs;
	return 0;
}

// Instruction: Break
// Function:    Program Sourced Interrupt
uint8_t olc6502::BRK()
{
	pc++;

	setFlag(I, 1);
	write(0x0100 + stack_ptr, (pc >> 8) & 0x00FF);
	stack_ptr--;
	write(0x0100 + stack_ptr, pc & 0x00FF);
	stack_ptr--;

	setFlag(B, 1);
	write(0x0100 + stack_ptr, status_reg);
	stack_ptr--;
	setFlag(B, 0);

	pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}


// Instruction: Pop Status Register off Stack
// Function:    Status <- stack
uint8_t olc6502::PLP()
{
	stack_ptr++;
	status_reg = read(0x0100 + stack_ptr);
	setFlag(U, 1);
	return 0;
}

// Instruction: Branch if Negative
// Function:    if(N == 1) pc = address
uint8_t olc6502::BMI()
{
	if (getFlag(N) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}


uint8_t olc6502::BIT()
{
	fetch();
	uint16_t temp = accumulator & fetched;
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, fetched & (1 << 7));
	setFlag(V, fetched & (1 << 6));
	return 0;
}

// Instruction: Arithmetic Shift Left
// Function:    A = C <- (A << 1) <- 0
// Flags Out:   N, Z, C
uint8_t olc6502::ASL()
{
	fetch();
	uint16_t temp = (uint16_t)fetched << 1;
	setFlag(C, (temp & 0xFF00) > 0);
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, temp & 0x80);
	if (lookup_tbl[opcode].addrmode == &olc6502::IMP)
		accumulator = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

// Instruction: Jump To Sub-Routine
// Function:    Push current pc to stack, pc = address
uint8_t olc6502::JSR()
{
	pc--;

	write(0x0100 + stack_ptr, (pc >> 8) & 0x00FF);
	stack_ptr--;
	write(0x0100 + stack_ptr, pc & 0x00FF);
	stack_ptr--;

	pc = addr_abs;
	return 0;
}


// Instruction: Load The Accumulator
// Function:    A = M
// Flags Out:   N, Z
uint8_t olc6502::LDA()
{
	fetch();
	accumulator = fetched;
	setFlag(Z, accumulator == 0x00);
	setFlag(N, accumulator & 0x80);
	return 1;
}


// Instruction: Load The X Register
// Function:    X = M
// Flags Out:   N, Z
uint8_t olc6502::LDX()
{
	fetch();
	x = fetched;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 1;
}


// Instruction: Load The Y Register
// Function:    Y = M
// Flags Out:   N, Z
uint8_t olc6502::LDY()
{
	fetch();
	y = fetched;
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);
	return 1;
}

uint8_t olc6502::LSR()
{
	fetch();
	setFlag(C, fetched & 0x0001);
	uint16_t temp = fetched >> 1;
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	if (lookup_tbl[opcode].addrmode == &olc6502::IMP)
		accumulator = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t olc6502::NOP()
{
	// Sadly not all NOPs are equal, Ive added a few here
	// based on https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
	// and will add more based on game compatibility, and ultimately
	// I'd like to cover all illegal opcodes too
	switch (opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}


// Instruction: Bitwise Logic OR
// Function:    A = A | M
// Flags Out:   N, Z
uint8_t olc6502::ORA()
{
	fetch();
	accumulator = accumulator | fetched;
	setFlag(Z, accumulator == 0x00);
	setFlag(N, accumulator & 0x80);
	return 1;
}

uint8_t olc6502::ROL()
{
	fetch();
	uint16_t temp = (uint16_t)(fetched << 1) | getFlag(C);
	setFlag(C, temp & 0xFF00);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	if (lookup_tbl[opcode].addrmode == &olc6502::IMP)
		accumulator = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t olc6502::ROR()
{
	fetch();
	uint16_t temp = (uint16_t)(getFlag(C) << 7) | (fetched >> 1);
	setFlag(C, fetched & 0x01);
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, temp & 0x0080);
	if (lookup_tbl[opcode].addrmode == &olc6502::IMP)
		accumulator = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t olc6502::RTI()
{
	stack_ptr++;
	status_reg = read(0x0100 + stack_ptr);
	status_reg &= ~B;
	status_reg &= ~U;

	stack_ptr++;
	pc = (uint16_t)read(0x0100 + stack_ptr);
	stack_ptr++;
	pc |= (uint16_t)read(0x0100 + stack_ptr) << 8;
	return 0;
}

uint8_t olc6502::RTS()
{
	stack_ptr++;
	pc = (uint16_t)read(0x0100 + stack_ptr);
	stack_ptr++;
	pc |= (uint16_t)read(0x0100 + stack_ptr) << 8;

	pc++;
	return 0;
}




// Instruction: Set Carry Flag
// Function:    C = 1
uint8_t olc6502::SEC()
{
	setFlag(C, true);
	return 0;
}


// Instruction: Set Decimal Flag
// Function:    D = 1
uint8_t olc6502::SED()
{
	setFlag(D, true);
	return 0;
}


// Instruction: Set Interrupt Flag / Enable Interrupts
// Function:    I = 1
uint8_t olc6502::SEI()
{
	setFlag(I, true);
	return 0;
}


// Instruction: Store Accumulator at Address
// Function:    M = A
uint8_t olc6502::STA()
{
	write(addr_abs, accumulator);
	return 0;
}


// Instruction: Store X Register at Address
// Function:    M = X
uint8_t olc6502::STX()
{
	write(addr_abs, x);
	return 0;
}


// Instruction: Store Y Register at Address
// Function:    M = Y
uint8_t olc6502::STY()
{
	write(addr_abs, y);
	return 0;
}


// Instruction: Transfer Accumulator to X Register
// Function:    X = A
// Flags Out:   N, Z
uint8_t olc6502::TAX()
{
	x = accumulator;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 0;
}


// Instruction: Transfer Accumulator to Y Register
// Function:    Y = A
// Flags Out:   N, Z
uint8_t olc6502::TAY()
{
	y = accumulator;
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);
	return 0;
}


// Instruction: Transfer Stack Pointer to X Register
// Function:    X = stack pointer
// Flags Out:   N, Z
uint8_t olc6502::TSX()
{
	x = stack_ptr;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 0;
}


// Instruction: Transfer X Register to Accumulator
// Function:    A = X
// Flags Out:   N, Z
uint8_t olc6502::TXA()
{
	accumulator = x;
	setFlag(Z, accumulator == 0x00);
	setFlag(N, accumulator & 0x80);
	return 0;
}


// Instruction: Transfer X Register to Stack Pointer
// Function:    stack pointer = X
uint8_t olc6502::TXS()
{
	stack_ptr = x;
	return 0;
}


// Instruction: Transfer Y Register to Accumulator
// Function:    A = Y
// Flags Out:   N, Z
uint8_t olc6502::TYA()
{
	accumulator = y;
	setFlag(Z, accumulator == 0x00);
	setFlag(N, accumulator & 0x80);
	return 0;
}


// This function captures illegal opcodes
uint8_t olc6502::XXX()
{
	return 0;
}





void olc6502::reset() {
	accumulator = 0;
	x = 0;
	y = 0;
	stack_ptr = 0xFD;
	status_reg = 0x00 | U;

	// Program Counter need not to be set to zero

	addr_abs = 0xFFFC;
	uint16_t low = read(addr_abs);
	uint16_t high = read(addr_abs + 1);

	pc = (high << 8) | low;

	addr_rel = 0x0000;
	addr_abs = 0x0000;
	fetched = 0x0000;

	//Reset takes time
	cycles = 8;
}

void olc6502::irq() {

	if (getFlag(I) == 0) {

		write(0x0100 + stack_ptr, (pc >> 8) & 0x00FF);
		stack_ptr--;
		write(0x0100 + stack_ptr, pc & 0x00FF);
		stack_ptr--;

		// Then Push the status register to the stack
		setFlag(B, 0);
		setFlag(U, 1);
		setFlag(I, 1);
		write(0x0100 + stack_ptr, status_reg);
		stack_ptr--;

		// Read new program counter location from fixed address
		addr_abs = 0xFFFE;
		uint16_t lo = read(addr_abs + 0);
		uint16_t hi = read(addr_abs + 1);
		pc = (hi << 8) | lo;

		// IRQs take time
		cycles = 7;
	}
}

void olc6502::nmi() {

	if (getFlag(I) == 0) {

		write(0x0100 + stack_ptr, (pc >> 8) & 0x00FF);
		stack_ptr--;
		write(0x0100 + stack_ptr, pc & 0x00FF);
		stack_ptr--;

		// Then Push the status register to the stack
		setFlag(B, 0);
		setFlag(U, 1);
		setFlag(I, 1);
		write(0x0100 + stack_ptr, status_reg);
		stack_ptr--;

		// Read new program counter location from fixed address
		addr_abs = 0xFFFA;
		uint16_t lo = read(addr_abs + 0);
		uint16_t hi = read(addr_abs + 1);
		pc = (hi << 8) | lo;

		// NMIs take time
		cycles = 8;
	}
}




bool olc6502::complete()
{
	return cycles == 0;
}

// This is the disassembly function. Its workings are not required for emulation.
// It is merely a convenience function to turn the binary instruction code into
// human readable form. Its included as part of the emulator because it can take
// advantage of many of the CPUs internal operations to do this.
std::map<uint16_t, std::string> olc6502::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with 
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = bus->cpuRead(addr, true); addr++;
		sInst += lookup_tbl[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (lookup_tbl[opcode].addrmode == &olc6502::IMP)
		{
			sInst += " {IMP}";
		}
		else if (lookup_tbl[opcode].addrmode == &olc6502::IMM)
		{
			value = bus->cpuRead(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (lookup_tbl[opcode].addrmode == &olc6502::ZP0)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (lookup_tbl[opcode].addrmode == &olc6502::ZPX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (lookup_tbl[opcode].addrmode == &olc6502::ZPY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (lookup_tbl[opcode].addrmode == &olc6502::IZX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (lookup_tbl[opcode].addrmode == &olc6502::IZY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (lookup_tbl[opcode].addrmode == &olc6502::ABS)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (lookup_tbl[opcode].addrmode == &olc6502::ABX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (lookup_tbl[opcode].addrmode == &olc6502::ABY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (lookup_tbl[opcode].addrmode == &olc6502::IND)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (lookup_tbl[opcode].addrmode == &olc6502::REL)
		{
			value = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + (int8_t)value, 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}
