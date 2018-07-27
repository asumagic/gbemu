#include "romfile.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <fstream>
#include <sstream>
#include <string_view>
#include <iomanip>

#include "debug.hpp"

ROMFile::ROMFile(const char* fname)
{
	std::ifstream file{fname, std::ios::binary};
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	rom = {std::istreambuf_iterator{file}, {}};

	app_name = std::string_view(&signed_data()[0x0134], 11);
}

unsigned char* ROMFile::data()
{
	return rom.data();
}

char* ROMFile::signed_data()
{
	return reinterpret_cast<char*>(rom.data());
}

void ROMFile::dump_header() const
{
	std::cout << "Dumping ROM header...\n\n";

	std::cout << "Entry point:   " << as_hex(0x0100, 0x0103) << '\n';
	std::cout << "Nintendo logo: " << as_hex(0x0104, 0x0133) << '\n';
	std::cout << "Title:         " << as_hex(0x0134, 0x0143) << '\n';
	std::cout << "Manufacturer:  " << as_hex(0x013F, 0x0142) << '\n';
	std::cout << "Color GB flag: " << as_hex(0x0143) << '\n';
	std::cout << "Licensee code: " << as_hex(0x0144, 0x0145) << '\n';
	std::cout << "Super GB flag: " << as_hex(0x0146) << '\n';
	std::cout << "Cart type:     " << as_hex(0x0147) << '\n';
	std::cout << "ROM size:      " << as_hex(0x0148) << '\n';
	std::cout << "RAM size:      " << as_hex(0x0149) << '\n';
	std::cout << "Dest. code:    " << as_hex(0x014A) << '\n';
	std::cout << "Old lic. code: " << as_hex(0x014B) << '\n';
	std::cout << "ROM version:   " << as_hex(0x014C) << '\n';
	std::cout << "Header chksum: " << as_hex(0x014D) << '\n';
	std::cout << "Global chksum: " << as_hex(0x014E, 0x014F) << '\n';

	std::cout << "\nVerifying header checksum... "
			  << (verify_header_checksum() ? "OK\n" : "Mismatch\n");

	std::cout << "Verifying global checksum... "
			  << (verify_global_checksum() ? "OK\n" : "Mismatch\n");

	std::cout << '\n';
}

std::string ROMFile::as_hex(unsigned start, unsigned end) const
{
	std::stringstream ss;

	ss << std::hex << std::uppercase;

	for (size_t i = start; i != end + 1; ++i)
	{
		ss << std::setw(2)
		   << std::setfill('0')
		   << static_cast<unsigned>(rom[i]) << ' ';
	}

	return ss.str();
}

std::string ROMFile::as_hex(unsigned at) const
{
	return debug_hex(rom[at]);
}

uint8_t ROMFile::header_checksum() const
{
	uint8_t x = 0;

	for (int i = 0x0134; i <= 0x014C; ++i)
	{
		x = x - rom[i] - 1;
	}

	return x;
}

uint16_t ROMFile::global_checksum() const
{
	return std::accumulate(rom.begin(), rom.end(), -rom[0x014E] - rom[0x014F]);
}

bool ROMFile::verify_header_checksum() const
{
	return header_checksum() == rom[0x014D];
}

bool ROMFile::verify_global_checksum() const
{
	return global_checksum() == ((rom[0x014E] << 8) | rom[0x014F]);
}
