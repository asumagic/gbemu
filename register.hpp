#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <array>
#include <string_view>
#include "common.hpp"

struct RegisterName
{
	std::string_view name;
	unsigned id;
};

constexpr std::array<RegisterName, 7> named_regs8 {{
	{"b", 0b000},
	{"c", 0b001},
	{"d", 0b010},
	{"e", 0b011},
	{"h", 0b100},
	{"l", 0b101},
	{"a", 0b111},
}};

constexpr std::array<RegisterName, 4> named_regs16 {{
	{"bc", 0b000},
	{"de", 0b010},
	{"hl", 0b100},
	{"sp", 0b110}
}};

class RegisterFile
{
	static R8& low(R16& reg)  { return reinterpret_cast<R8*>(&reg)[0]; }
	static R8& high(R16& reg) { return reinterpret_cast<R8*>(&reg)[1]; }

public:

	R16 af, bc, de, hl, sp, pc;
	R8 &a{high(af)}, /*&f{low(af)},*/
	   &b{high(bc)}, &c{low(bc)},
	   &d{high(de)}, &e{low(de)},
	   &h{high(hl)}, &l{low(hl)};

	void dump();

	R8* reg8(unsigned id)
	{
		switch (id)
		{
		case 0b111: return &a;
		case 0b000: return &b;
		case 0b001: return &c;
		case 0b010: return &d;
		case 0b011: return &e;
		case 0b100: return &h;
		case 0b101: return &l;
		default: return nullptr;
		}
	}

	R16* reg16(unsigned id)
	{
		switch (id)
		{
		case 0b000: return &bc;
		case 0b010: return &de;
		case 0b100: return &hl;
		case 0b110: return &sp;
		default: return nullptr;
		}
	}
};

#endif // REGISTER_HPP
