/*
 * Netduino Plus 2 (STM32F407VGT6, high density) linker script for
 * JTAG (bare metal, no bootloader) builds.
 */

MEMORY
{
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = 125K  /*Top 3K reserved for stack */
  rom (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K /* Flash - Code/ROData      */
}

GROUP(libcs4_stm32_high_density.a)

REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);
REGION_ALIAS("REGION_RODATA", rom);

_FLASH_BUILD = 1;

/* Note that for JTAG build, the low 32K is reserved for the */
/* vector table, the next two 16K blocks are reserved for    */
/* eeprom emulation.  So the remainder starts at 0x08010000. */

_CODE_START_OFFSET = 0x00010000; /* Offset code, allowing for EEPROM emulation */

INCLUDE common.inc

