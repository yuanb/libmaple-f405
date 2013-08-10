/*
 * stm32_portability.h
 *
 *  Created on: Aug 3, 2013
 *      Author: Scott
 */

#ifndef STM32_PORTABILITY_H_
#define STM32_PORTABILITY_H_

/*****************************************
 * STM 32 Portability
 ****************************************/
#include "wirish.h"

typedef uint8 uint8_t;
typedef uint16 uint16_t;
typedef long unsigned int uint32_t;
typedef uint32 processor_word;
typedef int16 int16_t;
#include <string.h> // Required include for memset/memcpy
#include <ctype.h>
#include <stdio.h>
typedef unsigned short word; // a count, or a 16-bit data type
							 // for interacting w/the ENC28J60 registers.

/************************************
 * global disable/renable of interrupts
 */

void cli();
void sei();

/************************************
 * On STM32 platforms, Flash memory and
 * RAM are both addressable using standard
 * memory access.
 */

typedef const char prog_char;
typedef prog_char * PGM_P;	// Same as in arduino
byte pgm_read_byte( prog_char *addr);
size_t strlen_P ( prog_char * str );
#define PROGMEM __FLASH__	// on STM32, use __FLASH__ attribute
typedef char __FlashStringHelper; // on STM32
#define PSTR(str) (PGM_P)str

void * memcpy_P( void * destination, const void * source, size_t num );

#define Serial SerialUSB

#endif /* STM32_PORTABILITY_H_ */
