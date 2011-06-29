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

/*! \file i2c.h */

#ifndef TCN75
#define TCN75

#include "i2c.h"

/*! Slave write address 1001 000 R/W */
#define ADDR 0x90

/*! Register configuration
 * 10 bit resolution and shutdown mode
 */
#define TCN_CONF 0x21
/*! Time in msec to wait for a sample in
 * single shot mode.
 * Depend on the resolution setting.
 */
#define TCN_TSAMPLE 250

void tcn75_init(void);
uint8_t tcn75_read_config_reg(uint8_t *reg);
float tcn75_read_temperature(void);

#endif
