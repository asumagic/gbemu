#include "register.hpp"

#include <iostream>
#include <iomanip>
#include "debug.hpp"

void RegisterFile::dump()
{
	std::cout << "AF: " << debug_hex(af) << '\n';
	std::cout << "BC: " << debug_hex(bc) << '\n';
	std::cout << "DE: " << debug_hex(de) << '\n';
	std::cout << "HL: " << debug_hex(hl) << '\n';
	std::cout << "SP: " << debug_hex(sp) << '\n';
	std::cout << "PC: " << debug_hex(pc) << '\n';

	std::cout << '\n';
}


