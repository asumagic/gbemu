#ifndef IO_HPP
#define IO_HPP

#include "common.hpp"

enum class Port : R16
{
	joypad = 0xFF00,

	serial_data = 0xFF01,
	serial_control = 0xFF02,

	timer_div = 0xFF04,
	timer_counter = 0xFF05,
	timer_modulo = 0xFF06,
	timer_control = 0xFF07,

	lcd_control = 0xFF40,
	lcd_status = 0xFF41,
	lcd_bg_scrollx = 0xFF42,
	lcd_bg_scrolly = 0xFF43,
	lcd_active_row = 0xFF44,
	lcd_active_row_trigger = 0xFF45,
	lcd_windowx = 0xFF4A,
	lcd_windowy = 0xFF4B,
	lcd_palette_data = 0xFF47,
	lcd_object_palette0_data = 0xFF48,
	lcd_object_palette1_data = 0xFF49,
	lcd_dma_start_address = 0xFF46,

	interrupt_flag = 0xFF0F,
	interrupt_enable = 0xFFFF
};

#endif // IO_HPP
