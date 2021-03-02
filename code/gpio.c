#include <gpiod.h>

#include "tco_libd.h"

/* XXX: A general problem is where two pins connected to the same chip get initialized then one of
them is freed. This might lead to all other pins on the chip that were previously initialized to not
work anymore. */

error_t gpio_handle_get(gpio_handle_t *const handle, uint8_t const chip_id, enum gpio_dir const dir, uint8_t const pin)
{
    if (chip_id > 9)
    {
        log_error("GPIO chip IDs greater than 9 are not supported");
        gpio_line_close(handle);
        return ERR_CRIT;
    }
    char id_str = '0' + chip_id;
    char gpio_chip_name[10] = "gpiochip \0";
    gpio_chip_name[8] = id_str;

    handle->chip = gpiod_chip_open_by_name(gpio_chip_name);
    if (!handle->chip)
    {
        log_error("Opening GPIO chip by name failed");
        gpio_line_close(handle);
        return ERR_GPIO_CTRL;
    }

    handle->line = gpiod_chip_get_line(handle->chip, pin);
    if (!handle->line)
    {
        log_error("Getting chip line (GPIO_ID : %d) failed", pin);
        gpio_line_close(handle);
        return ERR_GPIO_CTRL;
    }

    switch (dir)
    {
    case GPIO_DIR_IN:
        if (gpiod_line_request_input(handle->line, "tco_libd") < 0)
        {
            log_error("Request GPIO line as input failed");
            gpio_line_close(handle);
            return ERR_GPIO_CTRL;
        }
        break;
    case GPIO_DIR_OUT:
        if (gpiod_line_request_output(handle->line, "tco_libd", 0) < 0)
        {
            log_error("Request GPIO line as output failed");
            gpio_line_close(handle);
            return ERR_GPIO_CTRL;
        }
        break;
    }

    return ERR_OK;
}

error_t gpio_line_write(gpio_handle_t *const handle, enum gpio_val const value)
{
    if (gpiod_line_set_value(handle->line, value) != 0)
    {
        log_error("Failed to write to GPIO pin.");
        return ERR_GPIO_READ;
    }
    return ERR_OK;
}

error_t gpio_line_read(gpio_handle_t *const handle, enum gpio_val *const value)
{
    *value = gpiod_line_get_value(handle->line);
    if (*value < 0)
    {
        log_error("Failed to read from GPIO pin.");
        return ERR_GPIO_READ;
    }
    return ERR_OK;
}

error_t gpio_line_close(gpio_handle_t *const handle)
{
    if (handle->line != NULL)
    {
        gpiod_line_release(handle->line);
        handle->line = NULL;
    }
    if (handle->chip != NULL)
    {
        gpiod_chip_close(handle->chip);
        handle->chip = NULL;
    }
    return ERR_OK;
}
