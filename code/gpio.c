#include <gpiod.h>

#include "tco_libd.h"

gpio_handle_t gpio_line_init(uint8_t chip_id, enum gpio_dir dir, uint8_t pin)
{
    gpio_handle_t gpio_handle = {NULL, NULL};
    struct gpiod_chip *chip = NULL;
    struct gpiod_line *line = NULL;

    if (chip_id > 9)
    {
        log_error("GPIO chip IDs greater than 9 are not supported");
        goto clean_up;
    }
    char id_str = '0' + chip_id;
    char gpio_chip_name[10] = "gpiochip \0";
    gpio_chip_name[8] = id_str;

    chip = gpiod_chip_open_by_name(gpio_chip_name);
    if (!chip)
    {
        log_error("Opening GPIO chip by name failed");
        goto clean_up;
    }

    line = gpiod_chip_get_line(chip, pin);
    if (!line)
    {
        log_error("Getting chip line (GPIO_ID : %d) failed", pin);
        goto clean_up;
    }

    switch (dir)
    {
    case GPIO_IN:
        if (gpiod_line_request_input(line, "gpio_state") < 0)
        {
            log_error("Request GPIO line as input failed");
            goto clean_up;
        }
        break;
    case GPIO_OUT:
        if (gpiod_line_request_output(line, "Consumer", 0) < 0)
        {
            log_error("Request GPIO line as output failed");
            goto clean_up;
        }
        break;
    default:
        log_error("Unknown gpio_dir, got : %d", dir);
        goto clean_up;
    }

    gpio_handle.chip = chip;
    gpio_handle.line = line;
    return gpio_handle;

clean_up:
    if (chip != NULL)
    {
        gpiod_chip_close(chip);
    }
    if (line != NULL)
    {
        gpiod_line_release(line);
    }
    return gpio_handle;
}

int gpio_line_write(struct gpiod_line *line, enum gpio_value value)
{
    int ret = gpiod_line_set_value(line, value);
    if (ret < 0)
    {
        log_error("Failed to write to GPIO pin.");
    }
    return ret;
}

int gpio_line_read(struct gpiod_line *line)
{
    int ret = gpiod_line_get_value(line);
    if (ret < 0)
    {
        log_error("Failed to read from GPIO pin.");
    }
    return ret;
}

void gpio_line_close(struct gpiod_line *line)
{
    gpiod_line_release(line);
}

void gpio_chip_close(struct gpiod_chip *chip)
{
    gpiod_chip_close(chip);
}
