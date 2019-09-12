/*
 * adc.c
 *
 * Created: 12.09.2019 10:16:31
 *  Author: nelsonz
 */ 
#include "adc.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define F_CPU 4915200UL
#define FOSC 4915200 //Clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define BASE_ADDRESS 0x1000
#define ADC_OFFSET 0x1400



volatile int adc_read(uint8_t channel){
	if((channel <= 4 )&&( channel > 0)){
		volatile uint8_t *p = ADC_OFFSET;
		//p[0] = channel;
		*p = channel + 3;//+3
		_delay_us(60);
		//volatile uint8_t read = p[0];
		return *p;
		//return read;
	}
	else {
		return 0;
	}
}

int adc_slider_adjust(int voltage){
	return (voltage*200/255-100);
}

int adc_joystick_adjust(int voltage){
	int ans = (voktage-5)*200/250-100;
	if (ans<-100){
		ans = -100;
	}else if (ans>100){
		ans = 100;
	}else if (ans<16 && ans>-16){
		ans = 0;
	}return ans;
}

int adc_get_slider_pos(int *left_s, int *right_s){
	int left_temp = adc_read(3);
	int right_temp = adc_read(4);
	*left_s = adc_slider_adjust(left_temp);
	*right_s = adc_slider_adjust(right_temp);
	return 0;
}

int adc_get_joystick_pos(int *y, int *x){
	int y_temp = adc_read(1);
	int x_temp = adc_read(2);
	*y = adc_joystick_adjust(y_temp);
	*x = adc_joystick_adjust(x_temp);
	return 0;
}

int adc_joystick_angle(){
	int x = 0, y = 0;
	adc_get_joystick_pos(&y, &x);
	return atan(x/y);
}

int adc_joystick_direction(){
	int x = 0, y = 0;
	int ans = NEUTRAL;
	adc_get_joystick_pos(&y, &x);
	if (abs(x)<abs(y)){
		if (y<0){
			ans = DOWN;
		}else if(y>0){
			ans = UP;
		}
	}else if (abs(y)<abs(x)){
		if (x<0){
			ans = LEFT;
		}else if(x>0){
			ans = RIGHT;
		}
	}return ans;
}

int adc_test_function(){
	int x = 0, y = 0, left_s = 0, right_s = 0;
	
	adc_get_joystick_pos(&y, &x);
	adc_get_slider_pos(&left_s, &right_s);
	
	printf("x =  %d \r\n", x);
	printf("y =  %d \r\n", y);
	printf("left slider =  %d \r\n", left_s);
	printf("right slider =  %d \r\n", right_s);
	printf("joystick angle =  %d \r\n", adc_joystick_angle());
	printf("joystick direction =  %d \r\n", adc_joystick_direction());
}