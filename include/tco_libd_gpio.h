#ifndef _TCO_LIBD_GPIO_H_
#define _TCO_LIBD_GPIO_H_

#include <stdint.h>
#include "tco_libd.h"

enum gpio_dir
{
    GPIO_DIR_IN = 0,
    GPIO_DIR_OUT,
    GPIO_DIR_NONE, /* Great to allow other methods such as RISING_EDGE etc */
};

enum gpio_val
{
    GPIO_VAL_LOW = 0,
    GPIO_VAL_HIGH,
};

typedef struct gpio_handle_t
{
    struct gpiod_chip *chip;
    struct gpiod_line *line;
} gpio_handle_t;

/**
 * @brief Initaites communication and ensures access to the GPIO kernel interface from userspace.
 * @param Handle Location of the handle which will be initialized.
 * @param chip_id ID of the GPIO chip to use. The pin requested must be connected to this chip.
 * @param dir Direction of GPIO comm.
 * @param pin GPIO pin to init.
 * @return "gpio_handle_t" with chip and line initialized on succeess or both NULL on failure.
*/
error_t gpio_handle_get(gpio_handle_t *const handle, uint8_t const chip_id, enum gpio_dir const dir, uint8_t const pin);

/**
 * @brief Write a value to a pin.
 * @param line Handle to the pin received from init function.
 * @param value What to write.
 * @return 0 on success, else on failure.
*/
error_t gpio_line_write(gpio_handle_t *const handle, enum gpio_val const value);

/**
 * @brief Read a value from a pin.
 * @param line Handle to the pin received from init function.
 * @param value Location where the read value will be written.
 * @return Status code.
*/
error_t gpio_line_read(gpio_handle_t *const handle, enum gpio_val *const value);

/**
 * @brief Close a GPIO pin.
 * @param line Handle to the pin received from init function.
 * @return Status code.
*/
error_t gpio_line_close(gpio_handle_t *const handle);

#endif /* _TCO_LIBD_GPIO_H_ */
