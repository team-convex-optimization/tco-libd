#include <stdio.h>
#include <stdint.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#include "tco_libd.h"

error_t i2c_port_open(uint8_t const interface_id, int *const fd_loc)
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
    return ERR_OK;
}

error_t i2c_cmd_write(int const i2c_port_fd, uint8_t const i2c_addr, uint8_t const cmd, uint8_t *const data, uint8_t const data_size)
{
    if (ioctl(i2c_port_fd, I2C_SLAVE, i2c_addr) < 0)
    {
        log_error("Failed to send the slave address");
        log_error("ioctl: %s", strerror(errno));
        return ERR_I2C_CTRL;
    }

    if (data_size == 0)
    {
        if (i2c_smbus_write_byte(i2c_port_fd, cmd) != 0)
        {
            log_error("i2c_smbus_write_byte: %s", strerror(errno));
            return ERR_I2C_WRITE;
        }
    }
    else if (data_size == 1)
    {
        if (i2c_smbus_write_byte_data(i2c_port_fd, cmd, data[0]) != 0)
        {
            log_error("i2c_smbus_write_byte_data: %s", strerror(errno));
            return ERR_I2C_WRITE;
        }
    }
    else
    {
        if (i2c_smbus_write_block_data(i2c_port_fd, cmd, data_size, data) != 0)
        {
            log_error("i2c_smbus_write_block_data: %s", strerror(errno));
            return ERR_I2C_WRITE;
        }
    }
    return ERR_OK;
}

error_t i2c_cmd_read(int const i2c_port_fd, uint8_t const i2c_addr, uint8_t const cmd, uint8_t *const output, uint8_t const output_size)
{
    if (ioctl(i2c_port_fd, I2C_SLAVE, i2c_addr) < 0)
    {
        log_error("Failed to send the slave address");
        log_error("ioctl: %s", strerror(errno));
        return ERR_I2C_CTRL;
    }

    if (output_size == 1)
    {
        int const output_tmp = i2c_smbus_read_byte_data(i2c_port_fd, cmd);
        if (output_tmp != -1)
        {
            output[0] = output_tmp;
        }
        else
        {
            log_error("Failed to read the byte from slave");
            log_error("i2c_smbus_read_byte_data: %s", strerror(errno));
            return ERR_I2C_READ;
        }
    }
    else
    {
        uint8_t output_size_actual = 0;
        /* "i2c_smbus_read_block_data" requires that output be at least 32 bits in size. */
        if (output_size < 4)
        {
            uint8_t output_tmp[4] = {0, 0, 0, 0};
            output_size_actual = i2c_smbus_read_block_data(i2c_port_fd, cmd, output_tmp);
            if (output_size_actual != output_size)
            {
                log_error("Did not receive the expected number of bytes, expected %u, got %u", output_size, output_size_actual);
                log_error("i2c_smbus_read_block_data: %s", strerror(errno));
                return ERR_I2C_READ;
            }
            else
            {
                memcpy(output, &output_tmp, output_size);
            }
        }
        else
        {
            output_size_actual = i2c_smbus_read_block_data(i2c_port_fd, cmd, output);
            if (output_size_actual != output_size)
            {
                log_error("Did not receive the expected number of bytes, expected %u, got %u", output_size, output_size_actual);
                log_error("i2c_smbus_read_block_data: %s", strerror(errno));
                return ERR_I2C_READ;
            }
        }
    }
    return ERR_OK;
}
