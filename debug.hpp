#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

template<class T>
std::string debug_hex(T value)
{
	std::stringstream ss;

	ss << std::hex
	   << std::uppercase
	   << std::setw(sizeof(T) * 2)
	   << std::setfill('0')
	   << static_cast<unsigned long long>(value);

	return ss.str();
}

#endif // DEBUG_HPP
