/**
  ******************************************************************************
  * @file           : lcd_i2c.h
  * @author         : MrZloHex
  * @brief          : Library for liquid crystal displays with IIC expnder
  ******************************************************************************
  */

#ifndef __LCD_I2C_H__
#define __LCD_I2C_H__

#if   defined(STM32F0)
	#include "stm32f0xx_hal.h"
#elif defined(STM32F1)
	#include "stm32f1xx_hal.h"
#elif defined(STM32F3)
	#include "stm32f3xx_hal.h"
#elif defined(STM32F4)
	#include "stm32f4xx_hal.h"
#elif defined(STM32G0)
	#include "stm32g0xx_hal.h"
#elif defined(STM32G1)
	#include "stm32g1xx_hal.h"
#endif


typedef enum LCD_Size_E
{
	LCD_16x2   = 0x2U,
	LCD_16x4   = 0x3U,
	LCD_20x2   = 0x4U,
	LCD_20x4   = 0x5U,
	LCD_40x2   = 0x6U
} LCD_Size;

typedef struct LCD_I2C_S
{
	I2C_HandleTypeDef *hi2c;
	uint8_t 	address;
	uint32_t 	timeout;
	LCD_Size 	size;
} LCD_I2C;

/**
 * @brief Initilize LCD for an IIC bus (e.g. lcd_i2c_init(&lcd, &hi2c1, 0x4E, LCD_16x2, 10);)
 *
 * @param lcd		Pointer to LCD_I2C struct
 * @param i2c		IIC handler
 * @param address	Address of IIC expander connected to LCD (e.g. 0x4E for PCF8574)
 * @param size		Size of LCD (see LCD_Size enumeration)
 * @param timeout	Timeout value of blocking function of IIC transmit
 */
void
lcd_i2c_init
(
	LCD_I2C *lcd,
	I2C_HandleTypeDef *i2c,
	uint8_t  address,
	LCD_Size size,
	uint32_t timeout
);

/**
 * @brief Set cursor to position
 *
 * @param lcd		Pointer to LCD_I2C struct
 * @param x			Column
 * @param y			Row
 */
void
lcd_i2c_set_cursor(LCD_I2C *lcd, uint8_t x, uint8_t y);

/**
 * @brief Print string to set position
 *
 * @param lcd		Pointer to LCD_I2C struct
 * @param str		String to print
 */
void
lcd_i2c_print(LCD_I2C *lcd, const char *str);

/**
 * @brief Formatted print LCD
 *
 * @param lcd		Pointer to LCD_I2C struct
 * @param fmt		Format string
 * @param ...		va_list of formats
 */
void
lcd_i2c_printf
(
	LCD_I2C *lcd,
	const char * restrict fmt,
	...
);

/**
 * @brief Formattedd print to postition
 *
 * @param lcd		Pointer to LCD_I2C struct
 * @param x			Column
 * @param y			Row
 * @param fmt		Format string
 * @param ...		va_list of formats
 */
void
lcd_i2c_pos_printf
(
	LCD_I2C *lcd,
	uint8_t x,
	uint8_t y,
	const char * restrict fmt,
	...
);

/**
 * @brief Clear LCD
 *
 * @param lcd		Pointer to LCD_I2C struct
 */
void
lcd_i2c_clear(LCD_I2C *lcd);

HAL_StatusTypeDef
_lcd_i2c_cmd(LCD_I2C *lcd, uint8_t cmd);

HAL_StatusTypeDef
_lcd_i2c_data(LCD_I2C *lcd, uint8_t data);

#endif /* __LCD_I2C_H__ */

