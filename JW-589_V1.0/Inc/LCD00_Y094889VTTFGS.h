/*!-----------------------------------------------------------------
 * @file 	[LCD00_Y094889VTTFGS.c]
 * @driver	[Sitronix/ST7037]
 *------------------------------------------------------------------
 */
#ifndef __LCD00_Y094889VTTFGS_H__
#define __LCD00_Y094889VTTFGS_H__


#include "LCD00_Y094889VTTFGS.tf.h"
#include "LCD00_Y094889VTTFGS.part.h"

enum{
	E_BATTERY_A,
	E_BATTERY_B,
};

enum{
	E_ODOGRAPH_ODO,
	E_ODOGRAPH_TRIP,
	E_ODOGRAPH_ERROR,
};

/*!-----------------------------------------------------------------
 */
extern part_t	_part;

/*!-----------------------------------------------------------------
 */
void lcd_display_clear_all_flags(void);
void lcd_display_show_all_symbols(void);
void lcd_display_animation(uint8_t step);
void lcd_display_ready_task(void);
void lcd_display_go_task(void);
void lcd_display_ready_task2(void);
#endif
