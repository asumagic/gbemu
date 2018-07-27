#ifndef CPU_HPP
#define CPU_HPP

#include <array>
#include <vector>
#include "romfile.hpp"
#include "register.hpp"

class CPU
{
	void regs_init();
	void ram_init();

public:
	using Memory = std::vector<unsigned char>;
	Memory ram;
	const Memory* rom = nullptr;

	//! Sets the PC after an instruction handler has been run.
	//! On every new instruction, set to `regs.pc + 1`.
	R16 next_pc;

	//! Increments the cycle counter after an insturction handler has been run.
	R16 cycles_taken;

	RegisterFile regs;

	void boot(const ROMFile& file);

	uint8_t mem_fetch(Addr address);
	void mem_write(Addr address, R8 data);

	R8 fetch_pc_byte(unsigned off = 0);
	R16 fetch_pc_word(unsigned off = 0);

	void execute();
};

#endif // CPU_HPP
