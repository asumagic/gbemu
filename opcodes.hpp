#ifndef OPCODES_HPP
#define OPCODES_HPP

#include <functional>
#include <string_view>

class CPU;

struct Opcode
{
	//! Instruction mnemonic.
	std::string_view mnemonic = "(unimplemented)";

	using Runner = void(*)(CPU&);

	//! Ran by CPU::execute when the opcode matches.
	Runner run = [](CPU&){ throw std::runtime_error("Unimplemented opcode"); };
};

#include "opcodes/ld.hpp"
#include "opcodes/alu.hpp"
#include "opcodes/jump.hpp"
#include "opcodes/bad.hpp"

constexpr auto generate_opcode_list()
{
	std::array<Opcode, 256> ops{};

	ops[0x00] = { "nop", [](CPU&){} };

	opcode::create_ld_opcodes(ops);
	opcode::create_jp_opcodes(ops);
	opcode::create_al_opcodes(ops);
	opcode::create_bad_opcodes(ops);

	return ops;
}

constexpr auto opcodes = generate_opcode_list();

#endif // OPCODES_HPP
