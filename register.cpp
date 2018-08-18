#include "register.hpp"

#include <iostream>
#include <iomanip>
#include "debug.hpp"

void RegisterFile::dump()
{
	const auto [z, n, h, c] = get_flags();

	std::cout << "AF: " << debug_hex(af) << " (z=" << z << ", n=" << n << ", h=" << h << ", c=" << c << ")\n";
	std::cout << "BC: " << debug_hex(bc) << '\n';
	std::cout << "DE: " << debug_hex(de) << '\n';
	std::cout << "HL: " << debug_hex(hl) << '\n';
	std::cout << "SP: " << debug_hex(sp) << '\n';
	std::cout << "PC: " << debug_hex(pc) << '\n';

	std::cout << '\n';
}


