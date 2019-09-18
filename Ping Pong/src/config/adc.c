/*
 * adc.c
 *
 * Created: 12.09.2019 10:16:31
 *  Author: nelsonz
 */ 
#include "adc.h"

enum adc_joystick_dir{LEFT, RIGHT, UP, DOWN, NEUTRAL};





volatile int adc_read(uint8_t channel){
	if((channel <= 4 )&&( channel > 0)){
		volatile uint8_t *p = ADC_OFFSET;
		//p[0] = channel;
		*p = channel + 3;//+3
		_delay_us(160);
		//volatile uint8_t read = p[0];
		
		return *p;
		//return read;
	}
	else {
		return 0;
	}
}
/*
int adc_slider_adjust(int value){
	int ans = value * 200;
	ans = ans/250;
	ans = ans - 100;
	if (ans<-100){
		ans = -100;
	}else if (ans>100){
		ans = 100;
	}return ans;
}

int adc_joystick_adjust(int value){
	int ans = value * 200;
	
	ans = ans/250;
	
	ans = ans - 100;
	
	if (ans<-100){
		ans = -100;
	}else if (ans>100){
		ans = 100;
	}else if ((ans<20) && (ans>-20)){
		ans = 0;
	}
	return ans;
}*/

int adc_get_slider_pos(int *left_s, int *right_s){
	*left_s = adc_read(3);
	*right_s = adc_read(4);
	//*left_s = adc_slider_adjust(left_temp);
	//*right_s = adc_slider_adjust(right_temp);
	return 0;
}

int adc_get_joystick_pos(int *y, int *x){
	*y = adc_read(2);
	*x = adc_read(1);
	//*y = adc_joystick_adjust(y_temp);
	//*x = adc_joystick_adjust(x_temp);
	return 0;
}

int adc_joystick_angle(){	//still problematic
	int x = 0, y = 0;
	x = adc_read(1)-140;
	y = adc_read(2)-140;
	int ans = atan(x/y);
	if (abs(x)<30 && abs(y)<30){
		ans = 0;
	}
	return ans;
}
/*
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
}*/

int adc_joystick_direction(){
	int x = 0, y = 0;
	x = adc_read(1)-140;
	y = adc_read(2)-140;
	int ans = NEUTRAL;
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
	}if (abs(x)<20 && abs(y)<20){
		ans = NEUTRAL;
	}
	return ans;
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
	return 0;
}