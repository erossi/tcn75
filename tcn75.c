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

/*
PC0 SCL
PC1 SDA
PC2 IN ALLERT
PC3 - 5 ADDRESS
*/

uint8_t tcn75_write_config_reg(const uint8_t cfg)
{
	if (i2c_master_send_w(ADDR, 1, cfg))
		return(1);
	else
		return(0);
}

uint8_t tcn75_read_config_reg(uint8_t *reg)
{
	if (i2c_master_send_b(ADDR, 1))
		return(1);
	else
		if (i2c_master_read_b(ADDR, reg))
			return(2);

	return(0);
}

/*! \brief take a temperature sample.
 * \fix should check the config register after the
 * delay to see if the sample has been taken.
 */
void tcn75_one_shot(void)
{
	tcn75_write_config_reg(TCN_CONF | 0x80);
	_delay_ms(TCN_TSAMPLE);
}

void tcn75_init(void)
{
	i2c_init();
	tcn75_write_config_reg(TCN_CONF);
}

/*! read the temperature and return it in a float
 */
float tcn75_read_temperature(void)
{
	uint16_t code;
	float temp;

	temp = -99;

	tcn75_one_shot();

	if (i2c_master_send_b(ADDR, 0)) {
		/* error */
	} else {
		if (i2c_master_read_w(ADDR, &code)) {
			/* Error */
		} else {
			/* casting uint16 to int16 */
			temp = (int16_t)code / 256.0;
		}
	}

	return(temp);
}
