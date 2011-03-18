/* This file is part of OpenGarden
 * Copyright (C) 2011 Enrico Rossi
 *
 * OpenGarden is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenGarden is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _UART_H_
#define _UART_H_

/* UART baud rate */
#define UART_BAUD_0 9600
#define UART_BAUD_1 9600
#define UART_RXBUF_SIZE 64
#define UART_TXBUF_SIZE 64
#define UART_RXBUF_MASK ( UART_RXBUF_SIZE - 1 )
#define UART_TXBUF_MASK ( UART_TXBUF_SIZE - 1 )
#if ( UART_RXBUF_SIZE & UART_RXBUF_MASK )
#error RX buffer size is not a power of 2
#endif
#if ( UART_TXBUF_SIZE & UART_TXBUF_MASK )
#error TX buffer size is not a power of 2
#endif

struct uartStruct {
        char *rx_buffer;
        char *tx_buffer;
        volatile uint8_t rx_flag, tx_flag, rxIdx, txIdx;
};

void uart_init(const uint8_t port);
void uart_shutdown(const uint8_t port);
char uart_getchar(const uint8_t port, const uint8_t locked);
void uart_putchar(const uint8_t port, const char c);
void uart_printstr(const uint8_t port, const char *s);
void uart_printstrn(const uint8_t port, const char *s);

#endif
