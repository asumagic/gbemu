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

	/*   [$FF05] = $00   ; TIMA
   [$FF06] = $00   ; TMA
   [$FF07] = $00   ; TAC
   [$FF10] = $80   ; NR10
   [$FF11] = $BF   ; NR11
   [$FF12] = $F3   ; NR12
   [$FF14] = $BF   ; NR14
   [$FF16] = $3F   ; NR21
   [$FF17] = $00   ; NR22
   [$FF19] = $BF   ; NR24
   [$FF1A] = $7F   ; NR30
   [$FF1B] = $FF   ; NR31
   [$FF1C] = $9F   ; NR32
   [$FF1E] = $BF   ; NR33
   [$FF20] = $FF   ; NR41
   [$FF21] = $00   ; NR42
   [$FF22] = $00   ; NR43
   [$FF23] = $BF   ; NR30
   [$FF24] = $77   ; NR50
   [$FF25] = $F3   ; NR51
   [$FF26] = $F1-GB, $F0-SGB ; NR52
   [$FF40] = $91   ; LCDC
   [$FF42] = $00   ; SCY
   [$FF43] = $00   ; SCX
   [$FF45] = $00   ; LYC
   [$FF47] = $FC   ; BGP
   [$FF48] = $FF   ; OBP0
   [$FF49] = $FF   ; OBP1
   [$FF4A] = $00   ; WY
   [$FF4B] = $00   ; WX
   [$FFFF] = $00   ; IE*/

   // TODO: entire ram init

	constexpr std::array<std::pair<uint16_t, uint8_t>, 20> to_set
	{{
		{0xFF05, 0x00}, // TIMA
		{0xFF07, 0x00}, // TMA
		{0xFF07, 0x00}, // TAC
	}};

	for (auto p : to_set)
	{
		ram[p.first] = p.second;
	}
}

R8 CPU::fetch_pc_byte(unsigned off)
{
	return mem_fetch(regs.pc + off);
}

R16 CPU::fetch_pc_word(unsigned off)
{
	R16 data = 0;
	data |= mem_fetch(regs.pc + off);
	data |= mem_fetch(regs.pc + off + 1) << 8;
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
		throw std::runtime_error("Unhandled memory: WRAM Bank #1 (CGB)");
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
		throw std::runtime_error("Unhandled memory: WRAM Bank #1 (CGB)");
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
			const Opcode op = opcodes[fetch_pc_byte()];
			next_pc = regs.pc + 1;
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
