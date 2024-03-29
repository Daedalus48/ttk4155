/*
 * can_driver.c
 *
 * Created: 02.10.2019 11:10:10
 *  Author: nelsonz
 */ 


#include "can_driver.h"
#include "bit_macros.h"
#include "MCP2515.h"


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <avr/io.h>
#include <stdio.h>

volatile int flag = 0;


ISR(INT0_vect){
	flag = 1;
	printf("interrupt happened \n\r");
}

void can_driver_init(){
	printf("can driver init \n \r");
	//mcp_2515_init(mode);
	can_init();
	
	can_write(MCP_CANINTE, MCP_RX_INT);

	// Disable global interrupts
	cli();
	// Interrupt on falling edge PD2
	set_bit(MCUCR, ISC01);
	clear_bit(MCUCR, ISC00);
	// Enable interrupt on PD2
	set_bit(GICR,INT0);
	// Enable global interrupts
	sei();
	
	
}

uint8_t can_interrupt(){
	if (flag){
		flag = 0;
		return 1;
	}
	return 0;
}

can_message* can_handle_messages(){
	uint8_t v[2] = {0};
	
	can_int_vect(v);
	can_message* message1;
	
	if (v[0]){
		can_message_receive(0, &message1);
		can_bit_modify(MCP_CANINTF, 1, 0);
		can_int_vect(v);
		if (!v[1]){
			flag = 0;
		}
		return message1;
	}
	
	can_message* message2;
	
	if (v[1]){
		can_message_receive(1, &message2);
		can_bit_modify(MCP_CANINTF, 2, 0);
		can_int_vect(v);
		if (!v[0]){
			flag = 0;
		}
		return message2;
	}
	
	return message1;
}

void can_message_send(can_message* message){
	static int buffer_number = 0;
	
	buffer_number +=1;
	if (buffer_number > 2) {
		buffer_number = 0;
	}
	
	while (!can_transmit_complete(buffer_number)){
		buffer_number +=1;
		if (buffer_number > 2) {
			buffer_number = 0;
		}
	}
	
	// Arbitration field identifier
	unsigned int ID = message->id;
	char id_high = ID / 8;
	char id_low = ID % 8;
	id_low = id_low*0b100000;
	can_write(MCP_TXB0SIDH + 16 * buffer_number, id_high); //sekvensen feiler her
	can_write(MCP_TXB0SIDL + 16 * buffer_number, id_low);
	
	// Define data length of message
	uint8_t data_length = message->length;
	can_write(MCP_TXB0DLC + 16 * buffer_number, data_length);
	
	// Write data bytes to transmit buffer
	char* data_bytes = message->data;
	for (uint8_t byte = 0; byte < data_length; byte++) {
		can_write(MCP_TXB0Dm + byte + 16 * buffer_number, data_bytes[byte]);
	}
	
	// Request to send message, send if successful
	can_rqt_send(MCP_RTS_TX0 + buffer_number);
	
}

int can_transmit_complete(int buffer_number){
	uint8_t transmit_flag = can_read(MCP_CANINTF);
	uint8_t interrupt_bits = (transmit_flag & (MCP_TX0IF + buffer_number*2));
	
	if(interrupt_bits == (MCP_TX0IF + buffer_number*2)){
		return 0;
	}
	return 1;
}

void can_message_receive(int rec_buff_num, can_message* received_message){
	uint8_t id_high = can_read(MCP_RXB0SIDH + 16 * rec_buff_num);
	uint8_t id_low = can_read(MCP_RXB0SIDL + 16 * rec_buff_num);
	uint8_t mask = 0b11100000;
	id_low = (id_low & mask);
	received_message->id = 0b1000*id_high + id_low/0b100000;
	
	uint8_t data_length = can_read(MCP_RXB0DLC + 16 * rec_buff_num);
	mask = 0b1111;
	received_message->length = (data_length & mask);
	
	for (uint8_t byte = 0; byte < data_length; byte++) {
		received_message->data[byte] = can_read(MCP_RXB0DM + byte + 16 * rec_buff_num);
	}
	
	
}

void can_int_vect(int* v) {
	uint8_t int_flag = can_read(MCP_CANINTF);
	v[0] = (int_flag & MCP_RX0IF);
	v[1] = (int_flag & MCP_RX1IF);
}

int can_error(void){
	uint8_t err = can_read(MCP_CANINTF);
	uint8_t mask = 0b00100000;
	if (mask & err == mask){
		//printf("Error in CAN!\n");
		can_bit_modify(MCP_CANINTF,mask,0);
		return 1;
	}
	return 0;
}

int can_get_flag(){
	return flag;
}