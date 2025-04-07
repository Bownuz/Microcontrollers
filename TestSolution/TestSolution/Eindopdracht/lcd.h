#ifndef LCD_H
#define LCD_H

#include <avr/io.h>

void lcd_init_4bits_mode(void);
void lcd_clear(void);
void lcd_write_string(char *str);

#endif
