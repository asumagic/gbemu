#ifndef OPCODES_JUMP_HPP
#define OPCODES_JUMP_HPP

#include "common.hpp"

namespace opcode
{
inline void jp(CPU& cpu)
{
	cpu.next_pc = cpu.fetch_pc_word(1);
}

constexpr void create_jp_opcodes(Ops& ops)
{
	ops[0xC3] = { "jp", jp };
}
}

#endif // OPCODES_JUMP_HPP
