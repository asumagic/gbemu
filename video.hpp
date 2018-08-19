#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "common.hpp"

class CPU;

struct LCDStatus
{
	bool active_row_trigger_interrupt;

};

class Video
{
	CPU& _cpu;

public:
	Video(CPU& cpu) :
		_cpu{cpu}
	{}

	void set_active_row(R8 row);
	void set_status(R8 status);
	LCDStatus decode_status(R8 status);
};

#endif // VIDEO_HPP
