#ifndef OPCODES_HPP
#define OPCODES_HPP

#include <functional>
#include <string_view>
#include <iostream>
#include "cpu.hpp"

struct Opcode
{
	//! Instruction mnemonic.
	std::string_view mnemonic = "(unimplemented)";

	using Runner = void(*)(CPU&);

	//! Ran by CPU::execute when the opcode matches.
	Runner run = [](CPU&){ throw std::runtime_error("Unimplemented opcode"); };
};

namespace ops
{
// MEMORY READS
// 8-bit register reads
inline R8 a(CPU& cpu) { return cpu.regs.a; }
inline R8 b(CPU& cpu) { return cpu.regs.b; }
inline R8 c(CPU& cpu) { return cpu.regs.c; }
inline R8 d(CPU& cpu) { return cpu.regs.d; }
inline R8 e(CPU& cpu) { return cpu.regs.e; }
inline R8 h(CPU& cpu) { return cpu.regs.h; }
inline R8 l(CPU& cpu) { return cpu.regs.l; }

// 16-bit register reads
inline R16 hl(CPU& cpu)    { return cpu.regs.hl; }

// 8-bit immediate reads
inline R8 d8(CPU& cpu)          { return cpu.fetch_pc_byte(1); }

// 16-bit immediate reads
inline R16 d16(CPU& cpu)        { return cpu.fetch_pc_word(1); }

// 8-bit memory reads
inline R8 mem_high_d8(CPU& cpu) { return cpu.mem_fetch(0xFF00 + d8(cpu)); }

// MEMORY WRITES
// 8-bit register writes
inline void a(CPU& cpu, R8 value) { cpu.regs.a = value; }
inline void b(CPU& cpu, R8 value) { cpu.regs.b = value; }
inline void c(CPU& cpu, R8 value) { cpu.regs.c = value; }
inline void d(CPU& cpu, R8 value) { cpu.regs.d = value; }
inline void e(CPU& cpu, R8 value) { cpu.regs.e = value; }
inline void h(CPU& cpu, R8 value) { cpu.regs.h = value; }
inline void l(CPU& cpu, R8 value) { cpu.regs.l = value; }

// 16-bit register writes
inline void hl(CPU& cpu, R16 value) { cpu.regs.hl = value; }

// 8-bit memory writes
inline void mem_hl(CPU& cpu, R8 value)     { cpu.mem_write(cpu.regs.hl, value);}
inline void mem_hl_inc(CPU& cpu, R8 value) { cpu.mem_write(cpu.regs.hl++, value); }
inline void mem_hl_dec(CPU& cpu, R8 value) { cpu.mem_write(cpu.regs.hl--, value); }
inline void mem_high_d8(CPU& cpu, R8 value) { return cpu.mem_write(0xFF00 + d8(cpu), value); }

// BRANCHING
inline bool branch_always(CPU&)  { return true; }
inline bool branch_z(CPU& cpu)   { return cpu.regs.get_flags().z; }
inline bool branch_nz(CPU& cpu)  { return !cpu.regs.get_flags().z; }
inline bool branch_c(CPU& cpu)   { return cpu.regs.get_flags().c; }
inline bool branch_nc(CPU& cpu)  { return !cpu.regs.get_flags().c; }

template<R16(&src)(CPU&), size_t cycles_branch, bool(&cond)(CPU&) = branch_always, size_t cycles_no_branch = 0>
void ijp(CPU& cpu)
{
	auto target = src(cpu);

	if (cond(cpu))
	{
		cpu.next_pc = target;
		cpu.internal_cycle_counter += cycles_branch;
	}
	else
	{
		cpu.internal_cycle_counter += cycles_no_branch;
	}
}

template<size_t cycles_branch, bool(&cond)(CPU&) = branch_always, size_t cycles_no_branch = 0>
void ijr(CPU& cpu)
{
	auto off = static_cast<int8_t>(d8(cpu));

	if (cond(cpu))
	{
		cpu.next_pc = cpu.regs.pc + (off + 2);
		cpu.internal_cycle_counter += cycles_branch;
	}
	else
	{
		cpu.internal_cycle_counter += cycles_no_branch;
	}
}

template<R8(&src)(CPU&), size_t cycles>
void ixor(CPU& cpu)
{
	cpu.regs.a ^= src(cpu);
	cpu.internal_cycle_counter += cycles;
	cpu.regs.set_flags({.z = cpu.regs.a == 0});
}

template<class Reg, void(&dst_setter)(CPU&, Reg), Reg(&src)(CPU&), size_t cycles>
void ild(CPU& cpu)
{
	dst_setter(cpu, src(cpu));
	cpu.internal_cycle_counter += cycles;
}

template<class Reg, void(&dst_setter)(CPU&, Reg), Reg(&src)(CPU&), size_t cycles>
void idec(CPU& cpu)
{
	dst_setter(cpu, src(cpu) - 1);
	cpu.internal_cycle_counter += cycles;
	cpu.regs.set_flags({
		.z = src(cpu) == 0,
		.n = 1,
		.h = /* TODO */0
	});

	std::cout << "Stub: half-carry\n";
}

inline void idi(CPU& cpu)
{
	cpu.ime = false;
}

template<R8(&src)(CPU&), size_t cycles>
void icp(CPU& cpu)
{
	auto diff = cpu.regs.a - src(cpu);

	cpu.regs.set_flags({
		.z = diff == 0,
		.n = 1,
		.h = /* TODO */0,
		.c = /* TODO */0
	});

	std::cout << "Stub: half-carry, carry\n";
}

constexpr auto generate_opcode_list()
{
	Opcode ibad{"(bad)", [](CPU&) { throw std::runtime_error("Bad opcode"); }};
	Opcode inop{"nop", [](CPU&){}};

	std::array<Opcode, 256> ops{};

	auto mk = [&ops](size_t offset, Opcode value) {
		if (ops[offset].mnemonic != "(unimplemented)")
		{
			throw std::logic_error("Error: Tried to assign opcode twice in opcode generator!");
		}

		ops[offset] = value;
	};

	// Nop
	mk(0x00, inop);
	mk(0x05, {"DEC B", idec<R8, b, b, 4>});
	mk(0x06, {"LD B,d8", ild<R8, b, d8, 8>});
	mk(0x0D, {"DEC C", idec<R8, c, c, 4>});
	mk(0x0E, {"LD C,d8", ild<R8, c, d8, 8>});

	mk(0x20, {"JR NZ,r8", ijr<12, branch_nz, 8>});
	mk(0x21, {"LD HL,d16", ild<R16, hl, d16, 12>});

	mk(0x32, {"LD (HL-),A", ild<R8, mem_hl_dec, a, 8>});
	mk(0x3E, {"LD a,d8", ild<R8, a, d8, 8>});

	mk(0xAF, {"XOR A", ixor<a, 4>});

	mk(0xC3, {"JP a16", ijp<d16, 16>});

	mk(0xD3, ibad);
	mk(0xDB, ibad);
	mk(0xDD, ibad);

	mk(0xE0, {"LDH (a8),A", ild<R8, mem_high_d8, a, 12>});
	mk(0xE3, ibad);
	mk(0xE4, ibad);
	mk(0xEB, ibad);
	mk(0xEC, ibad);
	mk(0xED, ibad);

	mk(0xF0, {"LDH A,(a8)", ild<R8, a, mem_high_d8, 12>});
	mk(0xF3, {"DI", idi});
	mk(0xF4, ibad);
	mk(0xFC, ibad);
	mk(0xFD, ibad);
	mk(0xFE, {"CP d8", icp<d8, 8>});

	return ops;
}

}

constexpr auto opcodes = ops::generate_opcode_list();

#endif // OPCODES_HPP
