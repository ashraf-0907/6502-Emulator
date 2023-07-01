#include "Bus.h"



Bus::Bus()
{
	// Connect CPU to communication bus
	cpu.ConnectBus(this);

	// Clear RAM contents, just in case :P
	for (auto& i : cpuRam) i = 0x00;
}


Bus::~Bus()
{
}

void Bus::cpuWrite(uint16_t addr, uint8_t data)
{
	if (addr >= 0x0000 && addr <= 0xFFFF)
		cpuRam[addr] = data;
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly)
{
	if (addr >= 0x0000 && addr <= 0xFFFF)
		return cpuRam[addr];

	return 0x00;
}