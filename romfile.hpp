#ifndef ROMFILE_HPP
#define ROMFILE_HPP

#include <vector>
#include <string>

struct ROMFile
{
	ROMFile(const char* fname);

	// Raw data
	std::vector<unsigned char> rom;

	unsigned char* data();
	char* signed_data();

	// Parsed fields
	std::string_view app_name;

	// Debug
	void dump_header() const;

	std::string as_hex(unsigned start, unsigned end) const;
	std::string as_hex(unsigned at) const;

	// Checksum logic
	uint8_t header_checksum() const;
	uint16_t global_checksum() const;

	bool verify_header_checksum() const;
	bool verify_global_checksum() const;
};

#endif // ROMFILE_HPP
