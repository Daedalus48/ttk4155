/*
 * adc.h
 *
 * Created: 12.09.2019 10:16:09
 *  Author: nelsonz
 */ 


#ifndef ADC_H_
#define ADC_H_


#endif /* ADC_H_ */

enum adc_joystick_dir{LEFT, RIGHT, UP, DOWN, NEUTRAL};
	
volatile int adc_read(uint8_t channel);

int adc_slider_adjust(int voltage);

int adc_joystick_adjust(int voltage);

int adc_get_slider_pos(int *left_s, int *right_s);

int adc_get_joystick_pos(int *y, int *x);

int adc_joystick_angle();

int adc_joystick_direction();

int adc_test_function();