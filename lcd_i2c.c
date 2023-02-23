/**
  ******************************************************************************
  * @file           : lcd_i2c.c
  * @author         : MrZloHex
  * @brief          : Library for liquid crystal displays with IIC expnder
  ******************************************************************************
  */

#include "lcd_i2c.h"
#include "stdio.h"
#include "stdarg.h"

void
lcd_i2c_init
(
	LCD_I2C *lcd,
	I2C_HandleTypeDef *i2c,
	uint8_t  address,
	LCD_Size size,
	uint32_t timeout
)
{
	lcd->hi2c    = i2c;
	lcd->address = address;
	lcd->timeout = timeout;
	lcd->size    = size;

	HAL_Delay(50);  // wait for >40ms
	_lcd_i2c_cmd(lcd, 0x30);
	HAL_Delay(5);  // wait for >4.1ms
	_lcd_i2c_cmd(lcd, 0x30);
	HAL_Delay(5);  // wait for >100us
	_lcd_i2c_cmd(lcd, 0x30);
	HAL_Delay(2);
	_lcd_i2c_cmd(lcd, 0x20);  // 4bit mode
	HAL_Delay(5);

	_lcd_i2c_cmd(lcd, 0x28);
	HAL_Delay(1);
	_lcd_i2c_cmd(lcd, 0x08);
	HAL_Delay(1);
	lcd_i2c_clear(lcd);
	_lcd_i2c_cmd(lcd, 0x06);
	HAL_Delay(1);
	_lcd_i2c_cmd(lcd, 0x0C);
	HAL_Delay(1);

}

void
lcd_i2c_set_cursor(LCD_I2C *lcd, uint8_t x, uint8_t y)
{
	const static uint8_t k_lcd_16_y[4] = {0x00, 0x40, 0x10, 0x50};
	const static uint8_t k_lcd_20_y[4] = {0x00, 0x40, 0x14, 0x54};

	if (y >= 4)
		return;

	uint8_t address = 0x80;
	if (lcd->size >= LCD_16x2 && lcd->size <= LCD_16x4)
	{
		address |= k_lcd_16_y[y] + x;
	}
	else
	{
		address |= k_lcd_20_y[y] + x;
	}

	_lcd_i2c_cmd(lcd, address);
}

void
lcd_i2c_print(LCD_I2C *lcd, const char *str)
{
	while (*str) _lcd_i2c_data(lcd, (uint8_t) *str++);
}

void
lcd_i2c_clear(LCD_I2C *lcd)
{
	_lcd_i2c_cmd(lcd, 0x01);
	HAL_Delay(2);
}

void
lcd_i2c_printf
(
	LCD_I2C *lcd,
	const char * restrict fmt,
	...
)
{
	char str[41];
	va_list args;
	va_start(args, fmt);
	vsnprintf(str, 41, fmt, args);
	va_end(args);;
	lcd_i2c_print(lcd, str);
}


void
lcd_i2c_pos_printf
(
	LCD_I2C *lcd,
	uint8_t x,
	uint8_t y,
	const char * restrict fmt,
	...
)
{
	lcd_i2c_set_cursor(lcd, x, y);
	char str[41];
	va_list args;
	va_start(args, fmt);
	vsnprintf(str, 41, fmt, args);
	va_end(args);;
	lcd_i2c_print(lcd, str);
}


HAL_StatusTypeDef
_lcd_i2c_cmd(LCD_I2C *lcd, uint8_t cmd)
{
	uint8_t data_u = (cmd        & 0xF0);
	uint8_t data_l = ((cmd << 4) & 0xF0);
	uint8_t data_t[4];
	data_t[0] = data_u | 0x0C;  //en=1, rs=0
	data_t[1] = data_u | 0x08;  //en=0, rs=0
	data_t[2] = data_l | 0x0C;  //en=1, rs=0
	data_t[3] = data_l | 0x08;  //en=0, rs=0
	return HAL_I2C_Master_Transmit
	(
		lcd->hi2c, lcd->address,
		data_t, 4, lcd->timeout
	);
}

HAL_StatusTypeDef
_lcd_i2c_data(LCD_I2C *lcd, uint8_t data)
{
	uint8_t data_u = (data        & 0xF0);
	uint8_t data_l = ((data << 4) & 0xF0);
	uint8_t data_t[4];
	data_t[0] = data_u | 0x0D;  //en=1, rs=1
	data_t[1] = data_u | 0x09;  //en=0, rs=1
	data_t[2] = data_l | 0x0D;  //en=1, rs=1
	data_t[3] = data_l | 0x09;  //en=0, rs=1
	return HAL_I2C_Master_Transmit
	(
		lcd->hi2c, lcd->address,
		data_t, 4, lcd->timeout
	);
}

