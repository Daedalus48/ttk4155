/*
 * spi.h
 *
 * Created: 02.10.2019 09:24:06
 *  Author: nelsonz
 */ 
#ifndef SPI_H_
#define SPI_H_

#define MAX_CAN_LENGTH 10


#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

// Initialize spi as master
void spi_init();

void spi_transmit_recieve(uint8_t *send_data, uint8_t *recieve_data, uint8_t data_length);

// Reads as master data_length bytes of the SPI bus
void spi_receive(uint8_t *recieve_data, uint8_t data_length);

// Writes as master data_length bytes on the SPI bus
void spi_transmit(uint8_t *send_data, uint8_t data_length);

void spi_write(uint8_t *send_data);

uint8_t* spi_read();

#endif /* SPI_H_ */