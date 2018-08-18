#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <array>
#include <string_view>
#include "common.hpp"

struct Flags
{
	bool z = false, n = false, h = false, c = false;
};

class RegisterFile
{
	static R8& low(R16& reg)  { return reinterpret_cast<R8*>(&reg)[0]; }
	static R8& high(R16& reg) { return reinterpret_cast<R8*>(&reg)[1]; }

public:

	R16 af, bc, de, hl, sp, pc;
	R8 &a{high(af)}, &f{low(af)},
	   &b{high(bc)}, &c{low(bc)},
	   &d{high(de)}, &e{low(de)},
	   &h{high(hl)}, &l{low(hl)};

	void dump();

	void set_flags(Flags flags);
	Flags get_flags();
};

inline void RegisterFile::set_flags(Flags flags)
{
	const auto [z, n, h, c] = flags;

	// It is okay to override the 4 lower bits because they're assumed to be always zero.
	// TODO: enforce the 4 lower bits of F to be zero all the time.
	f = (z << 7) | (n << 6) | (h << 5) | (c << 4);
}

inline Flags RegisterFile::get_flags()
{
	return {
		.z = (f & (1 << 7)) != 0,
		.n = (f & (1 << 6)) != 0,
		.h = (f & (1 << 5)) != 0,
		.c = (f & (1 << 4)) != 0
	};
}


#endif // REGISTER_HPP
