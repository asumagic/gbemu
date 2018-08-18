#ifndef IO_HPP
#define IO_HPP

#include "common.hpp"

//************* make something flexible so we can read/write from ports in CPU using the same structure
AAAA
struct Ports
{
	struct
	{
		R8* div;
	} timers;

	struct
	{
		R8* control;
		R8* status;
		R8* bg_scrollx;
		R8* bg_scrolly;
		R8* active_row;
		R8* active_row_trigger;
		R8* windowx;
		R8* windowy;
		R8* palette_data;
		R8* object_palette0_data;
		R8* object_palette1_data;
		R8* dma_start_address;
	} lcd;
};

#endif // IO_HPP
