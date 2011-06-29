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

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "tcn75.h"
#include "uart.h"

int main(void)
{
	float temp;
	uint8_t cfg;
	char *string;

	string = malloc(80);

	DDRA = 0;
	tcn75_init();
	uart_init(0);
	uart_printstrn(0, "begin");

	while(1) {
		loop_until_bit_is_clear(PINA, PA0);
		uart_printstrn(0, "click");

		cfg = 255;

		if (tcn75_read_config_reg(&cfg)) {
			uart_printstrn(0, "Error reading conf reg!");
		} else {
			uart_printstrn(0, "Config reg in hex");
			string = itoa(cfg, string, 16);
			uart_printstrn(0, string);
		}

		temp = tcn75_read_temperature();

		if (temp == -99) {
			uart_printstrn(0, "Error reading temp!");
		} else {
			uart_printstrn(0, "temp");
			string = dtostrf(temp, 3, 5, string);
			uart_printstrn(0, string);
		}

		_delay_ms(1000);
	}

	free(string);
	return(0);
}
