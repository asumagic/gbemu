#ifndef OPCODES_LD_HPP
#define OPCODES_LD_HPP

#include "common.hpp"

namespace opcode
{
template<OperandType src_t, OperandType dst_t, unsigned src_id, unsigned dst_id>
void ld(CPU& cpu)
{
	write_to<dst_t, dst_id>(cpu, read_from<src_t, src_id>(cpu));
}

template<std::size_t... Is>
constexpr void create_ld_reg8_reg8_opcodes(Ops& ops, std::index_sequence<Is...>)
{
	((ops[make_binop_reg8(0b01, Is)] = { "ld", ld<OperandType::reg8, OperandType::reg8, src(Is), dst(Is)> }), ...);
}

template<std::size_t... Is>
constexpr void create_ld_memhl_opcodes(Ops& ops, std::index_sequence<Is...>)
{
	((ops[(0b01 << 6) | (named_regs8[Is].id << 3) | 0b110] = { "ld", ld<OperandType::mem_hl, OperandType::reg8, 0, named_regs8[Is].id> }), ...);
	((ops[(0b01 << 6) | (0b110 << 3) | named_regs8[Is].id] = { "ld", ld<OperandType::reg8, OperandType::mem_hl, named_regs8[Is].id, 0> }), ...);
}

template<std::size_t... Is>
constexpr void create_ld_reg16_imm16_opcodes(Ops& ops, std::index_sequence<Is...>)
{
	((ops[(named_regs16[Is].id << 3) | 0b001] = { "ld", ld<OperandType::imm16, OperandType::reg16, 0, named_regs16[Is].id> }), ...);
}

template<std::size_t... Is>
constexpr void create_ld_reg8_imm8_opcodes(Ops& ops, std::index_sequence<Is...>)
{
	((ops[(named_regs8[Is].id << 3) | 0b110] = { "ld", ld<OperandType::imm8, OperandType::reg8, 0, named_regs8[Is].id> }), ...);
}

constexpr void create_ld_opcodes(Ops& ops)
{
	create_ld_reg8_reg8_opcodes(ops, std::make_index_sequence<7u * 7u>());
	create_ld_memhl_opcodes(ops, std::make_index_sequence<7u>());
	create_ld_reg16_imm16_opcodes(ops, std::make_index_sequence<4u>());
	create_ld_reg8_imm8_opcodes(ops, std::make_index_sequence<7u>());
}
}

#endif // OPCODES_LD_HPP
