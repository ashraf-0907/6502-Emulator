#pragma once
#include <cstdint>
#include <array>

#include "olc6502.h"

class Bus
{
public:
	Bus();
	~Bus();

public: // Devices on Main Bus

	// The 6502 derived processor
	olc6502 cpu;

	// 2KB of RAM
	std::array<uint8_t, 64 * 1024> cpuRam;

public: // Main Bus Read & Write
	void    cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

private:

public: // System Interface
	// Resets the system
	void reset();
	// Clocks the system - a single whole systme tick
	void clock();
};
