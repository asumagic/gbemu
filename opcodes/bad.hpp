#ifndef OPCODES_BAD_HPP
#define OPCODES_BAD_HPP

#include "common.hpp"

namespace opcode
{
constexpr void create_bad_opcodes(Ops& ops)
{
	for (auto i : {0xD3, 0xDB, 0xDD, 0xE3, 0xE4, 0xEB, 0xEC, 0xED, 0xF4, 0xFC, 0xFD})
	{
		ops[i] = { "(bad)", [](CPU&) { throw std::runtime_error("Bad opcode"); } };
	}
}
}

#endif // OPCODES_BAD_HPP
