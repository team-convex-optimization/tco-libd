#ifndef _TCO_LIBD_GPIO_H_
#define _TCO_LIBD_GPIO_H_

#include <stdint.h>

enum gpio_dir
{
    GPIO_IN = 0,
    GPIO_OUT,
};

enum gpio_value
{
    GPIO_LOW = 0,
    GPIO_HIGH,
};

typedef struct gpio_handle_t
{
    struct gpiod_chip *chip;
    struct gpiod_line *line;
} gpio_handle_t;

/**
 * @brief Initaites communication and ensures access to the GPIO kernel interface from userspace.
 * @param chip_id ID of the GPIO chip to use. The pin requested must be connected to this chip.
 * @param dir Direction of GPIO comm.
 * @param pin GPIO pin to init.
 * @return "gpio_handle_t" with chip and line initialized on succeess or both NULL on failure.
*/
gpio_handle_t gpio_line_init(uint8_t chip_id, enum gpio_dir dir, uint8_t pin);

/**
 * @brief Allows to write a value to a pin
 * @param line The gpiod_line of the pin. 
 * @param value What to write.
 * @return 0 on success, else on failure.
*/
int gpio_line_write(struct gpiod_line *line, unsigned int value);

/**
 * @brief Allows to read the value of a pin
 * @param line the gpiod_line to the pin. Must first be initialized by init_gpio_line
 * @return read value on success, else on failure
*/
int gpio_line_read(struct gpiod_line *line);

/**
 * @brief Close the gpio_chip
*/
void gpio_chip_close(struct gpiod_chip *chip);

/**
 * @brief Close a gpio_line (pin)
*/
void gpio_line_close(struct gpiod_line *line);

#endif /* _TCO_LIBD_GPIO_H_ */
