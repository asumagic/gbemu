#include "cpu.hpp"
#include "opcodes.hpp"
#include "debug.hpp"

#include <iostream>

int main()
{
	size_t counter = 0;
	for (size_t i = 0; i <= 0xFF; ++i)
	{
		const Opcode& opcode = opcodes[i];
		std::cout << "0x" << debug_hex(uint8_t(i)) << ": '" << opcode.mnemonic << "'\n";
		if (opcode.mnemonic != "(unimplemented)")
		{
			++counter;
		}
	}
	std::cout << "ISA is implemented at " << (100 * counter) / 256 << "%\n";

	ROMFile rom{"tetris.gb"};
	rom.dump_header();

	CPU cpu;
	cpu.boot(rom);
	cpu.execute();
}
