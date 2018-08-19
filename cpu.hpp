#ifndef CPU_HPP
#define CPU_HPP

#include <array>
#include <vector>
#include "romfile.hpp"
#include "register.hpp"
#include "video.hpp"
#include "io.hpp"

class CPU
{
	void regs_init();
	void ram_init();

public:
	using Memory = std::vector<unsigned char>;
	Memory ram;
	const Memory* rom = nullptr;

	CPU();

	//! Sets the PC after an instruction handler has been run.
	//! On every new instruction, set to `regs.pc + 1`.
	R16 next_pc;

	//! Increments the cycle counter after an insturction handler has been run.
	R16 internal_cycle_counter;

	RegisterFile regs;

	Video video;

	//! Interrupt Master Enable. Controls all interrupts.
	bool ime;

	void boot(const ROMFile& file);

	R8 mem_fetch(Addr address);
	void mem_write(Addr address, R8 data);

	R8 port_read(R16 port);
	void port_write(R16 port, R8 data);
	R8& port_ram(Port port);

	R8 fetch_pc_byte(unsigned off = 0);
	R16 fetch_pc_word(unsigned off = 0);

	void execute();
};

#endif // CPU_HPP
