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
 * @file   discovery_f4.cpp
 * @author ala42
 * @brief  discovery_f4 board file.
 */

#ifdef BOARD_netduinoplus2

#include "netduinoplus2.h"

//#include "fsmc.h"
#include "gpio.h"
#include "rcc.h"
#include "timer.h"

#include "wirish_types.h"

//static void initSRAMChip(void);
void boardInit(void) {
	// remap TIMER2 (General Purpose Timer) to PA0-3
	gpio_set_af_mode(GPIOA, 0, 1);
	gpio_set_af_mode(GPIOA, 1, 1);
	gpio_set_af_mode(GPIOA, 2, 1);
	gpio_set_af_mode(GPIOA, 3, 1);

	// remap TIMER2 (General Purpose Timer) to PA0-3
	gpio_set_af_mode(GPIOB, 10, 1);

	// remap TIMER3 (General Purpose Timer) to PA6
	gpio_set_af_mode(GPIOA, 6, 2);

	// remap TIMER4 (General Purpose Timer) to PB6,7,8,9
	gpio_set_af_mode(GPIOB,  6, 2);
	gpio_set_af_mode(GPIOB,  7, 2);
	gpio_set_af_mode(GPIOB,  8, 2);
	gpio_set_af_mode(GPIOB,  9, 2);

	// The following configuration items were ported from the
	// Netduino Plus 2 firmware, file name IO_Init.c, function BootstrapCode_GPIO()

	// Configure MCO1 output, to output a 25Mhz clock for use by the onboar
	// ethernet controller; set to alternate function 0 (MCO1), 100Mhz speed, push-pull

	gpio_set_mode(GPIOA,8,GPIO_AF_OUTPUT_PP_100MHZ);
	gpio_set_af_mode(GPIOA,8,0);	// AF0 - system function, MCO1

    // optionally...turn on power headers automatically (important if we're debugging/deploying via COM1 on pins D0/D1)
    // PB2: CONTROL_OF_POWER_HEADERS
	gpio_set_mode(GPIOB,2,GPIO_OUTPUT_PP);
	gpio_write_bit(GPIOB,2,1);

	// optionally...turn on SD card power automatically
    // PB1: /PWR_CTRL_MICROSD
	gpio_set_mode(GPIOB,1,GPIO_OUTPUT_PP);
	gpio_write_bit(GPIOB,1,0);

	// optionally...turn on ENC28J60 power automatically and disable reset
    // PC15: /PWR_CTRL_ETHERNET
	gpio_set_mode(GPIOC,15,GPIO_OUTPUT_PP);
	gpio_write_bit(GPIOC,15,0);

	// PD2: /ENC_RESET
	gpio_set_mode(GPIOD,2,GPIO_INPUT_FLOATING);
	return;
}


#if 0
typedef struct stm32_pin_info {
    gpio_dev *gpio_device;      /**< Maple pin's GPIO device */
    timer_dev *timer_device;    /**< Pin's timer device, if any. */
    const adc_dev *adc_device;  /**< ADC device, if any. */
    uint8 gpio_bit;             /**< Pin's GPIO port bit. */
    uint8 timer_channel;        /**< Timer channel, or 0 if none. */
    uint8 adc_channel;          /**< Pin ADC channel, or ADCx if none. */
} stm32_pin_info;

#endif

// TODO:  The Timers, ADCs, timer channel, adc channel
// need to be fixed up.

