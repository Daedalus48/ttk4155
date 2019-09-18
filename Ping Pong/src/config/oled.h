/*
 * oled.h
 *
 * Created: 18.09.2019 11:21:07
 *  Author: nelsonz
 */ 


#ifndef OLED_H_
#define OLED_H_

#define OLED_PAGES 8
#define OLED_COLUMBS 128

#define OLED_COLUMB_SET 0x21
#define OLED_PAGE_SET 0x22

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

void oled_write_c(uint8_t data);

void oled_write_data(uint8_t data);

int oled_init();

void oled_columb_range_select(uint8_t start, uint8_t end);

void oled_page_select(uint8_t page);

void oled_clear_page(uint8_t page);

void oled_clear_screen();

void oled_print_char_of_size(char letter, uint8_t size);

uint8_t oled_print_char(char letter);

void oled_printf(char text[]);

#endif /* OLED_H_ */
