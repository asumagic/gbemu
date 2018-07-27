#ifndef OPCODES_COMMON_HPP
#define OPCODES_COMMON_HPP

#include "../opcodes.hpp"

namespace opcode
{
using Ops = std::array<Opcode, 256>;

enum class OperandType
{
	reg8,
	reg16,

	imm8,
	imm16,

	mem_bc,
	mem_de,
	mem_hl,
	mem_hl_postinc,
	mem_hl_postdec,
	mem_sp,

	mem_imm16
};

template<OperandType type, unsigned reg_id = 0>
auto read_from(CPU& cpu)
{
	if constexpr (type == OperandType::reg8)
	{
		return *cpu.regs.reg8(reg_id);
	}

	if constexpr (type == OperandType::reg16)
	{
		cpu.cycles_taken += 4;
		return *cpu.regs.reg16(reg_id);
	}

	if constexpr (type == OperandType::imm8)
	{
		cpu.next_pc += 1;
		cpu.cycles_taken += 4;
		return cpu.fetch_pc_byte(1);
	}

	if constexpr (type == OperandType::imm16)
	{
		cpu.next_pc += 2;
		cpu.cycles_taken += 4;
		return cpu.fetch_pc_byte(1);
	}

	if constexpr (type == OperandType::mem_bc)
	{
		cpu.cycles_taken += 4;
		return cpu.mem_fetch(cpu.regs.bc);
	}

	if constexpr (type == OperandType::mem_de)
	{
		cpu.cycles_taken += 4;
		return cpu.mem_fetch(cpu.regs.de);
	}

	if constexpr (type == OperandType::mem_hl)
	{
		cpu.cycles_taken += 4;
		return cpu.mem_fetch(cpu.regs.hl);
	}

	if constexpr (type == OperandType::mem_hl_postinc)
	{
		cpu.cycles_taken += 4;
		return cpu.mem_fetch(cpu.regs.hl++);
	}

	if constexpr (type == OperandType::mem_hl_postdec)
	{
		cpu.cycles_taken += 4;
		return cpu.mem_fetch(cpu.regs.hl--);
	}

	if constexpr (type == OperandType::mem_sp)
	{
		cpu.cycles_taken += 4;
		return cpu.mem_fetch(cpu.regs.sp);
	}

	if constexpr (type == OperandType::mem_imm16)
	{
		cpu.next_pc += 2;
		cpu.cycles_taken += 4;
		return cpu.mem_fetch(cpu.fetch_pc_word(1));
	}
}

template<OperandType type, unsigned reg_id = 0>
void write_to(CPU& cpu, uint16_t data)
{
	if constexpr (type == OperandType::reg8)
	{
		*cpu.regs.reg8(reg_id) = data;
	}

	if constexpr (type == OperandType::reg16)
	{
		cpu.cycles_taken += 4;
		*cpu.regs.reg16(reg_id) = data;
	}

	if constexpr (type == OperandType::imm8)
	{
		throw std::logic_error("Cannot write to 8-bit immediate");
	}

	if constexpr (type == OperandType::imm16)
	{
		throw std::logic_error("Cannot write to 16-bit immediate");
	}

	if constexpr (type == OperandType::mem_bc)
	{
		cpu.cycles_taken += 4;
		cpu.mem_write(cpu.regs.bc, data);
	}

	if constexpr (type == OperandType::mem_de)
	{
		cpu.cycles_taken += 4;
		cpu.mem_write(cpu.regs.de, data);
	}

	if constexpr (type == OperandType::mem_hl)
	{
		cpu.cycles_taken += 4;
		cpu.mem_write(cpu.regs.hl, data);
	}

	if constexpr (type == OperandType::mem_hl_postinc)
	{
		cpu.cycles_taken += 4;
		cpu.mem_write(cpu.regs.hl++, data);
	}

	if constexpr (type == OperandType::mem_hl_postdec)
	{
		cpu.cycles_taken += 4;
		cpu.mem_write(cpu.regs.hl--, data);
	}

	if constexpr (type == OperandType::mem_sp)
	{
		cpu.cycles_taken += 4;
		cpu.mem_write(cpu.regs.sp, data);
	}

	if constexpr (type == OperandType::mem_imm16)
	{
		cpu.next_pc += 2;
		cpu.cycles_taken += 12;
		cpu.mem_write(cpu.fetch_pc_word(1), data);
	}
}

constexpr unsigned src(size_t i)
{
	return named_regs8[i % 7].id;
}

constexpr unsigned dst(size_t i)
{
	return named_regs8[i / 7].id;
}

constexpr uint8_t make_binop_reg8(uint8_t cat, size_t i)
{
	return (cat << 6) | (src(i) << 3) | dst(i);
}
}

#endif // OPCODES_COMMON_HPP