extern const stm32_pin_info PIN_MAP[BOARD_NR_GPIO_PINS] = {
    {GPIOA, TIMER2, NULL,  0, 1, ADCx}, /* D00/PA0  */ // Silkscreen D8 (UART4 TX)
    {GPIOA, TIMER2, NULL,  1, 2, ADCx}, /* D01/PA1  */ // Silkscreen D7 (UART4 RX)
    {GPIOA, TIMER2, NULL,  2, 3, ADCx}, /* D02/PA2  */ // Silkscreen D3 (USART2 TX)
    {GPIOA, TIMER2, NULL,  3, 4, ADCx}, /* D03/PA3  */ // Silkscreen D2 (USART2 RX)
    {GPIOA,   NULL, NULL,  4, 0, ADCx}, /* D04/PA4  */ // /ENC_INT input
    {GPIOA,   NULL, NULL,  5, 0, ADCx}, /* D05/PA5  */ // ENC_SCK (SPI1)
    {GPIOA, TIMER3, NULL,  6, 1, ADCx}, /* D06/PA6  */ // Silkscreen D9
    {GPIOA,   NULL, NULL,  7, 0, ADCx}, /* D07/PA7  */ // ENC_MOSI (SPI1)
    {GPIOA,   NULL, NULL,  8, 0, ADCx}, /* D08/PA8  */ // Clock Out 25Mhz
    {GPIOA,   NULL, NULL,  9, 0, ADCx}, /* D09/PA9  */ // USB VBUS Input (USB_OTG_FS)
    {GPIOA,   NULL, NULL, 10, 0, ADCx}, /* D10/PA10 */ // LED Output
    {GPIOA,   NULL, NULL, 11, 0, ADCx}, /* D11/PA11 */ // USB DM (USB_OTG_FS)
    {GPIOA,   NULL, NULL, 12, 0, ADCx}, /* D12/PA12 */ // USB DP (USB_OTG_FS)
    {GPIOA,   NULL, NULL, 13, 0, ADCx}, /* D13/PA13 */ // JTMS-SWDIO
    {GPIOA,   NULL, NULL, 14, 0, ADCx}, /* D14/PA14 */ // JTCK-SWCLK
    {GPIOA,   NULL, NULL, 15, 0, ADCx}, /* D15/PA15 */ // JTDI

    {GPIOB,   NULL, NULL,  0, 0, ADCx}, /* D16/PB0  */ // /MICRO_SD_CS output
    {GPIOB,   NULL, NULL,  1, 0, ADCx}, /* D17/PB1  */ // /POWER_CTRL_MICROSD output
    {GPIOB,   NULL, NULL,  2, 0, ADCx}, /* D18/PB2  */ // BOOT1 input
    {GPIOB,   NULL, NULL,  3, 0, ADCx}, /* D19/PB3  */ // JTDO/TRACESWO
    {GPIOB,   NULL, NULL,  4, 0, ADCx}, /* D20/PB4  */ // ENC_MISO (SPI1)
    {GPIOB,   NULL, NULL,  5, 0, ADCx}, /* D21/PB5  */ // /MICRO_SD_CARD_INSERTED input
    {GPIOB, TIMER4, NULL,  6, 1, ADCx}, /* D22/PB6  */ // Silkscreen I2C1 SCL
    {GPIOB, TIMER4, NULL,  7, 2, ADCx}, /* D23/PB7  */ // Silkscreen I2C1 SDA
    {GPIOB, TIMER4, NULL,  8, 3, ADCx}, /* D24/PB8  */ // Silkscreen D5, I2C1 SCL ALT
    {GPIOB, TIMER4, NULL,  9, 4, ADCx}, /* D25/PB9  */ // Silkscreen D6, I2C1 SCL ALT
    {GPIOB, TIMER2, NULL, 10, 3, ADCx}, /* D26/PB10 */ // Silkscreen D10
    {GPIOB,   NULL, NULL, 11, 0, ADCx}, /* D27/PB11 */ // SWITCH input
    {GPIOB,   NULL, NULL, 12, 0, ADCx}, /* D28/PB12 */ // Silkscreen D4 (SPI2 NSS)
    {GPIOB,   NULL, NULL, 13, 0, ADCx}, /* D29/PB13 */ // Silkscreen D13 (SPI2 SCK)
    {GPIOB,   NULL, NULL, 14, 0, ADCx}, /* D30/PB14 */ // Silkscreen D12 (SPI2 MISO)
    {GPIOB,   NULL, NULL, 15, 0, ADCx}, /* D31/PB15 */ // Silkscreen D11 (SPI2 MOSI)

    {GPIOC,   NULL, ADC1,  0, 0,    0}, /* D32/PC0  */ // Silkscreen A0
    {GPIOC,   NULL, ADC1,  1, 0,    1}, /* D33/PC1  */ // Silkscreen A1
    {GPIOC,   NULL, ADC1,  2, 0,    2}, /* D34/PC2  */ // Silkscreen A2
    {GPIOC,   NULL, ADC1,  3, 0,    3}, /* D35/PC3  */ // Silkscreen A3
    {GPIOC,   NULL, ADC1,  4, 0,    4}, /* D36/PC4  */ // Silkscreen A4
    {GPIOC,   NULL, ADC1,  5, 0,    5}, /* D37/PC5  */ // Silkscreen A5
    {GPIOC,   NULL, NULL,  6, 0, ADCx}, /* D38/PC6  */ // Silkscreen D1 (USART6 TX)
    {GPIOC,   NULL, NULL,  7, 0, ADCx}, /* D39/PC7  */ // Silkscreen D0 (USART6 RX)
    {GPIOC,   NULL, NULL,  8, 0, ADCx}, /* D40/PC8  */ // /ENC_CS (SPI1)
    {GPIOC,   NULL, NULL,  9, 0, ADCx}, /* D41/PC9  */ // NOT CONNECTED
    {GPIOC,   NULL, NULL, 10, 0, ADCx}, /* D42/PC10 */ // SD_SCK (SPI3)
    {GPIOC,   NULL, NULL, 11, 0, ADCx}, /* D43/PC11 */ // SD_MISO (SPI3)
    {GPIOC,   NULL, NULL, 12, 0, ADCx}, /* D44/PC12 */ // SD_MOSI (SPI3)
    {GPIOC,   NULL, NULL, 13, 0, ADCx}, /* D45/PC13 */ // /POWER_LED output
    {GPIOC,   NULL, NULL, 14, 0, ADCx}, /* D46/PC14 */ // SWITCH input
    {GPIOC,   NULL, NULL, 15, 0, ADCx}, /* D47/PC15 */ // /PWR_CTRL_ETHERNET output

    {GPIOD,   NULL, NULL,  2, 0, ADCx}, /* D50/PD2  */ // /ENC_RESET
};


extern const uint8 boardPWMPins[BOARD_NR_PWM_PINS] __FLASH__ = {
   0,1,2,3,6,22,23,24,25,26
};

extern const uint8 boardADCPins[BOARD_NR_ADC_PINS] __FLASH__ = {
    32, 33, 34, 35, 36, 37
};

extern const uint8 boardUsedPins[BOARD_NR_USED_PINS] __FLASH__ = {
    BOARD_LED_PIN, BOARD_BUTTON_PIN, BOARD_JTMS_SWDIO_PIN,
    BOARD_JTCK_SWCLK_PIN, BOARD_JTDI_PIN, BOARD_JTDO_PIN,
};

/*
static void initSRAMChip(void) {
    fsmc_nor_psram_reg_map *regs = FSMC_NOR_PSRAM1_BASE;

    fsmc_sram_init_gpios();
    rcc_clk_enable(RCC_FSMC);

    regs->BCR = (FSMC_BCR_WREN | FSMC_BCR_MWID_16BITS | FSMC_BCR_MTYP_SRAM |
                 FSMC_BCR_MBKEN);
    fsmc_nor_psram_set_addset(regs, 0);
    fsmc_nor_psram_set_datast(regs, 3);
}
*/
#endif
