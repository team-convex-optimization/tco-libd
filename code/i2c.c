#include <stdio.h>
#include <stdint.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#include "tco_libd.h"

i2c_error_t i2c_port_open(uint8_t const interface_id, int *fd_loc)
{
    if (interface_id > 9)
    {
        log_error("I2C adapter IDs greater than 9 are not supported");
        return ERR_CRIT;
    }
    char id_str = '0' + interface_id;
    char path[11] = "/dev/i2c- \0";
    path[9] = id_str;
    int fd = open(path, O_RDWR);
    if (fd == -1)
    {
        log_error("open: %s", strerror(errno));
        log_error("Tried to open '%s'", path);
        return ERR_CRIT;
    }
    *fd_loc = fd;
    return ERR_CRIT;
}

i2c_error_t i2c_cmd_write(int const i2c_port_fd, uint8_t const i2c_addr, uint8_t const cmd, uint8_t *const data, uint8_t const data_size)
{
    if (ioctl(i2c_port_fd, I2C_SLAVE, i2c_addr) < 0)
    {
        log_error("Failed to send the slave address");
        log_error("ioctl: %s", strerror(errno));
        return ERR_I2C_CTRL;
    }
    if (i2c_smbus_write_byte(i2c_port_fd, cmd) != 0)
    {
        log_error("Failed to send command");
        log_error("i2c_smbus_write_byte: %s", strerror(errno));
        return ERR_I2C_WRITE;
    }
    for (uint8_t data_idx = 0; data_idx < data_size; data_idx++)
    {
        if (i2c_smbus_write_byte(i2c_port_fd, data[data_idx]) != 0)
        {
            log_error("Failed to send command");
            log_error("i2c_smbus_write_byte: %s", strerror(errno));
            return ERR_I2C_WRITE;
        }
    }

    return ERR_OK;
}

i2c_error_t i2c_cmd_read(int const i2c_port_fd, uint8_t const i2c_addr, uint8_t const cmd, uint8_t *const output, uint8_t const output_size)
{
    if (ioctl(i2c_port_fd, I2C_SLAVE, i2c_addr) < 0)
    {
        log_error("Failed to send the slave address");
        log_error("ioctl: %s", strerror(errno));
        return ERR_I2C_CTRL;
    }

    if (i2c_smbus_write_byte(i2c_port_fd, cmd) != 0)
    {
        log_error("Failed to send command");
        log_error("i2c_smbus_write_byte: %s", strerror(errno));
        return ERR_I2C_WRITE;
    }
    for (uint8_t byte_idx = 0; byte_idx < output_size; byte_idx++)
    {
        int const output_tmp = i2c_smbus_read(i2c_port_fd);
        if (output_tmp < 0)
        {
            log_error("Failed to read byte number %i", byte_idx);
            log_error("i2c_smbus_read: %s", strerror(errno));
            return ERR_I2C_READ;
        }
        output[byte_idx] = output_tmp;
    }
    return ERR_OK;
}
