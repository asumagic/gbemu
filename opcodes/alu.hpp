#ifndef OPCODES_ALU_HPP
#define OPCODES_ALU_HPP

#include "common.hpp"

namespace opcode
{
template<OperandType type, class Op, unsigned src_id = 0>
void alu(CPU& cpu)
{
	// TODO: update flags
	cpu.regs.a = Op()(cpu.regs.a, read_from<type, src_id>(cpu));

}

constexpr uint8_t make_alu_op_reg8(uint8_t alu_cat, size_t i)
{
	return (0b10 << 6) | (alu_cat << 3) | src(i);
}

constexpr uint8_t make_alu_op_hl(uint8_t alu_cat)
{
	return (0b10 << 6) | (alu_cat << 3) | 0b110;
}

template<std::size_t... Is>
constexpr void create_al_reg8_opcodes(Ops& ops, std::index_sequence<Is...>)
{
	constexpr OperandType type = OperandType::reg8;
	((ops[make_alu_op_reg8(0b000, Is)] = { "add", alu<type, std::plus<R8>, src(Is)> }), ...);
	((ops[make_alu_op_reg8(0b010, Is)] = { "sub", alu<type, std::minus<R8>, src(Is)> }), ...);
	((ops[make_alu_op_reg8(0b100, Is)] = { "and", alu<type, std::bit_and<R8>, src(Is)> }), ...);
	((ops[make_alu_op_reg8(0b101, Is)] = { "xor", alu<type, std::bit_xor<R8>, src(Is)> }), ...);
	((ops[make_alu_op_reg8(0b110, Is)] = { "or", alu<type, std::bit_xor<R8>, src(Is)> }), ...);
}

template<OperandType type>
constexpr void create_al_custom_opcodes(Ops& ops)
{
	ops[make_alu_op_hl(0b000)] = { "add", alu<type, std::plus<R8>> };
	ops[make_alu_op_hl(0b010)] = { "sub", alu<type, std::minus<R8>> };
	ops[make_alu_op_hl(0b100)] = { "and", alu<type, std::bit_and<R8>> };
	ops[make_alu_op_hl(0b101)] = { "xor", alu<type, std::bit_xor<R8>> };
	ops[make_alu_op_hl(0b110)] = { "or", alu<type, std::bit_xor<R8>> };
}

constexpr void create_al_opcodes(Ops& ops)
{
	create_al_reg8_opcodes(ops, std::make_index_sequence<7u>());
	create_al_custom_opcodes<OperandType::mem_hl>(ops);
}
}

#endif // OPCODES_ALU_HPP
