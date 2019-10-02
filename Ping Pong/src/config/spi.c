/*
 * spi.c
 *
 * Created: 02.10.2019 09:23:54
 *  Author: nelsonz
 */ 
#include "spi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void spi_init() {
	/* Set MOSI and SCK and CS output, all others input */
	DDRB = (1 << PB5) | (1 << PB7) | (1 << PB4);
	PORTB |= 1 << PB4;
	/* Enable interrupt */

	// SPCR = 1<<SPIE;

	/* Enable SPI, Master, set clock rate fck/16 , SPI mode 0 by default*/
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_transmit_recieve(uint8_t *send_data, uint8_t *recieve_data, uint8_t data_length) {
	// Set chip select
	PORTB &= ~(1 << PB4);
	SPSR &= ~(1 << SPIF);
	for (int i = 0; i < data_length; i++) {
		SPDR = send_data[i];
		while (!(SPSR & (1 << SPIF)));
		recieve_data[i] = SPDR;
	}
	PORTB |= 1 << PB4;
}


void spi_receive(uint8_t *recieve_data, uint8_t data_length) {
	for (int i = 0; i < MAX_CAN_LENGTH; i++) {
		recieve_data[i] = 0;
	}
	uint8_t dummy_data[MAX_CAN_LENGTH];
	spi_transmit_recieve(dummy_data, recieve_data, data_length);
}

void spi_transmit(uint8_t *send_data, uint8_t data_length) {
	uint8_t dummy_recieve[MAX_CAN_LENGTH];
	spi_transmit_recieve(send_data, dummy_recieve, data_length);
}

void spi_write(uint8_t *send_data){
	spi_transmit(send_data, MAX_CAN_LENGTH);	//strlen(send_data)
}

uint8_t* spi_read(){
	uint8_t *receive_data;
	spi_receive(receive_data, MAX_CAN_LENGTH);
	return receive_data;
}