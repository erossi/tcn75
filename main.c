/* Copyright (C) 2011 Enrico Rossi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include "uart.h"

/*
PC0 SCL
PC1 SDA
PC2 IN ALLERT
PC3 - 5 OUT A0 - 2
*/

#define ADDR_W 0x9e /* 1001000 0 */
#define ADDR_R 0x9f /* 1001000 1 */

uint8_t i2c_send_start(void) {
	/* send start condition */
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
	/* wait for transmission */
	loop_until_bit_is_set(TWCR, TWINT);
	return(TW_STATUS);
}

void i2c_send_stop(void) {
	/* send stop condition */
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}

uint8_t i2c_send_sla_w(void) {
	TWDR = ADDR_W;
	/* clear interrupt to start transmission */
	TWCR = _BV(TWINT) | _BV(TWEN); 
	/* wait for transmission */
	loop_until_bit_is_set(TWCR, TWINT);
	return(TW_STATUS);
}

uint8_t i2c_send_sla_r(void) {
	TWDR = ADDR_R;
	/* clear interrupt to start transmission */
	TWCR = _BV(TWINT) | _BV(TWEN); 
	/* wait for transmission */
	loop_until_bit_is_set(TWCR, TWINT);
	return(TW_STATUS);
}

uint8_t i2c_send_data(const uint8_t data) {
	TWDR = data;
	/* clear interrupt to start transmission */
	TWCR = _BV(TWINT) | _BV(TWEN); 
	/* wait for transmission */
	loop_until_bit_is_set(TWCR, TWINT);
	return(TW_STATUS);
}

uint8_t i2c_send_ack(void) {
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	/* wait for transmission */
	loop_until_bit_is_set(TWCR, TWINT);
	return(TW_STATUS);
}

uint8_t i2c_send_nack(void) {
	TWCR = _BV(TWINT) | _BV(TWEN);
	/* wait for transmission */
	loop_until_bit_is_set(TWCR, TWINT);
	return(TW_STATUS);
}

int main(void)
{
	uint8_t err;
	int8_t temp;
	char *string;

	string = malloc(80);

	DDRA = 0;
	DDRC = _BV(3) | _BV(4) | _BV(5);
	PORTC = _BV(3) | _BV(4) | _BV(5);
	TWSR = 3;
	TWBR = 32;
	uart_init(0);
	uart_printstrn(0, "begin");

	while(1) {
		temp = -99;
		loop_until_bit_is_clear(PINA, PA0);
		uart_printstrn(0, "click");

		/* Send ADDR_W reg. 0 req. */
		err = i2c_send_start();

		if (err == TW_START)
			err = i2c_send_sla_w();
		else
			uart_printstrn(0, "ERR: send start!");

		if (err == TW_MT_SLA_ACK)
			err = i2c_send_data(0);
		else
			uart_printstrn(0, "ERR: send slaW!");

		if (err == TW_MT_DATA_ACK) {
			err = i2c_send_start();

			if (err == TW_REP_START)
				err = i2c_send_sla_r();

			if (err == TW_MR_SLA_ACK)
				err = i2c_send_nack();

			if (err == TW_MR_DATA_NACK) {
				temp = TWDR;
			} else {
				uart_printstrn(0, "ERR: recv DATA!");
			}
		} else {
			uart_printstrn(0, "ERR: send DATA!");
		}

		i2c_send_stop();
		string = itoa(temp, string, 10);
		uart_printstrn(0, string);
		string = itoa(err, string, 16);
		uart_printstrn(0, string);
		_delay_ms(1000);
    }

	free(string);
	return(0);
}
