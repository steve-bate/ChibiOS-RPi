#include "bcm2835.h"

/**
 * @brief Set GPIO pin function/mode. Used in multiple peripherals.
 *
 * @param[in] gpio_pin       BCM2835 pin number
 * @param[in] mode          GPIO pin function
 *
 * @notapi
 */
void bcm2835_gpio_fnsel(uint8_t gpio_pin, uint8_t gpio_fn) {
	int offset = gpio_pin / 10;

	unsigned long val = GPIO_REGS ->GPFSEL[offset].REG; // Read in the original register value.

	int item = gpio_pin % 10;
	val &= ~(0x7 << (item * 3));
	val |= ((gpio_fn & 0x7) << (item * 3));
	GPIO_REGS ->GPFSEL[offset].REG = val;
}

void set_gpio_direction(uint8_t pin_nr, uint8_t dir) {
	bcm2835_gpio_fnsel(pin_nr, dir);
}

/*
 *
 */
void set_gpio(uint8_t pin_nr, uint8_t pinVal) {
	unsigned long offset = pin_nr / 32;
	unsigned long mask = (1 << (pin_nr % 32));

	if (pinVal)
		GPIO_REGS ->GPSET[offset].REG |= mask;
	else
		GPIO_REGS ->GPCLR[offset].REG |= mask;

}

/*
 *
 */
uint32_t read_gpio(uint8_t pin_nr) {
	return ((GPIO_REGS ->GPLEV[pin_nr / 32].REG) >> (pin_nr % 32)) & 1;
}

void bcm2835_delay(uint32_t n) {
	volatile uint32_t i = 0;
	for (i = 0; i < n; i++)
		;
}

