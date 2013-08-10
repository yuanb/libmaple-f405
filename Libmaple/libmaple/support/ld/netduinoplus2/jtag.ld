/*
 * Netduino Plus 2 (STM32F407VGT6, high density) linker script for
 * JTAG (bare metal, no bootloader) builds.
 */

MEMORY
{
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = 125K /*Top 3K reserved for stack */
  /* Note:  ROM Low-64K (0x08000000-0x0800FFFF) reserved for EEPROM emulation, boot */
  rom (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
}

GROUP(libcs4_stm32_high_density.a)

REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);
REGION_ALIAS("REGION_RODATA", rom);

_FLASH_BUILD = 1;
INCLUDE common.inc
