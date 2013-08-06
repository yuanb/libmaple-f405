/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2011 LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file   netduinoplus2.h
 * @author Scott Libert <scott@kauailabs.com>
 * @brief  Private include file for Maple Native in boards.h
 *
 * See maple.h for more information on these definitions.
 */

#ifndef _BOARD_NETDUINO_PLUS_2_H_
#define _BOARD_NETDUINO_PLUS_2_H_

#define Port2Pin(port, bit) ((port-'A')*16+bit)

#define CYCLES_PER_MICROSECOND  168


#undef  STM32_PCLK1
#undef  STM32_PCLK2
#define STM32_PCLK1   (CYCLES_PER_MICROSECOND*1000000/4)
#define STM32_PCLK2   (CYCLES_PER_MICROSECOND*1000000/2)

#define SYSTICK_RELOAD_VAL      (CYCLES_PER_MICROSECOND*1000-1)

#define BOARD_LED_PIN           Port2Pin('A', 10)
#define BOARD_BUTTON_PIN        Port2Pin('C', 14)

/* Number of USARTs/UARTs whose pins are broken out to headers  */
/* Note that on netduinoplus2, UART1, 3 and 5 are not available */
/* as their associated pins are dedicated to other uses.        */

#define BOARD_NR_USARTS         3
#define BOARD_USART6_TX_PIN     Port2Pin('C', 6) // D1
#define BOARD_USART6_RX_PIN     Port2Pin('C', 7) // D0
#define BOARD_USART2_TX_PIN     Port2Pin('A', 2) // D3
#define BOARD_USART2_RX_PIN     Port2Pin('A', 3) // D2
#define BOARD_UART4_TX_PIN      Port2Pin('A', 0) // D8
#define BOARD_UART4_RX_PIN      Port2Pin('A', 1) // D7

#define BOARD_NR_SPI            3

// SPI1 ==> on-board Ethernet EMC28J60T

#define BOARD_SPI1_NSS_PIN      Port2Pin('C', 8) // /ENC_CS
#define BOARD_SPI1_MOSI_PIN     Port2Pin('A', 7) // ENC_MOSI
#define BOARD_SPI1_MISO_PIN     Port2Pin('B', 4) // ENC_MISO
#define BOARD_SPI1_SCK_PIN      Port2Pin('A', 5) // ENC_SCK

// SPI2 ==> Broken out to header as D4, D11, D12, D13

#define BOARD_SPI2_NSS_PIN      Port2Pin('B',12) // D4
#define BOARD_SPI2_MOSI_PIN     Port2Pin('B',15) // D11
#define BOARD_SPI2_MISO_PIN     Port2Pin('B',14) // D12
#define BOARD_SPI2_SCK_PIN      Port2Pin('B',13) // D13

// SPI3 ==> on-board Micro SD Card

#define BOARD_SPI3_NSS_PIN      Port2Pin('B', 0) // /MICROSD_CS
#define BOARD_SPI3_MOSI_PIN     Port2Pin('C', 12) // SPI3_MOSI
#define BOARD_SPI3_MISO_PIN     Port2Pin('C', 11) // SPI3_MISO
#define BOARD_SPI3_SCK_PIN      Port2Pin('C', 10) // SPI3_SCK

// NOTE:  The following I2C defs don't have to go in this file
// in order to conform to the libmaple standard, but are placed
// here for completeness.

#define BOARD_I2C1_SCL_PIN		Port2Pin('B',  6) // SCL
#define BOARD_I2C1_SDA_PIN		Port2Pin('B',  7) // SDA

#define BOARD_I2C1_ALT_SCL_PIN	Port2Pin('B',  8) // SCL, D5
#define BOARD_I2C1_ALT_SDA_PIN	Port2Pin('B',  9) // SDA, D6

#define BOARD_NR_GPIO_PINS      49 // GPIOA 0-15, GPIOB 0-15, GPIOC 0-15, GPIOD2
#define BOARD_NR_PWM_PINS       10 // All pins mapped to TIMERx
#define BOARD_NR_ADC_PINS       6
#define BOARD_NR_USED_PINS      6 // Set to same as maple, still not sure the meaning of this...
#define BOARD_JTMS_SWDIO_PIN    Port2Pin('A',13) // ok
#define BOARD_JTCK_SWCLK_PIN    Port2Pin('A',14) // ok
#define BOARD_JTDI_PIN          Port2Pin('A',15) // ok
#define BOARD_JTDO_PIN          Port2Pin('B', 3) // ok

#endif
