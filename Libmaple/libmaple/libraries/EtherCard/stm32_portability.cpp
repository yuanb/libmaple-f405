/*
 * stm32_portability.cpp
 *
 *  Created on: Aug 3, 2013
 *      Author: Scott
 */
#include "stm32_portability.h"

void cli() {
	noInterrupts();
}

void sei() {
	interrupts();
}

byte pgm_read_byte(const prog_char *addr) {
	return byte(*addr);
}

size_t strlen_P ( const prog_char * str ) {
	return strlen(str);
}

void * memcpy_P( void * destination, const void * source, size_t num ) {
	return memcpy(destination,source,num);
}



