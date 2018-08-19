#include "video.hpp"
#include "cpu.hpp"

void Video::set_active_row(R8 row)
{
	_cpu.port_ram(Port::lcd_active_row) = row;

	if (row == _cpu.port_ram(Port::lcd_active_row_trigger)
	 && decode_status().active_row_trigger_interrupt)
	{

	}
}

void Video::set_status(R8 status)
{

}

LCDStatus Video::decode_status(R8 status)
{
	return {
		.active_row_trigger_interrupt = (status & (1 << 7)) != 0
	};
}
