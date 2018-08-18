#include "cpu.hpp"
#include <array>
#include <iostream>
#include <utility>
#include "debug.hpp"
#include "opcodes.hpp"

void CPU::regs_init()
{
   regs.af = 0x01B0;
   regs.bc = 0x0013;
   regs.de = 0x00D8;
   regs.hl = 0x014D;
   regs.sp = 0xFFFE;
   regs.pc = 0x0100;
}

void CPU::ram_init()
{
	ram.clear();
	ram.resize(1 << 16);

	constexpr std::array<std::pair<uint16_t, uint8_t>, 30> to_set
	{{
		{0xFF05, 0x00}, // TIMA
		{0xFF07, 0x00}, // TMA
		{0xFF10, 0x00}, // TAC
		{0xFF11, 0xBF}, // NR11
		{0xFF12, 0xF3}, // NR12
		{0xFF14, 0xBF}, // NR14
		{0xFF16, 0x3F}, // NR21
		{0xFF17, 0x00}, // NR22
		{0xFF19, 0xBF}, // NR24
		{0xFF1A, 0x7F}, // NR30
		{0xFF1B, 0xFF}, // NR31
		{0xFF1C, 0x9F}, // NR32
		{0xFF1E, 0xBF}, // NR33
		{0xFF20, 0xFF}, // NR41
		{0xFF21, 0x00}, // NR42
		{0xFF22, 0x00}, // NR43
		{0xFF23, 0xBF}, // NR30
		{0xFF24, 0x77}, // NR50
		{0xFF25, 0xF3}, // NR51
		// TODO: [$FF26] = $F1-GB, $F0-SGB ; NR52
		{0xFF26, 0xF1}, // NR52
		{0xFF40, 0x91}, // LCDC
		{0xFF42, 0x00}, // SCY
		{0xFF43, 0x00}, // SCX
		{0xFF43, 0x00}, // LYC
		{0xFF47, 0xFC}, // BGP
		{0xFF48, 0xFF}, // OBP0
		{0xFF49, 0xFF}, // OBP1
		{0xFF4A, 0x00}, // WY
		{0xFF4B, 0x00}, // WX
		{0xFFFF, 0x00}, // IE
	}};

	for (auto p : to_set)
	{
		ram[p.first] = p.second;
	}
}

R8 CPU::fetch_pc_byte(unsigned off)
{
	auto data = mem_fetch(regs.pc + off);

	next_pc += 1;

	return data;
}

R16 CPU::fetch_pc_word(unsigned off)
{
	R16 data = 0;
	data |= mem_fetch(regs.pc + off);
	data |= mem_fetch(regs.pc + off + 1) << 8;

	next_pc += 2;

	return data;
}

void CPU::boot(const ROMFile& file)
{
	std::cout << "Booting CPU with cartridge '" << file.app_name << "'...\n";

	regs_init();
	ram_init();
	rom = &file.rom;
}

uint8_t CPU::mem_fetch(Addr address)
{
	if (address <= 0x3FFF) // ROM bank 0
	{
		return rom->data()[address];
	}
	else if (address >= 0x4000 && address <= 0x7FFF) // ROM configurable bank
	{
		throw std::runtime_error("Unhandled memory: Extra ROM bank");
	}
	else if (address >= 0x8000 && address <= 0x9FFF)
	{
		throw std::runtime_error("Unhandled memory: VRAM");
	}
	else if (address >= 0xA000 && address <= 0xBFFF)
	{
		throw std::runtime_error("Unhandled memory: External RAM");
	}
	else if (address >= 0xC000 && address <= 0xCFFF)
	{
		return ram[address];
	}
	else if (address >= 0xD000 && address <= 0xDFFF)
	{
		//throw std::runtime_error("Unhandled memory: WRAM Bank #1 (CGB)");
		return ram[address];
	}
	else if (address >= 0xE000 && address <= 0xFDFF)
	{
		throw std::runtime_error("Unhandled memory: WRAM echo");
	}
	else if (address >= 0xFE00 && address <= 0xFE9F)
	{
		throw std::runtime_error("Unhandled memory: OAM");
	}
	else if (address >= 0xFEA0 && address <= 0xFEFF)
	{
		throw std::runtime_error("Unhandled memory: Dead area");
	}
	else if (address >= 0xFF00 && address <= 0xFF7F)
	{
		std::cout << "Stub: I/O memory\n";
		return ram[address];
	}
	else if (address >= 0xFF80 && address <= 0xFFFE)
	{
		return ram[address];
	}
	else
	{
		throw std::runtime_error("Unhandled memory: Unknown area");
	}
}

void CPU::mem_write(Addr address, R8 data)
{
	if (address <= 0x3FFF) // ROM bank 0
	{
		throw std::runtime_error("Memory violation: Tried to write to ROM");
	}
	else if (address >= 0x4000 && address <= 0x7FFF) // ROM configurable bank
	{
		throw std::runtime_error("Memory violation: Tried to write to ROM");
	}
	else if (address >= 0x8000 && address <= 0x9FFF)
	{
		throw std::runtime_error("Unhandled memory: VRAM");
	}
	else if (address >= 0xA000 && address <= 0xBFFF)
	{
		throw std::runtime_error("Unhandled memory: External RAM");
	}
	else if (address >= 0xC000 && address <= 0xCFFF)
	{
		ram[address] = data;
	}
	else if (address >= 0xD000 && address <= 0xDFFF)
	{
		//throw std::runtime_error("Unhandled memory: WRAM Bank #1 (CGB)");
		ram[address] = data;
	}
	else if (address >= 0xE000 && address <= 0xFDFF)
	{
		throw std::runtime_error("Unhandled memory: WRAM echo");
	}
	else if (address >= 0xFE00 && address <= 0xFE9F)
	{
		throw std::runtime_error("Unhandled memory: OAM");
	}
	else if (address >= 0xFEA0 && address <= 0xFEFF)
	{
		throw std::runtime_error("Unhandled memory: Dead area");
	}
	else if (address >= 0xFF00 && address <= 0xFF7F)
	{
		std::cout << "Stub: I/O memory\n";
		ram[address] = data;
	}
	else if (address >= 0xFF80 && address <= 0xFFFE)
	{
		ram[address] = data;
	}
	else
	{
		throw std::runtime_error("Unhandled memory: Unknown area");
	}
}

void CPU::execute()
{
	try
	{
		for (;;)
		{
			next_pc = regs.pc;
			const Opcode op = opcodes[fetch_pc_byte()];
			cycles_taken = 0;
			std::cout << "Op: " << op.mnemonic << "\n";
			op.run(*this);
			regs.pc = next_pc;
		}
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "The emulator has crashed. " << e.what() << '\n';

		std::cout << "\nDumping emulator state...\n";
		regs.dump();

		std::cout << "Memory at PC: " << debug_hex(rom->data()[regs.pc]) << "\n\n";
	}
}
